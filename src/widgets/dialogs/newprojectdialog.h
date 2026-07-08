#pragma once

#include <QDialog>

#include "dashboard.h"

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog: public QDialog {
  Q_OBJECT

public:
  explicit NewProjectDialog(QWidget* parent = nullptr);
  ~NewProjectDialog();

  ProjectData getProjectData() const;

private slots:
  void on_selectPdfButton_clicked();
  void on_selectOutDirButton_clicked();

private:
  Ui::NewProjectDialog* ui;
  QString m_pdfPath;
  QString m_outDir;
};
