#pragma once

#include <QColorDialog>
#include <QFileDialog>
#include <QProgressDialog>
#include <QStandardPaths>

class SelectPDFDialog: public QFileDialog {
  Q_OBJECT

public:
  SelectPDFDialog() = delete;
  explicit SelectPDFDialog(
      QWidget* parent = nullptr,
      QString setDirPath =
          QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
  );
};

class SelectOutDirDialog: public QFileDialog {
  Q_OBJECT

public:
  SelectOutDirDialog() = delete;
  explicit SelectOutDirDialog(
      QWidget* parent = nullptr,
      QString setDirPath =
          QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
  );
};

class SelectLayoutFileDialog: public QFileDialog {
  Q_OBJECT

public:
  SelectLayoutFileDialog() = delete;
  explicit SelectLayoutFileDialog(
      QWidget* parent = nullptr,
      QString setDirPath =
          QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
  );
};

class SelectGridlineColor: public QColorDialog {
  Q_OBJECT

public:
  SelectGridlineColor() = delete;
  explicit SelectGridlineColor(QWidget* parent = nullptr);
};

class OpenPDFProgress: public QProgressDialog {
  Q_OBJECT

public:
  OpenPDFProgress() = delete;
  explicit OpenPDFProgress(QWidget* parent = nullptr);
};
