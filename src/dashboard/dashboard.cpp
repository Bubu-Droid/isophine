#include "dashboard.h"

#include <sysexits.h>

#include <QByteArray>
#include <QDir>
#include <QIODeviceBase>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <cstdlib>

#include "messageboxes.h"
#include "newprojectdialog.h"
#include "openprojectdialog.h"
#include "projectcard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Dashboard),
    m_saveDirPath(
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
    ),
    m_projectsListFileName("isophine_projects_list.json") {
  ui->setupUi(this);

  validateDB();
  loadProjects();
}

Dashboard::~Dashboard() {
  delete ui;
}

void Dashboard::on_actionQuit_triggered() {
  close();
}

void Dashboard::on_actionNew_Project_triggered() {
  NewProjectDialog dialog = NewProjectDialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    ProjectData projDat = dialog.getProjectData();

    if (projDat.projectName.isEmpty() || projDat.pdfPath.isEmpty()
        || projDat.layoutPath.isEmpty() || projDat.projOutDir.isEmpty()) {
      ProjSettingsIncompleteMsgBox msgBox = ProjSettingsIncompleteMsgBox(this);
      msgBox.exec();
      return;
    }

    saveProject(projDat);
  }

  loadProjects();
}

void Dashboard::on_actionOpen_Existing_Project_triggered() {
  OpenProjectDialog dialog = OpenProjectDialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    ProjectData projDat = dialog.getProjectData();

    if (projDat.projectName.isEmpty() || projDat.pdfPath.isEmpty()
        || projDat.layoutPath.isEmpty() || projDat.projOutDir.isEmpty()) {
      ProjSettingsIncompleteMsgBox msgBox = ProjSettingsIncompleteMsgBox(this);
      msgBox.exec();
      return;
    }

    saveProject(projDat);
  }

  loadProjects();
}

void Dashboard::loadProjects() {
  if (!m_projectsListFile.open(QIODeviceBase::ReadOnly)) {
    return;
  }
  QJsonObject fileJsonObj =
      QJsonDocument::fromJson(m_projectsListFile.readAll()).object();
  m_projectsListFile.close();

  while (QLayoutItem* item = ui->verticalLayout->takeAt(0)) {
    if (QWidget* widget = item->widget()) {
      widget->deleteLater();
    }
    delete item;
  }

  for (auto it = fileJsonObj.begin(); it != fileJsonObj.end(); it++) {
    QJsonObject content = it.value().toObject();
    ProjectData projDat;
    projDat.projectName = content["project_name"].toString();
    projDat.pdfPath = content["pdf_path"].toString();
    projDat.layoutPath = content["layout_path"].toString();
    projDat.projOutDir = content["proj_out_dir"].toString();
    projDat.boundBoxHeight = content["bound_box_height"].toDouble();
    projDat.boundBoxWidth = content["bound_box_width"].toDouble();
    projDat.isExisting = content["is_existing"].toBool();

    ProjectCard* card = new ProjectCard(this, projDat);
    connect(card, &ProjectCard::openProject, this, &Dashboard::openProject);
    connect(card, &ProjectCard::editProject, this, &Dashboard::editProject);
    connect(card, &ProjectCard::deleteProject, this, &Dashboard::deleteProject);
    ui->verticalLayout->addWidget(card);
  }

  ui->verticalLayout->addStretch();
}

void Dashboard::saveProject(const ProjectData& projDat) {
  if (!m_projectsListFile.open(QIODeviceBase::ReadOnly)) {
    return;
  }
  QJsonObject fileJsonObj =
      QJsonDocument::fromJson(m_projectsListFile.readAll()).object();
  m_projectsListFile.close();

  for (auto it = fileJsonObj.begin(); it != fileJsonObj.end(); it++) {
    if (it.value().toObject()["project_name"] == projDat.projectName) {
      SameProjectNameMsgBox msgBox = SameProjectNameMsgBox(this);
      msgBox.exec();
      return;
    }
  }

  QString isoTimestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
  QJsonObject content;
  content["project_name"] = projDat.projectName;
  content["pdf_path"] = projDat.pdfPath;
  content["layout_path"] = projDat.layoutPath;
  content["proj_out_dir"] = projDat.projOutDir;
  content["bound_box_height"] = projDat.boundBoxHeight;
  content["bound_box_width"] = projDat.boundBoxWidth;
  content["is_existing"] = projDat.isExisting;

  fileJsonObj[isoTimestamp] = content;

  if (!m_projectsListFile.open(
          QIODeviceBase::WriteOnly | QIODeviceBase::Truncate
      )) {
    return;
  }

  m_projectsListFile.write(QJsonDocument(fileJsonObj).toJson());
  m_projectsListFile.close();
}

void Dashboard::openProject(const ProjectData& projDat) {
  qDebug() << "open button clicked";
}

