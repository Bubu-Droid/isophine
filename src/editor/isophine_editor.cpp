#include "isophine_editor.h"

#include <qpdfdocument.h>

#include <QObject>
#include <QPdfDocument>

#include "project_settings.h"
#include "ui_isophine_editor.h"

IsophineEditor::IsophineEditor(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::IsophineEditor) {
  ui->setupUi(this);

  ProjectSettings::instance().pdfDocument = new QPdfDocument(this);
  QPdfDocument* pdfDocument = ProjectSettings::instance().pdfDocument;

  pdfDocument->load(ProjectSettings::instance().pdfPath);
  ui->thumbnailView->setModel(pdfDocument->pageModel());

  ProjectSettings::instance().pageTransformVector.reserve(
      pdfDocument->pageCount()
  );
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

void IsophineEditor::on_xOffsetDoubleSpinBox_editingFinished() {
  ui->xOffsetDoubleSpinBox->clearFocus();
}

void IsophineEditor::on_yOffsetDoubleSpinBox_editingFinished() {
  ui->yOffsetDoubleSpinBox->clearFocus();
}

void IsophineEditor::on_scaleDoubleSpinBox_editingFinished() {
  ui->scaleDoubleSpinBox->clearFocus();
}

void IsophineEditor::on_rotationDoubleSpinBox_editingFinished() {
  ui->rotationDoubleSpinBox->clearFocus();
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
