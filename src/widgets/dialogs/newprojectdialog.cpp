#include "newprojectdialog.h"

#include "buttondialogs.h"
#include "ui_newprojectdialog.h"

NewProjectDialog::NewProjectDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog) {
  ui->setupUi(this);
  this->setWindowTitle("Create New Project");
}

NewProjectDialog::~NewProjectDialog() {
  delete ui;
}

void NewProjectDialog::on_selectPdfButton_clicked() {
  SelectPDFDialog dialog = SelectPDFDialog(this);

  if (dialog.exec() == QDialog::Accepted) {
    m_pdfPath = dialog.selectedFiles().constFirst();
    ui->selectPdfButton->setText(QFileInfo(m_pdfPath).fileName());
  }
}

void NewProjectDialog::on_selectOutDirButton_clicked() {
  SelectOutDirDialog dialog = SelectOutDirDialog(this);

  if (dialog.exec() == QDialog::Accepted) {
    m_outDir = dialog.selectedFiles().constFirst();
    ui->selectOutDirButton->setText(QFileInfo(m_outDir).fileName());
  }
}

ProjectData NewProjectDialog::getProjectData() const {
  ProjectData projDat;
  projDat.projectName = ui->projNameLineEdit->text();
  projDat.pdfPath = m_pdfPath;
  projDat.layoutPath = QDir(m_outDir).filePath(
      QString("%1_layout.json").arg(projDat.projectName)
  );
  projDat.projOutDir = m_outDir;
  projDat.boundBoxHeight = ui->selectOutPdfHeightSpinBox->value();
  projDat.boundBoxWidth = ui->selectOutPdfWidthSpinBox->value();

  return projDat;
}
