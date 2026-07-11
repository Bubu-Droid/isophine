#include "isophine_editor.h"

#include "pageviewer.h"
#include "project_settings.h"
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
  emit quitApp();
}

void IsophineEditor::on_actionToggleInspector_triggered(bool checked) {
  ui->inspectorPanelDockWidget->setVisible(checked);
}

void IsophineEditor::on_actionToggleThumbnail_triggered(bool checked) {
  ui->thumbnailViewDockWidget->setVisible(checked);
  // TODO: add dialog messages on failures here later on
  // add a signal and show a message to the statusbar
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
}

void IsophineEditor::on_pageViewWidget_yOffsetChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  ui->yOffsetDoubleSpinBox->setValue(
      pageTransformVector[currentPageNo].yOffset
  );
}

void IsophineEditor::on_pageViewWidget_scaleChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  ui->scaleDoubleSpinBox->setValue(
      pageTransformVector[currentPageNo].scaleAmount
  );
}

void IsophineEditor::on_pageViewWidget_rotationChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  ui->rotationDoubleSpinBox->setValue(
      pageTransformVector[currentPageNo].rotationAmount
  );
}

void IsophineEditor::on_pageViewWidget_pageTransformChanged() {
  on_pageViewWidget_xOffsetChangedByKey();
  on_pageViewWidget_yOffsetChangedByKey();
  on_pageViewWidget_scaleChangedByKey();
  on_pageViewWidget_rotationChangedByKey();
}
