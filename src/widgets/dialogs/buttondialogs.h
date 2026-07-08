#pragma once

#include <QFileDialog>

class SelectPDFDialog: public QFileDialog {
  Q_OBJECT

public:
  explicit SelectPDFDialog(QWidget* parent = nullptr);
};

class SelectOutDirDialog: public QFileDialog {
  Q_OBJECT

public:
  explicit SelectOutDirDialog(QWidget* parent = nullptr);
};

class SelectLayoutFileDialog: public QFileDialog {
  Q_OBJECT

public:
  explicit SelectLayoutFileDialog(QWidget* parent = nullptr);
};
