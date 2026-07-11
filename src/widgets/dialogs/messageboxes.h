#pragma once

#include <QMessageBox>

class DBCorruptMsgBox: public QMessageBox {
  Q_OBJECT

public:
  DBCorruptMsgBox() = delete;
  explicit DBCorruptMsgBox(
      QWidget* parent = nullptr,
      const QString& projListFilePath = ""
  );
};

class FileReadFailedMsgBox: public QMessageBox {
  Q_OBJECT

public:
  FileReadFailedMsgBox() = delete;
  explicit FileReadFailedMsgBox(
      QWidget* parent = nullptr,
      const QString& projListFilePath = ""
  );
};

class SameProjectNameMsgBox: public QMessageBox {
  Q_OBJECT

public:
  SameProjectNameMsgBox() = delete;
  explicit SameProjectNameMsgBox(QWidget* parent = nullptr);
};

class ProjSettingsIncompleteMsgBox: public QMessageBox {
  Q_OBJECT

public:
  ProjSettingsIncompleteMsgBox() = delete;
  explicit ProjSettingsIncompleteMsgBox(QWidget* parent = nullptr);
};

class DeleteConfirmationMsgBox: public QMessageBox {
  Q_OBJECT

public:
  DeleteConfirmationMsgBox() = delete;
  explicit DeleteConfirmationMsgBox(QWidget* parent = nullptr);
};

class ProjectSettingsDoNotMatch: public QMessageBox {
  Q_OBJECT

public:
  ProjectSettingsDoNotMatch() = delete;
  explicit ProjectSettingsDoNotMatch(QWidget* parent = nullptr);
};

class LayoutWriteFailed: public QMessageBox {
  Q_OBJECT

public:
  LayoutWriteFailed() = delete;
  explicit LayoutWriteFailed(QWidget* parent = nullptr);
};

class LayoutReadFailed: public QMessageBox {
  Q_OBJECT

public:
  LayoutReadFailed() = delete;
  explicit LayoutReadFailed(QWidget* parent = nullptr);
};

class LayoutCorruptMsgBox: public QMessageBox {
  Q_OBJECT

public:
  LayoutCorruptMsgBox() = delete;
  explicit LayoutCorruptMsgBox(
      QWidget* parent = nullptr,
      const QString& layoutFilePath = ""
  );
};

class PDFMissing: public QMessageBox {
  Q_OBJECT

public:
  PDFMissing() = delete;
  explicit PDFMissing(QWidget* parent = nullptr);
};
