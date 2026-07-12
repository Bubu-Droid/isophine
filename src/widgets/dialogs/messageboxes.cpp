#include "messageboxes.h"

DBCorruptMsgBox::DBCorruptMsgBox(
    QWidget* parent,
    const QString& projListFilePath
) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Database Corrupted"));
  this->setText(tr(
      "The projects list database structure is malformed! Manual intervention needed, cannot run application."
  ));
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
      "This deletes the entry from the projects list database, not the project related files."
  );
  this->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  this->setIcon(QMessageBox::Warning);
}

ProjectSettingsDoNotMatch::ProjectSettingsDoNotMatch(QWidget* parent) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Project Settings Mismatch"));
  this->setText(tr(
      "The page count of the PDF document does not match the page count inside the layout file."
  ));
  this->setIcon(QMessageBox::Critical);
}

LayoutWriteFailed::LayoutWriteFailed(QWidget* parent) : QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Failed to Write to Layout File"));
  this->setText(tr(
      "Permission to write to layout file not granted, manual intervention needed."
  ));
  this->setInformativeText(
      "Please ensure that the layout file resides in a writable location."
  );
  this->setIcon(QMessageBox::Critical);
}

LayoutReadFailed::LayoutReadFailed(QWidget* parent) : QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Failed to Read Layout File"));
  this->setText(tr(
      "Permission to read layout file not granted, manual intervention needed."
  ));
  this->setInformativeText(
      "Please ensure that the layout file has read permission."
  );
  this->setIcon(QMessageBox::Critical);
}

LayoutCorruptMsgBox::LayoutCorruptMsgBox(
    QWidget* parent,
    const QString& layoutFilePath
) :
    QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Layout Malformed"));
  this->setText(tr(
      "The layout file structure is malformed! Manual intervention needed, cannot launch editor."
  ));
  this->setDetailedText(
      tr("Please open an issue on GitHub with the content of the layout file.\n\n"
         "The file is present at the following path:\n"
         "%1")
          .arg(layoutFilePath)
  );
  this->setIcon(QMessageBox::Critical);
}

PDFMissing::PDFMissing(QWidget* parent) : QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("PDF Missing"));
  this->setText(tr(
      "The PDF is not present at the path provided. Please update the project settings by re-selecting the PDF."
  ));
  this->setIcon(QMessageBox::Critical);
}

SaveUnsavedChanges::SaveUnsavedChanges(QWidget* parent) : QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::Save);
  this->setWindowTitle(tr("Save Unsaved Changes"));
  this->setText(tr("Do you want to save your changes?"));
  this->setIcon(QMessageBox::Information);
  this->setStandardButtons(
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
  );
}

ThumbSaveFailed::ThumbSaveFailed(QWidget* parent) : QMessageBox(parent) {
  this->setDefaultButton(QMessageBox::NoButton);
  this->setWindowTitle(tr("Failed to save thumbnail"));
  this->setText(tr("Failed to save thumbnail due to unknown reason."));
  this->setIcon(QMessageBox::Warning);
}
