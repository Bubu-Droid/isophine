#include "isophine_editor.h"

#include <QObject>
#include <QPdfDocument>
#include <vector>

#include "pageviewer.h"
#include "project_settings.h"
#include "ui_isophine_editor.h"

IsophineEditor::IsophineEditor(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::IsophineEditor) {
  ui->setupUi(this);

  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;

  ui->thumbnailView->setModel(pdfDocument->pageModel());
}

IsophineEditor::~IsophineEditor() {
  delete ui;
}

void IsophineEditor::on_actionQuit_triggered() {
  close();
  // use QKeySequence::quit instead
}

void IsophineEditor::on_actionToggleInspector_triggered(bool checked) {
  ui->inspectorPanelDockWidget->setVisible(checked);
}

void IsophineEditor::on_actionToggleThumbnail_triggered(bool checked) {
  ui->thumbnailViewDockWidget->setVisible(checked);
  // gotta add dialog messages on failures here later on
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
  ProjectSettings::instance().boundBoxScale +=
      ProjectSettings::instance().zoomCanvasAmount;
  update();
}

void IsophineEditor::on_actionZoom_Out_triggered() {
  ProjectSettings::instance().boundBoxScale -=
      ProjectSettings::instance().zoomCanvasAmount;
  update();
}

void IsophineEditor::on_pageViewWidget_pageTransformChangedByKey() {
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  if (ui->xOffsetDoubleSpinBox->value()
      != pageTransformVector[currentPageNo].xOffset) {
    ui->xOffsetDoubleSpinBox->setValue(
        pageTransformVector[currentPageNo].xOffset
    );
  }
  if (ui->yOffsetDoubleSpinBox->value()
      != pageTransformVector[currentPageNo].yOffset) {
    ui->yOffsetDoubleSpinBox->setValue(
        pageTransformVector[currentPageNo].yOffset
    );
  }
  if (ui->scaleDoubleSpinBox->value()
      != pageTransformVector[currentPageNo].scaleAmount) {
    ui->scaleDoubleSpinBox->setValue(
        pageTransformVector[currentPageNo].scaleAmount
    );
  }
  if (ui->rotationDoubleSpinBox->value()
      != pageTransformVector[currentPageNo].rotationAmount) {
    ui->rotationDoubleSpinBox->setValue(
        pageTransformVector[currentPageNo].rotationAmount
    );
  }
}
