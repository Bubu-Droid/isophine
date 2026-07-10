#pragma once

#include <QDialog>

namespace Ui {
class AppSettingsDialog;
}

class AppSettingsDialog: public QDialog {
  Q_OBJECT

public:
  explicit AppSettingsDialog(QWidget* parent = nullptr);
  ~AppSettingsDialog();

  void updateSettings();

private slots:
  void on_colorPushButton_clicked();

private:
  Ui::AppSettingsDialog* ui;
  QColor m_gridLineColor;
};
