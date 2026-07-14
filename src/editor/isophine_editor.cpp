#include "isophine_editor.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "appsettingsdialog.h"
#include "buttondialogs.h"
#include "messageboxes.h"
#include "pageviewer.h"
#include "project_settings.h"
#include "renderer.h"
#include "ui_isophine_editor.h"

IsophineEditor::IsophineEditor(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::IsophineEditor) {
  ui->setupUi(this);
}

IsophineEditor::~IsophineEditor() {
  delete ui;
}

void IsophineEditor::loadPage() {
  ui->pageViewWidget->loadPage();
}

void IsophineEditor::setPageCount(int pageCount) {
  ui->pageViewWidget->setPageCount(pageCount);
}

void IsophineEditor::loadThumbnails() {
  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  ui->thumbnailView->setModel(pdfDocument->pageModel());
}

void IsophineEditor::on_actionQuit_triggered() {
  if (!ProjectSettings::instance().isSaved) {
    SaveUnsavedChanges msgBox = SaveUnsavedChanges(this);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
      on_actionSave_triggered();
    } else if (ret == QMessageBox::Cancel) {
      return;
    }
  }
  emit quitApp();
}

void IsophineEditor::on_actionHome_triggered() {
  if (!ProjectSettings::instance().isSaved) {
    SaveUnsavedChanges msgBox = SaveUnsavedChanges(this);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Save) {
      on_actionSave_triggered();
    } else if (ret == QMessageBox::Cancel) {
      return;
    }
  }
  emit goToHome();
}

void IsophineEditor::on_actionEditGlobalSettings_triggered() {
  AppSettingsDialog dialog = AppSettingsDialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    dialog.updateSettings();
    emit refreshSettings();
  }
}

void IsophineEditor::on_actionSave_triggered() {
  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  m_layoutFile.setFileName(ProjectSettings::instance().layoutPath);
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;

  if (!m_layoutFile.open(QIODeviceBase::ReadOnly)) {
    return;
  }
  QJsonObject layoutJsonObj =
      QJsonDocument::fromJson(m_layoutFile.readAll()).object();
  m_layoutFile.close();

  QJsonArray transformArray;
  QJsonObject pageTransform;
  for (auto it = pageTransformVector.begin(); it != pageTransformVector.end();
       ++it) {
    pageTransform["x_offset"] = it->xOffset;
    pageTransform["y_offset"] = it->yOffset;
    pageTransform["rotation_amount"] = it->rotationAmount;
    pageTransform["scale_amount"] = it->scaleAmount;
    transformArray.append(pageTransform);
  }

  layoutJsonObj["transform_array"] = transformArray;

  if (!m_layoutFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
    return;
  }
  m_layoutFile.write(QJsonDocument(layoutJsonObj).toJson());
  m_layoutFile.close();

  ui->statusbar->showMessage("Layout Saved.", 2000);
  ProjectSettings::instance().isSaved = true;
  emit updateTitle(QString("%1 — Isophine Editor")
                       .arg(ProjectSettings::instance().projectName));
}

void IsophineEditor::on_actionToggleInspector_triggered(bool checked) {
  ui->inspectorPanelDockWidget->setVisible(checked);
}

void IsophineEditor::on_actionToggleThumbnail_triggered(bool checked) {
  ui->thumbnailViewDockWidget->setVisible(checked);
}

void IsophineEditor::on_thumbnailView_activated(const QModelIndex& index) {
  ProjectSettings::instance().currentPageNo = index.row();
  ui->pageViewWidget->loadPage();
  ui->pageViewWidget->setFocus();
}

void IsophineEditor::on_xOffsetDoubleSpinBox_editingFinished() {
  ui->pageViewWidget->setFocus();
}

void IsophineEditor::on_xOffsetDoubleSpinBox_valueChanged(double arg1) {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  pageTransformVector[currentPageNo].xOffset =
      ui->xOffsetDoubleSpinBox->value();
  update();
}

void IsophineEditor::on_yOffsetDoubleSpinBox_editingFinished() {
  ui->pageViewWidget->setFocus();
}

void IsophineEditor::on_yOffsetDoubleSpinBox_valueChanged(double arg1) {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  pageTransformVector[currentPageNo].yOffset =
      ui->yOffsetDoubleSpinBox->value();
  update();
}

