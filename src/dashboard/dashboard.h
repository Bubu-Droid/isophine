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
  qreal boundBoxHeight = 1.0;
  qreal boundBoxWidth = 1.0;
};

class Dashboard: public QMainWindow {
  Q_OBJECT

public:
  explicit Dashboard(QWidget* parent = nullptr);
  ~Dashboard() override;

private slots:
  void loadProjects();
  void saveProject(const ProjectData& projDat);
  void openProject(const ProjectData& projDat);
  void editProject(const QString& oldProjName, const ProjectData& projDat);
  void deleteProject(const QString& projName);
  void on_actionQuit_triggered();
  void on_actionNew_Project_triggered();
  void on_actionOpen_Existing_Project_triggered();
  void on_actionEdit_App_Settings_triggered();

signals:
  void openProjectToRoot(const ProjectData& projDat);
  void quitApp();

private:
  Ui::Dashboard* ui;
  void validateDB();
  bool validateProjectLayout(const ProjectData& projDat);
  QString m_saveDirPath;
  QString m_projectsListFileName;
  QFile m_projectsListFile;
  QFile m_layoutFile;
};