void Dashboard::editProject(
    const QString& oldProjName,
    const ProjectData& projDat
) {
  if (!m_projectsListFile.open(QIODeviceBase::ReadOnly)) {
    return;
  }
  QJsonObject fileJsonObj =
      QJsonDocument::fromJson(m_projectsListFile.readAll()).object();
  m_projectsListFile.close();

  QString oldProjKey;
  QJsonObject content;
  content["project_name"] = projDat.projectName;
  content["pdf_path"] = projDat.pdfPath;
  content["layout_path"] = projDat.layoutPath;
  content["proj_out_dir"] = projDat.projOutDir;
  content["bound_box_height"] = projDat.boundBoxHeight;
  content["bound_box_width"] = projDat.boundBoxWidth;
  content["is_existing"] = projDat.isExisting;

  for (auto it = fileJsonObj.begin(); it != fileJsonObj.end(); it++) {
    if (it.value().toObject()["project_name"] == oldProjName) {
      oldProjKey = it.key();
      break;
    }
  }

  fileJsonObj[oldProjKey] = content;

  if (!m_projectsListFile.open(
          QIODeviceBase::WriteOnly | QIODeviceBase::Truncate
      )) {
    return;
  }
  m_projectsListFile.write(QJsonDocument(fileJsonObj).toJson());
  m_projectsListFile.close();

  loadProjects();
}

void Dashboard::deleteProject(const QString& projName) {
  DeleteConfirmationMsgBox msgBox = DeleteConfirmationMsgBox(this);
  if (msgBox.exec() == QMessageBox::No) {
    return;
  }
  if (!m_projectsListFile.open(QIODeviceBase::ReadOnly)) {
    return;
  }
  QJsonObject fileJsonObj =
      QJsonDocument::fromJson(m_projectsListFile.readAll()).object();
  m_projectsListFile.close();

  QString projKey;

  for (auto it = fileJsonObj.begin(); it != fileJsonObj.end(); it++) {
    if (it.value().toObject()["project_name"] == projName) {
      projKey = it.key();
      break;
    }
  }

  fileJsonObj.remove(projKey);

  if (!m_projectsListFile.open(
          QIODeviceBase::WriteOnly | QIODeviceBase::Truncate
      )) {
    return;
  }

  m_projectsListFile.write(QJsonDocument(fileJsonObj).toJson());
  m_projectsListFile.close();

  loadProjects();
}

void Dashboard::validateDB() {
  QDir dir;
  if (!dir.exists(m_saveDirPath)) {
    dir.mkpath(m_saveDirPath);
  }

  QString m_projectsListFilePath =
      QDir(m_saveDirPath).filePath(m_projectsListFileName);
  m_projectsListFile.setFileName(m_projectsListFilePath);

  if (!m_projectsListFile.exists()) {
    if (!m_projectsListFile.open(QIODeviceBase::WriteOnly)) {
      return;
    }
    m_projectsListFile.write("{}");
    m_projectsListFile.close();
  }

  if (!m_projectsListFile.open(QIODeviceBase::ReadOnly)) {
    FileReadFailedMsgBox msgBox =
        FileReadFailedMsgBox(this, m_projectsListFilePath);
    msgBox.exec();
    std::exit(EX_NOINPUT);
  }

  QJsonDocument fileJsonDoc =
      QJsonDocument::fromJson(m_projectsListFile.readAll());
  m_projectsListFile.close();

  if (!fileJsonDoc.isObject()) {
    DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
    msgBox.exec();
    std::exit(EX_DATAERR);
  }

  QJsonObject fileJsonParentObj = fileJsonDoc.object();

  for (auto it = fileJsonParentObj.begin(); it != fileJsonParentObj.end();
       it++) {
    if (!it.value().isObject()) {
      DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
      msgBox.exec();
      std::exit(EX_DATAERR);
    }

    QJsonObject fileJsonChildObj = it.value().toObject();

    if (fileJsonChildObj.count() != 7
        || !fileJsonChildObj.contains("project_name")
        || !fileJsonChildObj.contains("pdf_path")
        || !fileJsonChildObj.contains("layout_path")
        || !fileJsonChildObj.contains("proj_out_dir")
        || !fileJsonChildObj.contains("bound_box_height")
        || !fileJsonChildObj.contains("bound_box_width")
        || !fileJsonChildObj.contains("is_existing")) {
      DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
      msgBox.exec();
      std::exit(EX_CONFIG);
    }

    if (!fileJsonChildObj["project_name"].isString()
        || !fileJsonChildObj["pdf_path"].isString()
        || !fileJsonChildObj["layout_path"].isString()
        || !fileJsonChildObj["proj_out_dir"].isString()
        || !fileJsonChildObj["bound_box_height"].isDouble()
        || !fileJsonChildObj["bound_box_width"].isDouble()
        || !fileJsonChildObj["is_existing"].isBool()) {
      DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
      msgBox.exec();
      std::exit(EX_CONFIG);
    }
  }
}
