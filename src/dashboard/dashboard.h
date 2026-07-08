#pragma once

#include <QFile>
#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui {
class Dashboard;
}

QT_END_NAMESPACE

struct ProjectData {
  QString projectName;
  QString pdfPath;
  QString layoutPath;
  QString projOutDir;
};

class Dashboard: public QMainWindow {
  Q_OBJECT

public:
  explicit Dashboard(QWidget* parent = nullptr);
  ~Dashboard() override;

private slots:
  void loadProjects();
  void saveProjects(const ProjectData& projDat);
  void on_actionQuit_triggered();
  void on_actionNew_Project_triggered();
  void on_actionOpen_Existing_Project_triggered();

private:
  Ui::Dashboard* ui;
  void validateConfig();
  QString m_saveDirPath;
  QString m_projectsListFileName;
  QFile m_projectsListFile;
};
