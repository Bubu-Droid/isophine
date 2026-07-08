#include "projectcard.h"

#include <QHBoxLayout>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QVBoxLayout>

ProjectCard::ProjectCard(QWidget* parent, const ProjectData& projDat) :
    QWidget(parent) {
  this->resize(800, 600);
  this->setWindowTitle("ProjectCard");

  QSizePolicy mainSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  mainSizePolicy.setHorizontalStretch(0);
  mainSizePolicy.setVerticalStretch(0);
  this->setSizePolicy(mainSizePolicy);

  QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
  horizontalLayout->setObjectName("horizontalLayout");

  thumbnailPreviewWidget = new QWidget(this);
  thumbnailPreviewWidget->setObjectName("thumbnailPreviewWidget");

  QSizePolicy thumbSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  thumbSizePolicy.setHorizontalStretch(0);
  thumbSizePolicy.setVerticalStretch(0);
  thumbnailPreviewWidget->setSizePolicy(thumbSizePolicy);

  horizontalLayout->addWidget(thumbnailPreviewWidget);

  projectOverviewWidget = new QWidget(this);
  projectOverviewWidget->setObjectName("projectOverviewWidget");

  QSizePolicy overviewSizePolicy(
      QSizePolicy::Expanding,
      QSizePolicy::Expanding
  );
  overviewSizePolicy.setHorizontalStretch(0);
  overviewSizePolicy.setVerticalStretch(0);
  projectOverviewWidget->setSizePolicy(overviewSizePolicy);

  QVBoxLayout* verticalLayout_2 = new QVBoxLayout(projectOverviewWidget);
  verticalLayout_2->setObjectName("verticalLayout_2");

  projectNameLabel = new QLabel(
      QString("Project Name: %1").arg(projDat.projectName),
      projectOverviewWidget
  );

  projectNameLabel->setObjectName("projectNameLabel");
  QFont font = projectNameLabel->font();
  font.setBold(true);
  projectNameLabel->setFont(font);
  verticalLayout_2->addWidget(projectNameLabel);

  QSpacerItem* verticalSpacer =
      new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  verticalLayout_2->addItem(verticalSpacer);

  pdfPathLabel = new QLabel(
      QString("PDF Path: %1").arg(projDat.pdfPath),
      projectOverviewWidget
  );
  pdfPathLabel->setObjectName("pdfPathLabel");
  verticalLayout_2->addWidget(pdfPathLabel);

  projOutDirLabel = new QLabel(
      QString("Project Output Directory: %1").arg(projDat.projOutDir),
      projectOverviewWidget
  );
  projOutDirLabel->setObjectName("projectOutDirLabel");
  verticalLayout_2->addWidget(projOutDirLabel);

  horizontalLayout->addWidget(projectOverviewWidget);

  controlButtonsWidget = new QWidget(this);
  controlButtonsWidget->setObjectName("controlButtonsWidget");

  QSizePolicy buttonSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  buttonSizePolicy.setHorizontalStretch(0);
  buttonSizePolicy.setVerticalStretch(0);
  controlButtonsWidget->setSizePolicy(buttonSizePolicy);

  QVBoxLayout* verticalLayout = new QVBoxLayout(controlButtonsWidget);
  verticalLayout->setObjectName("verticalLayout");

  openPushButton = new QPushButton("Open", controlButtonsWidget);
  openPushButton->setObjectName("openPushButton");
  verticalLayout->addWidget(openPushButton);

  settingsPushButton = new QPushButton("Settings", controlButtonsWidget);
  settingsPushButton->setObjectName("settingsPushButton");
  verticalLayout->addWidget(settingsPushButton);

  deletePushButton = new QPushButton("Delete", controlButtonsWidget);
  deletePushButton->setObjectName("deletePushButton");
  verticalLayout->addWidget(deletePushButton);

  horizontalLayout->addWidget(controlButtonsWidget);
}
