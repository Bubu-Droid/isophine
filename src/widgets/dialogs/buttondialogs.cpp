#include "buttondialogs.h"

#include <qnamespace.h>

SelectPDFDialog::SelectPDFDialog(QWidget* parent, QString setDirPath) :
    QFileDialog(parent) {
  this->setWindowTitle("Select Target PDF");
  this->setDirectory(setDirPath);
  this->setMimeTypeFilters({"application/pdf"});
  this->setFileMode(QFileDialog::ExistingFile);
  this->setViewMode(QFileDialog::Detail);
  this->setAcceptMode(QFileDialog::AcceptOpen);
}

SelectOutDirDialog::SelectOutDirDialog(QWidget* parent, QString setDirPath) :
    QFileDialog(parent) {
  this->setWindowTitle("Select Output Directory");
  this->setDirectory(setDirPath);
  this->setFileMode(QFileDialog::Directory);
  this->setOptions(QFileDialog::ShowDirsOnly);
  this->setViewMode(QFileDialog::Detail);
  this->setAcceptMode(QFileDialog::AcceptOpen);
}

SelectLayoutFileDialog::SelectLayoutFileDialog(
    QWidget* parent,
    QString setDirPath
) :
    QFileDialog(parent) {
  this->setWindowTitle("Select Layout File");
  this->setDirectory(setDirPath);
  this->setMimeTypeFilters({"application/json"});
  this->setFileMode(QFileDialog::ExistingFile);
  this->setViewMode(QFileDialog::Detail);
  this->setAcceptMode(QFileDialog::AcceptOpen);
}

SelectGridlineColor::SelectGridlineColor(QWidget* parent) :
    QColorDialog(parent) {
  this->setWindowTitle("Select Gridline Color");
}

OpenPDFProgress::OpenPDFProgress(QWidget* parent) : QProgressDialog(parent) {
  this->setWindowTitle("Opening Project");
  this->setMinimum(0);
  this->setMaximum(100);
  this->setCancelButton(nullptr);
  this->setLabelText("Loading PDF...");
  this->setWindowModality(Qt::WindowModal);
}
