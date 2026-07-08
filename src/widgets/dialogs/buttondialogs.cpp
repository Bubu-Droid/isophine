#include "buttondialogs.h"

#include <QStandardPaths>

SelectPDFDialog::SelectPDFDialog(QWidget* parent) : QFileDialog(parent) {
  this->setWindowTitle("Select Target PDF");
  this->setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
  );
  this->setMimeTypeFilters({"application/pdf"});
  this->setFileMode(QFileDialog::ExistingFile);
  this->setViewMode(QFileDialog::Detail);
  this->setAcceptMode(QFileDialog::AcceptOpen);
}

SelectOutDirDialog::SelectOutDirDialog(QWidget* parent) : QFileDialog(parent) {
  this->setWindowTitle("Select Output Directory");
  this->setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
  );
  this->setFileMode(QFileDialog::Directory);
  this->setOptions(QFileDialog::ShowDirsOnly);
  this->setViewMode(QFileDialog::Detail);
  this->setAcceptMode(QFileDialog::AcceptOpen);
}

SelectLayoutFileDialog::SelectLayoutFileDialog(QWidget* parent) :
    QFileDialog(parent) {
  this->setWindowTitle("Select Layout File");
  this->setDirectory(
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
  );
  this->setMimeTypeFilters({"application/json"});
  this->setFileMode(QFileDialog::ExistingFile);
  this->setViewMode(QFileDialog::Detail);
  this->setAcceptMode(QFileDialog::AcceptOpen);
}
