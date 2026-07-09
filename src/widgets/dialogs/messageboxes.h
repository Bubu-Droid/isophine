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
  explicit SameProjectNameMsgBox(QWidget* parent = nullptr);
};

class ProjSettingsIncompleteMsgBox: public QMessageBox {
  Q_OBJECT

public:
  explicit ProjSettingsIncompleteMsgBox(QWidget* parent = nullptr);
};

class DeleteConfirmationMsgBox: public QMessageBox {
  Q_OBJECT

public:
  explicit DeleteConfirmationMsgBox(QWidget* parent = nullptr);
};
