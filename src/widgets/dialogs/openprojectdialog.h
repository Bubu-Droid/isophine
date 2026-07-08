#pragma once

#include <QDialog>

#include "dashboard.h"

namespace Ui {
class OpenProjectDialog;
}

class OpenProjectDialog: public QDialog {
  Q_OBJECT

public:
  explicit OpenProjectDialog(QWidget* parent = nullptr);
  ~OpenProjectDialog();

  ProjectData getProjectData() const;

private slots:
  void on_selectPdfButton_clicked();
  void on_selectLayoutPathButton_clicked();
  void on_selectOutDirButton_clicked();

private:
  Ui::OpenProjectDialog* ui;
  QString m_pdfPath;
  QString m_layoutPath;
  QString m_outDir;
};