void IsophineEditor::on_scaleDoubleSpinBox_editingFinished() {
  ui->pageViewWidget->setFocus();
}

void IsophineEditor::on_scaleDoubleSpinBox_valueChanged(double arg1) {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  pageTransformVector[currentPageNo].scaleAmount =
      ui->scaleDoubleSpinBox->value();
  update();
}

void IsophineEditor::on_rotationDoubleSpinBox_editingFinished() {
  ui->pageViewWidget->setFocus();
}

void IsophineEditor::on_rotationDoubleSpinBox_valueChanged(double arg1) {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  pageTransformVector[currentPageNo].rotationAmount =
      ui->rotationDoubleSpinBox->value();
  update();
}

void IsophineEditor::on_actionZoom_In_triggered() {
  if (ProjectSettings::instance().boundBoxScale
          + ProjectSettings::instance().zoomCanvasAmount
      <= MAX_ZOOM) {
    ProjectSettings::instance().boundBoxScale +=
        ProjectSettings::instance().zoomCanvasAmount;
  }
  update();
}

void IsophineEditor::on_actionZoom_Out_triggered() {
  if (ProjectSettings::instance().boundBoxScale
          - ProjectSettings::instance().zoomCanvasAmount
      >= MIN_ZOOM) {
    ProjectSettings::instance().boundBoxScale -=
        ProjectSettings::instance().zoomCanvasAmount;
  }
  update();
}

void IsophineEditor::on_pageViewWidget_xOffsetChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  ui->xOffsetDoubleSpinBox->setValue(
      pageTransformVector[currentPageNo].xOffset
  );
  ProjectSettings::instance().isSaved = false;
  emit updateTitle(QString("%1* — Isophine Editor")
                       .arg(ProjectSettings::instance().projectName));
}

void IsophineEditor::on_pageViewWidget_yOffsetChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  ui->yOffsetDoubleSpinBox->setValue(
      pageTransformVector[currentPageNo].yOffset
  );
  ProjectSettings::instance().isSaved = false;
  emit updateTitle(QString("%1* — Isophine Editor")
                       .arg(ProjectSettings::instance().projectName));
}

void IsophineEditor::on_pageViewWidget_scaleChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  ui->scaleDoubleSpinBox->setValue(
      pageTransformVector[currentPageNo].scaleAmount
  );
  ProjectSettings::instance().isSaved = false;
  emit updateTitle(QString("%1* — Isophine Editor")
                       .arg(ProjectSettings::instance().projectName));
}

void IsophineEditor::on_pageViewWidget_rotationChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  ui->rotationDoubleSpinBox->setValue(
      pageTransformVector[currentPageNo].rotationAmount
  );
  ProjectSettings::instance().isSaved = false;
  emit updateTitle(QString("%1* — Isophine Editor")
                       .arg(ProjectSettings::instance().projectName));
}

void IsophineEditor::on_pageViewWidget_pageTransformChanged() {
  on_pageViewWidget_xOffsetChangedByKey();
  on_pageViewWidget_yOffsetChangedByKey();
  on_pageViewWidget_scaleChangedByKey();
  on_pageViewWidget_rotationChangedByKey();
  update();
}

void IsophineEditor::on_actionExport_triggered() {
  SelectPageBgColor dialog = SelectPageBgColor(this);
  if (dialog.exec() == QDialog::Accepted) {
    std::string result = PDFRenderer(
        ProjectSettings::instance().pdfPath,
        ProjectSettings::instance().pdfOutPath,
        ProjectSettings::instance().pageTransformVector,
        dialog.selectedColor(),
        QSizeF(
            ProjectSettings::instance().boundBoxWidth,
            ProjectSettings::instance().boundBoxHeight
        )
    );

    if (result.empty()) {
      ProjectRenderSuccess msgBox = ProjectRenderSuccess(this);
      msgBox.exec();
    } else {
      QFile logFile = QFile(
          QDir(ProjectSettings::instance().projectOutDir).filePath("log.txt")
      );

      if (!logFile.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate)) {
        return;
      }
      logFile.write(QString::fromStdString(result).toUtf8());
      logFile.close();
      ProjectRenderFailed msgBox = ProjectRenderFailed(this);
      msgBox.exec();
    }
  }
}
