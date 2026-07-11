#pragma once

#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "dashboard.h"

class ProjectCard: public QWidget {
  Q_OBJECT

public:
  explicit ProjectCard(
      QWidget* parent = nullptr,
      const ProjectData& projDat = {}
  );

private slots:
  void on_openPushButton_clicked();
  void on_settingsPushButton_clicked();
  void on_deletePushButton_clicked();

signals:
  void openProjectToDash(const ProjectData& projDat);
  void editProject(const QString& oldProjName, const ProjectData& projDat);
  void deleteProject(const QString& projName);

private:
  QString m_projectsListFilePath;
  ProjectData m_projDat;

  QWidget* thumbnailPreviewWidget;
  QWidget* projectOverviewWidget;
  QWidget* controlButtonsWidget;

  QLabel* projectNameLabel;
  QLabel* pdfPathLabel;
  QLabel* projOutDirLabel;

  QPushButton* openPushButton;
  QPushButton* settingsPushButton;
  QPushButton* deletePushButton;
};
