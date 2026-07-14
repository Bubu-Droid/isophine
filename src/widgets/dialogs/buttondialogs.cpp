#include "buttondialogs.h"

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

SelectPageBgColor::SelectPageBgColor(QWidget* parent) : QColorDialog(parent) {
  this->setWindowTitle("Select Page Background Color");
}
