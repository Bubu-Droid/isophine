#include "messageboxes.h"

DBCorruptMsgBox::DBCorruptMsgBox(
    QWidget* parent,
    const QString& projListFilePath
) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Database Corrupted"));
  this->setText(tr("The projects list database structure is malformed!"));
  this->setInformativeText(
      tr("Manual intervention needed, cannot run application.")
  );
  this->setDetailedText(
      tr("Please open an issue on GitHub with the content of the projects list database file.\n\n"
         "The file is present at the following path:\n"
         "%1")
          .arg(projListFilePath)
  );
  this->setIcon(QMessageBox::Critical);
}

FileReadFailedMsgBox::FileReadFailedMsgBox(
    QWidget* parent,
    const QString& projListFilePath
) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Failed to Read Database"));
  this->setText(tr(
      "Permission to read database file not granted, cannot run application."
  ));
  this->setDetailedText(
      tr("Please add read permission flag to the projects list database file.\n\n"
         "The file is present at the following path:\n"
         "%1")
          .arg(projListFilePath)
  );
  this->setIcon(QMessageBox::Critical);
}

SameProjectNameMsgBox::SameProjectNameMsgBox(QWidget* parent) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Duplicate Project Name"));
  this->setText(tr("Project names must be distinct!"));
  this->setIcon(QMessageBox::Warning);
}

ProjSettingsIncompleteMsgBox::ProjSettingsIncompleteMsgBox(QWidget* parent) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Incomplete Project Details"));
  this->setText(tr("All entries are compulsory, they must not be empty!"));
  this->setIcon(QMessageBox::Warning);
}

DeleteConfirmationMsgBox::DeleteConfirmationMsgBox(QWidget* parent) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Confirmation for Entry Deletion"));
  this->setText(tr("Are you sure you want to delete the project entry?"));
  this->setInformativeText(
      "This deletes the entry from the projects list database, not the project files."
  );
  this->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  this->setIcon(QMessageBox::Warning);
}
