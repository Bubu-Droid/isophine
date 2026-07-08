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

  validateConfig();
  loadProjects();
}

Dashboard::~Dashboard() {
  delete ui;
}

void Dashboard::on_actionQuit_triggered() {
  close();
}

void Dashboard::on_actionNew_Project_triggered() {
  NewProjectDialog newProjDialog = NewProjectDialog(this);
  if (newProjDialog.exec() == QDialog::Accepted) {
    ProjectData projDat = newProjDialog.getProjectData();

    if (projDat.projectName.isEmpty() || projDat.pdfPath.isEmpty()
        || projDat.layoutPath.isEmpty() || projDat.projOutDir.isEmpty()) {
      ProjSettingsIncompleteMsgBox msgBox = ProjSettingsIncompleteMsgBox(this);
      msgBox.exec();
      return;
    }

    saveProjects(projDat);
  }

  loadProjects();
}

void Dashboard::on_actionOpen_Existing_Project_triggered() {
  OpenProjectDialog openProjDialog = OpenProjectDialog(this);
  if (openProjDialog.exec() == QDialog::Accepted) {
    ProjectData projDat = openProjDialog.getProjectData();

    if (projDat.projectName.isEmpty() || projDat.pdfPath.isEmpty()
        || projDat.layoutPath.isEmpty() || projDat.projOutDir.isEmpty()) {
      ProjSettingsIncompleteMsgBox msgBox = ProjSettingsIncompleteMsgBox(this);
      msgBox.exec();
      return;
    }

    saveProjects(projDat);
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

  while (QLayoutItem* item = ui->projectListContainer->layout()->takeAt(0)) {
    Q_ASSERT(!item->layout());
    delete item->widget();
    delete item;
  }

  for (auto it = fileJsonObj.begin(); it != fileJsonObj.end(); it++) {
    QJsonObject content = it.value().toObject();
    ProjectData projDat = {
        content["project_name"].toString(),
        content["pdf_path"].toString(),
        content["layout_path"].toString(),
        content["proj_out_dir"].toString()
    };
    ProjectCard* card = new ProjectCard(this, projDat);
    ui->verticalLayout->addWidget(card);
  }

  ui->verticalLayout->addStretch();
}

void Dashboard::saveProjects(const ProjectData& projDat) {
  if (!m_projectsListFile.open(QIODeviceBase::ReadWrite)) {
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

  fileJsonObj[isoTimestamp] = content;

  if (!m_projectsListFile.open(
          QIODeviceBase::WriteOnly | QIODeviceBase::Truncate
      )) {
    return;
  }

  m_projectsListFile.write(QJsonDocument(fileJsonObj).toJson());
  m_projectsListFile.close();
}

void Dashboard::validateConfig() {
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

    if (fileJsonChildObj.count() != 4
        || !fileJsonChildObj.contains("project_name")
        || !fileJsonChildObj.contains("pdf_path")
        || !fileJsonChildObj.contains("layout_path")
        || !fileJsonChildObj.contains("proj_out_dir")) {
      DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
      msgBox.exec();
      std::exit(EX_CONFIG);
    }
  }
}
