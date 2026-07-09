#pragma once

#include <QDialog>

#include "dashboard.h"

namespace Ui {
class ProjSettingsDialog;
}

class ProjSettingsDialog: public QDialog {
  Q_OBJECT

public:
  explicit ProjSettingsDialog(
      QWidget* parent = nullptr,
      const ProjectData& projDat = {}
  );
  ~ProjSettingsDialog();

  ProjectData getProjectData() const;

private slots:
  void on_selectPdfButton_clicked();
  void on_selectLayoutPathButton_clicked();
  void on_selectOutDirButton_clicked();

private:
  Ui::ProjSettingsDialog* ui;
  const ProjectData& m_projDat;
  QString m_pdfPath;
  QString m_layoutPath;
  QString m_outDir;
};
