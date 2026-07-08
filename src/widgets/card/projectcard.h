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

private:
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
