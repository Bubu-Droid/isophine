#include "dashboard.h"

#include <sysexits.h>

#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QIODeviceBase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QPdfDocument>
#include <QScreen>
#include <QStandardPaths>
#include <cstdlib>

#include "appsettingsdialog.h"
#include "messageboxes.h"
#include "newprojectdialog.h"
#include "openprojectdialog.h"
#include "project_settings.h"
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
  emit quitApp();
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

    m_layoutFile.setFileName(projDat.layoutPath);

    if (!validateProjectLayout(projDat)) {
      return;
    }

    if (!m_layoutFile.open(QIODeviceBase::ReadOnly)) {
      return;
    }
    QJsonObject layoutJsonObj =
        QJsonDocument::fromJson(m_layoutFile.readAll()).object();
    m_layoutFile.close();
    projDat.boundBoxHeight = layoutJsonObj["bound_box_height"].toDouble();
    projDat.boundBoxWidth = layoutJsonObj["bound_box_width"].toDouble();
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

  for (auto it = fileJsonObj.constBegin(); it != fileJsonObj.end(); ++it) {
    QJsonObject content = it.value().toObject();
    ProjectData projDat;
    projDat.projectName = content["project_name"].toString();
    projDat.pdfPath = content["pdf_path"].toString();
    projDat.layoutPath = content["layout_path"].toString();
    projDat.projOutDir = content["proj_out_dir"].toString();
    projDat.boundBoxHeight = content["bound_box_height"].toDouble();
    projDat.boundBoxWidth = content["bound_box_width"].toDouble();

    ProjectCard* card = new ProjectCard(this, projDat);
    connect(
        card,
        &ProjectCard::openProjectToDash,
        this,
        &Dashboard::openProject
    );
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

  for (auto it = fileJsonObj.constBegin(); it != fileJsonObj.end(); ++it) {
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
  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  m_layoutFile.setFileName(projDat.layoutPath);

  if (!validateProjectLayout(projDat)) {
    return;
  }

  if (!m_layoutFile.open(QIODeviceBase::ReadOnly)) {
    return;
  }
  QJsonObject layoutJsonObj =
      QJsonDocument::fromJson(m_layoutFile.readAll()).object();
  m_layoutFile.close();

  layoutJsonObj["bound_box_height"] = projDat.boundBoxHeight;
  layoutJsonObj["bound_box_width"] = projDat.boundBoxWidth;

  if (!m_layoutFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    return;
  }
  m_layoutFile.write(QJsonDocument(layoutJsonObj).toJson());
  m_layoutFile.close();

  if (!m_layoutFile.open(QIODeviceBase::ReadOnly)) {
    return;
  }
  layoutJsonObj = QJsonDocument::fromJson(m_layoutFile.readAll()).object();
  m_layoutFile.close();
  QJsonArray layoutJsonArr = layoutJsonObj["transform_array"].toArray();

  ProjectSettings::instance().isSaved = true;
  ProjectSettings::instance().currentPageNo =
      layoutJsonObj["current_page_no"].toInt();
  ProjectSettings::instance().projectName = projDat.projectName;
  ProjectSettings::instance().layoutPath = projDat.layoutPath;
  ProjectSettings::instance().projectOutDir = projDat.projOutDir;
  ProjectSettings::instance().pdfPath = projDat.pdfPath;
  ProjectSettings::instance().pdfOutPath =
      QDir(projDat.projOutDir)
          .filePath(QString("%1_output.pdf").arg(projDat.projectName));
  ProjectSettings::instance().boundBoxHeight = projDat.boundBoxHeight
      * (QGuiApplication::primaryScreen()->physicalDotsPerInchX() / 72.0);
  ProjectSettings::instance().boundBoxWidth = projDat.boundBoxWidth
      * (QGuiApplication::primaryScreen()->physicalDotsPerInchY() / 72.0);
  ProjectSettings::instance().pageTransformVector.resize(
      pdfDocument->pageCount()
  );
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  for (int i = 0; i != layoutJsonObj["transform_array"].toArray().count();
       ++i) {
    pageTransformVector[i].xOffset =
        layoutJsonArr[i].toObject()["x_offset"].toDouble();
    pageTransformVector[i].yOffset =
        layoutJsonArr[i].toObject()["y_offset"].toDouble();
    pageTransformVector[i].rotationAmount =
        layoutJsonArr[i].toObject()["rotation_amount"].toDouble();
    pageTransformVector[i].scaleAmount =
        layoutJsonArr[i].toObject()["scale_amount"].toDouble();
  }

  QSizeF pageSize = pdfDocument->pagePointSize(0);
  int thumbHeight = 150;
  int thumbWidth = (pageSize.width() / pageSize.height()) * thumbHeight;
  QSize renderSize(thumbWidth, thumbHeight);
  QImage thumbImg = pdfDocument->render(0, renderSize);
  QImage thumbBg(renderSize, QImage::Format_ARGB32_Premultiplied);
  thumbBg.fill(Qt::white);
  QPainter painter(&thumbBg);
  painter.drawImage(0, 0, thumbImg);
  painter.end();
  QString thumbPath = QDir(projDat.projOutDir).filePath("thumbnail.png");
  if (!thumbBg.save(thumbPath, "PNG")) {
    ThumbSaveFailed msgBox = ThumbSaveFailed(this);
    msgBox.exec();
  }

  emit openProjectToRoot(projDat);
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

  for (auto it = fileJsonObj.constBegin(); it != fileJsonObj.end(); ++it) {
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

  for (auto it = fileJsonObj.constBegin(); it != fileJsonObj.constEnd(); ++it) {
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

bool Dashboard::validateProjectLayout(const ProjectData& projDat) {
  QDir dir;
  if (!dir.exists(projDat.projOutDir)) {
    dir.mkpath(projDat.projOutDir);
  }

  if (!QFile(projDat.pdfPath).exists()) {
    PDFMissing msgBox = PDFMissing(this);
    msgBox.exec();
    return false;
  }

  if (!m_layoutFile.exists()) {
    if (!m_layoutFile.open(QIODeviceBase::WriteOnly)) {
      LayoutWriteFailed msgBox = LayoutWriteFailed(this);
      msgBox.exec();
      return false;
    }

    QJsonObject content;
    content["current_page_no"] = -1;
    content["bound_box_height"] = 1.0;
    content["bound_box_width"] = 1.0;
    content["transform_array"] = QJsonArray();
    m_layoutFile.write(QJsonDocument(content).toJson());
    m_layoutFile.close();
  }

  if (!m_layoutFile.open(QIODeviceBase::ReadOnly)) {
    LayoutReadFailed msgBox = LayoutReadFailed(this);
    msgBox.exec();
    return false;
  }
  QJsonDocument layoutJsonDoc = QJsonDocument::fromJson(m_layoutFile.readAll());
  m_layoutFile.close();

  if (!QFileInfo(m_layoutFile).isWritable()) {
    LayoutWriteFailed msgBox = LayoutWriteFailed(this);
    msgBox.exec();
    return false;
  }

  if (!layoutJsonDoc.isObject()) {
    LayoutCorruptMsgBox msgBox = LayoutCorruptMsgBox(this, projDat.layoutPath);
    msgBox.exec();
    return false;
  }

  QJsonObject layoutJsonObj = layoutJsonDoc.object();
  if (layoutJsonObj.count() != 4 || !layoutJsonObj.contains("current_page_no")
      || !layoutJsonObj.contains("bound_box_height")
      || !layoutJsonObj.contains("bound_box_width")
      || !layoutJsonObj.contains("transform_array")) {
    LayoutCorruptMsgBox msgBox = LayoutCorruptMsgBox(this);
    msgBox.exec();
    return false;
  }

  if (!layoutJsonObj.value("current_page_no").isDouble()
      || !layoutJsonObj.value("bound_box_height").isDouble()
      || !layoutJsonObj.value("bound_box_width").isDouble()
      || !layoutJsonObj.value("transform_array").isArray()) {
    LayoutCorruptMsgBox msgBox = LayoutCorruptMsgBox(this);
    msgBox.exec();
    return false;
  }

  ui->statusbar->showMessage(tr("Loading PDF..."));
  ui->statusbar->repaint();

  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  pdfDocument = new QPdfDocument(this);
  pdfDocument->load(projDat.pdfPath);

  if (layoutJsonObj.value("current_page_no").toInt() == -1) {
    QJsonArray transformArray;
    QJsonObject pageTransform;
    pageTransform["x_offset"] = 0.0;
    pageTransform["y_offset"] = 0.0;
    pageTransform["rotation_amount"] = 0.0;
    pageTransform["scale_amount"] = 1.0;
    for (int i = 0; i < pdfDocument->pageCount(); ++i) {
      transformArray.append(pageTransform);
    }

    layoutJsonObj["current_page_no"] = 0;
    layoutJsonObj["bound_box_height"] = 1.0;
    layoutJsonObj["bound_box_width"] = 1.0;
    layoutJsonObj["transform_array"] = transformArray;

    if (!m_layoutFile.open(
            QIODeviceBase::WriteOnly | QIODeviceBase::Truncate
        )) {
      return false;
    }
    m_layoutFile.write(QJsonDocument(layoutJsonObj).toJson());
    m_layoutFile.close();
  }

  if (!m_layoutFile.open(QIODeviceBase::ReadOnly)) {
    return false;
  }
  layoutJsonObj = QJsonDocument::fromJson(m_layoutFile.readAll()).object();
  m_layoutFile.close();

  if (layoutJsonObj["current_page_no"].toInt() < 0
      || layoutJsonObj["bound_box_height"].toDouble() < 1.0
      || layoutJsonObj["bound_box_width"].toDouble() < 1.0) {
    LayoutCorruptMsgBox msgBox = LayoutCorruptMsgBox(this);
    msgBox.exec();
    return false;
  }

  QJsonArray layoutJsonArr = layoutJsonObj["transform_array"].toArray();

  for (auto it = layoutJsonArr.constBegin(); it != layoutJsonArr.constEnd();
       ++it) {
    if (!it->isObject()) {
      LayoutCorruptMsgBox msgBox = LayoutCorruptMsgBox(this);
      msgBox.exec();
      return false;
    } else if (
        it->toObject().count() != 4
        || !it->toObject().contains("rotation_amount")
        || !it->toObject().contains("scale_amount")
        || !it->toObject().contains("x_offset")
        || !it->toObject().contains("y_offset")
    ) {
      LayoutCorruptMsgBox msgBox = LayoutCorruptMsgBox(this);
      msgBox.exec();
      return false;
    } else if (
        !it->toObject()["rotation_amount"].isDouble()
        || !it->toObject()["scale_amount"].isDouble()
        || !it->toObject()["x_offset"].isDouble()
        || !it->toObject()["y_offset"].isDouble()
    ) {
      LayoutCorruptMsgBox msgBox = LayoutCorruptMsgBox(this);
      msgBox.exec();
      return false;
    }
  }

  if (layoutJsonObj.value("transform_array").toArray().count()
      != pdfDocument->pageCount()) {
    ProjectSettingsDoNotMatch msgBox = ProjectSettingsDoNotMatch(this);
    msgBox.exec();
    return false;
  }

  return true;
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

  for (auto it = fileJsonParentObj.constBegin(); it != fileJsonParentObj.end();
       ++it) {
    if (!it.value().isObject()) {
      DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
      msgBox.exec();
      std::exit(EX_DATAERR);
    }

    QJsonObject fileJsonChildObj = it.value().toObject();

    if (fileJsonChildObj.count() != 6
        || !fileJsonChildObj.contains("project_name")
        || !fileJsonChildObj.contains("pdf_path")
        || !fileJsonChildObj.contains("layout_path")
        || !fileJsonChildObj.contains("proj_out_dir")
        || !fileJsonChildObj.contains("bound_box_height")
        || !fileJsonChildObj.contains("bound_box_width")) {
      DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
      msgBox.exec();
      std::exit(EX_CONFIG);
    }

    if (!fileJsonChildObj["project_name"].isString()
        || !fileJsonChildObj["pdf_path"].isString()
        || !fileJsonChildObj["layout_path"].isString()
        || !fileJsonChildObj["proj_out_dir"].isString()
        || !fileJsonChildObj["bound_box_height"].isDouble()
        || !fileJsonChildObj["bound_box_width"].isDouble()) {
      DBCorruptMsgBox msgBox = DBCorruptMsgBox(this, m_projectsListFilePath);
      msgBox.exec();
      std::exit(EX_CONFIG);
    }
  }
}

void Dashboard::on_actionEdit_App_Settings_triggered() {
  AppSettingsDialog dialog = AppSettingsDialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    dialog.updateSettings();
    emit refreshSettings();
  }
}
