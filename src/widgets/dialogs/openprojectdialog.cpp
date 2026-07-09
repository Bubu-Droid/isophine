#include "openprojectdialog.h"

#include "buttondialogs.h"
#include "ui_openprojectdialog.h"

OpenProjectDialog::OpenProjectDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::OpenProjectDialog) {
  ui->setupUi(this);
  this->setWindowTitle("Open Existing Project");
}

OpenProjectDialog::~OpenProjectDialog() {
  delete ui;
}

void OpenProjectDialog::on_selectPdfButton_clicked() {
  SelectPDFDialog dialog = SelectPDFDialog(this);

  if (dialog.exec() == QDialog::Accepted) {
    m_pdfPath = dialog.selectedFiles().constFirst();
    ui->selectPdfButton->setText(QFileInfo(m_pdfPath).fileName());
  }
}

void OpenProjectDialog::on_selectLayoutPathButton_clicked() {
  SelectLayoutFileDialog dialog = SelectLayoutFileDialog(this);

  if (dialog.exec() == QDialog::Accepted) {
    m_layoutPath = dialog.selectedFiles().constFirst();
    ui->selectLayoutPathButton->setText(QFileInfo(m_layoutPath).fileName());
  }
}

void OpenProjectDialog::on_selectOutDirButton_clicked() {
  SelectOutDirDialog dialog = SelectOutDirDialog(this);

  if (dialog.exec() == QDialog::Accepted) {
    m_outDir = dialog.selectedFiles().constFirst();
    ui->selectOutDirButton->setText(QFileInfo(m_outDir).fileName());
  }
}

ProjectData OpenProjectDialog::getProjectData() const {
  ProjectData projDat;
  projDat.projectName = ui->projNameLineEdit->text();
  projDat.pdfPath = m_pdfPath;
  projDat.layoutPath = m_layoutPath;
  projDat.projOutDir = m_outDir;
  projDat.isExisting = true;

  return projDat;
}
