#include "projsettingsdialog.h"

#include <QFileInfo>

#include "buttondialogs.h"
#include "ui_projsettingsdialog.h"

ProjSettingsDialog::ProjSettingsDialog(
    QWidget* parent,
    const ProjectData& projDat
) :
    QDialog(parent),
    ui(new Ui::ProjSettingsDialog),
    m_projDat(projDat),
    m_pdfPath(projDat.pdfPath),
    m_layoutPath(projDat.layoutPath),
    m_outDir(projDat.projOutDir) {
  ui->setupUi(this);
  this->setWindowTitle("Edit Project Settings");

  ui->projNameLineEdit->setText(m_projDat.projectName);
  ui->selectPdfButton->setText(QFileInfo(m_projDat.pdfPath).fileName());
  ui->selectLayoutPathButton->setText(
      QFileInfo(m_projDat.layoutPath).fileName()
  );
  ui->selectOutDirButton->setText(QFileInfo(m_projDat.projOutDir).fileName());

  if (m_projDat.isExisting) {
    ui->selectOutPdfHeightSpinBox->setEnabled(false);
    ui->selectOutPdfWidthSpinBox->setEnabled(false);
  }
  ui->selectOutPdfHeightSpinBox->setValue(m_projDat.boundBoxHeight);
  ui->selectOutPdfWidthSpinBox->setValue(m_projDat.boundBoxWidth);
}

ProjSettingsDialog::~ProjSettingsDialog() {
  delete ui;
}

void ProjSettingsDialog::on_selectPdfButton_clicked() {
  SelectPDFDialog dialog =
      SelectPDFDialog(this, QFileInfo(m_projDat.pdfPath).absolutePath());

  if (dialog.exec() == QDialog::Accepted) {
    m_pdfPath = dialog.selectedFiles().constFirst();
    ui->selectPdfButton->setText(QFileInfo(m_pdfPath).fileName());
  }
}

void ProjSettingsDialog::on_selectLayoutPathButton_clicked() {
  SelectLayoutFileDialog dialog = SelectLayoutFileDialog(
      this,
      QFileInfo(m_projDat.layoutPath).absolutePath()
  );

  if (dialog.exec() == QDialog::Accepted) {
    m_layoutPath = dialog.selectedFiles().constFirst();
    ui->selectLayoutPathButton->setText(QFileInfo(m_layoutPath).fileName());
  }
}

void ProjSettingsDialog::on_selectOutDirButton_clicked() {
  SelectOutDirDialog dialog = SelectOutDirDialog(this, m_projDat.projOutDir);

  if (dialog.exec() == QDialog::Accepted) {
    m_outDir = dialog.selectedFiles().constFirst();
    ui->selectOutDirButton->setText(QFileInfo(m_outDir).fileName());
  }
}

ProjectData ProjSettingsDialog::getProjectData() const {
  ProjectData projDat;
  projDat.projectName = ui->projNameLineEdit->text();
  projDat.pdfPath = m_pdfPath;
  projDat.layoutPath = m_layoutPath;
  projDat.projOutDir = m_outDir;
  projDat.boundBoxHeight = ui->selectOutPdfHeightSpinBox->value();
  projDat.boundBoxWidth = ui->selectOutPdfWidthSpinBox->value();
  projDat.isExisting = projDat.isExisting;

  return projDat;
}
