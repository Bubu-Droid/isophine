#include <sysexits.h>

#include <QApplication>
#include <QColor>
#include <QSettings>
#include <QStackedWidget>
#include <QStatusBar>

#include "dashboard.h"
#include "isophine_editor.h"
#include "project_settings.h"

void refreshSettings(QSettings& settings);
void validateConfig(QSettings& settings);
void createConfigIfNeeded(QSettings& settings);

int main(int argc, char* argv[]) {
  QCoreApplication::setOrganizationName("isophine");
  QCoreApplication::setOrganizationDomain("github.com/bubu-droid");
  QCoreApplication::setApplicationName("isophine");
  QSettings::setDefaultFormat(QSettings::IniFormat);

  QSettings settings;
  createConfigIfNeeded(settings);
  validateConfig(settings);

  QApplication app(argc, argv);

  refreshSettings(settings);

  QStackedWidget rootContainer;

  Dashboard* dashWindow = new Dashboard(&rootContainer);
  IsophineEditor* editWindow = new IsophineEditor(&rootContainer);

  QObject::connect(
      editWindow,
      &IsophineEditor::goToHome,
      dashWindow,
      &Dashboard::loadProjects
  );
  QObject::connect(
      dashWindow,
      &Dashboard::openProjectToRoot,
      [&](const ProjectData& projDat) {
        editWindow->setPageCount(
            ProjectSettings::instance().pdfDocument->pageCount()
        );
        editWindow->loadThumbnails();
        editWindow->loadPage();
        dashWindow->statusBar()->clearMessage();
        rootContainer.setCurrentWidget(editWindow);
        rootContainer.setWindowTitle(
            QString("%1 — Isophine Editor")
                .arg(ProjectSettings::instance().projectName)
        );
      }
  );
  QObject::connect(editWindow, &IsophineEditor::goToHome, [&]() {
    rootContainer.setCurrentWidget(dashWindow);
    rootContainer.setWindowTitle("Isophine Dashboard");
  });
  QObject::connect(dashWindow, &Dashboard::quitApp, &app, &QApplication::quit);
  QObject::connect(
      editWindow,
      &IsophineEditor::quitApp,
      &app,
      &QApplication::quit
  );
  QObject::connect(
      editWindow,
      &IsophineEditor::updateTitle,
      [&](const QString& title) { rootContainer.setWindowTitle(title); }
  );

  QObject::connect(dashWindow, &Dashboard::refreshSettings, [&]() {
    refreshSettings(settings);
  });
  QObject::connect(editWindow, &IsophineEditor::refreshSettings, [&]() {
    refreshSettings(settings);
  });

  dashWindow->setWindowFlags(Qt::Widget);
  editWindow->setWindowFlags(Qt::Widget);

  rootContainer.addWidget(dashWindow);
  rootContainer.addWidget(editWindow);

  rootContainer.setCurrentWidget(dashWindow);
  rootContainer.setWindowTitle("Isophine Dashboard");

  // TODO: put the default dimensions after trialing
  // rootContainer.resize();
  rootContainer.show();

  return QApplication::exec();
}

void createConfigIfNeeded(QSettings& settings) {
  if (QFile(settings.fileName()).exists()) {
    return;
  }

  // settings.beginGroup("Canvas");
  // { settings.setValue("ZoomAmount", 0.25); }
  // settings.endGroup();

  settings.beginGroup("Increments");
  {
    settings.beginGroup("Translate");
    settings.setValue("SmallAmount", 1.0);
    settings.setValue("LargeAmount", 10.0);
    settings.endGroup();

    settings.beginGroup("Rotate");
    settings.setValue("SmallAmount", 0.1);
    settings.setValue("LargeAmount", 1.0);
    settings.endGroup();

    settings.beginGroup("Scale");
    settings.setValue("SmallAmount", 0.01);
    settings.setValue("LargeAmount", 0.1);
    settings.endGroup();
  }
  settings.endGroup();

  settings.beginGroup("BoundingBox");
  {
    settings.setValue("Scale", 1.0);
    // settings.setValue("Height", 1122.67);
    // settings.setValue("Width", 793.33);
    settings.setValue("LineColor", QColor(Qt::darkBlue));
    settings.setValue("HorizontalLinesCount", 5);
    settings.setValue("VerticalLinesCount", 3);
  }
  settings.endGroup();

  settings.beginGroup("Keybindings");
  {
    settings.beginGroup("Navigation");
    settings.setValue("NextKey", QKeySequence(Qt::Key_N));
    settings.setValue("PrevKey", QKeySequence(Qt::Key_P));
    settings.endGroup();

    settings.beginGroup("ScalePage");
    {
      settings.beginGroup("Up");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_Equal));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_Plus)
      );
      settings.endGroup();

      settings.beginGroup("Down");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_Minus));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_Underscore)
      );
      settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Movement");
    {
      settings.beginGroup("Left");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_H));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_H)
      );
      settings.endGroup();

      settings.beginGroup("Down");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_J));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_J)
      );
      settings.endGroup();

      settings.beginGroup("Up");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_K));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_K)
      );
      settings.endGroup();

      settings.beginGroup("Right");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_L));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_L)
      );
      settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Rotate");
    {
      settings.beginGroup("Clockwise");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_R));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_R)
      );
      settings.endGroup();

      settings.beginGroup("AntiClockwise");
      settings.setValue("SmallKey", QKeySequence(Qt::Key_E));
      settings.setValue(
          "LargeKey",
          QKeySequence(Qt::ShiftModifier | Qt::Key_E)
      );
      settings.endGroup();
    }
    settings.endGroup();
  }
  settings.endGroup();
}

void refreshSettings(QSettings& settings) {
  settings.beginGroup("Increments");
  {
    settings.beginGroup("Translate");
    ProjectSettings::instance().moveSmallAmount =
        settings.value("SmallAmount", 1.0).toDouble();
    ProjectSettings::instance().moveLargeAmount =
        settings.value("LargeAmount", 10.0).toDouble();
    settings.endGroup();

    settings.beginGroup("Rotate");
    ProjectSettings::instance().rotateSmallAmount =
        settings.value("SmallAmount", 0.1).toDouble();
    ProjectSettings::instance().rotateLargeAmount =
        settings.value("LargeAmount", 1.0).toDouble();
    settings.endGroup();

    settings.beginGroup("Scale");
    ProjectSettings::instance().scalePageSmallAmount =
        settings.value("SmallAmount", 0.01).toDouble();
    ProjectSettings::instance().scalePageLargeAmount =
        settings.value("LargeAmount", 0.1).toDouble();
    settings.endGroup();
  }
  settings.endGroup();

  settings.beginGroup("BoundingBox");
  {
    ProjectSettings::instance().boundBoxScale =
        settings.value("Scale", 1.0).toDouble();
    ProjectSettings::instance().boundBoxLineColor =
        settings.value("LineColor", QColor(Qt::darkBlue)).value<QColor>();
    ProjectSettings::instance().boundBoxHorLinesCount =
        settings.value("HorizontalLinesCount", 5).toInt();
    ProjectSettings::instance().boundBoxVerLinesCount =
        settings.value("VerticalLinesCount", 3).toInt();
  }
  settings.endGroup();

  settings.beginGroup("Keybindings");
  {
    settings.beginGroup("Navigation");
    ProjectSettings::instance().nextKey =
        settings.value("NextKey", QKeySequence(Qt::Key_N))
            .value<QKeySequence>();
    ProjectSettings::instance().prevKey =
        settings.value("PrevKey", QKeySequence(Qt::Key_P))
            .value<QKeySequence>();
    settings.endGroup();

    settings.beginGroup("ScalePage");
    {
      settings.beginGroup("Up");
      ProjectSettings::instance().scalePageUpSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_Equal))
              .value<QKeySequence>();
      ProjectSettings::instance().scalePageUpLargeKey =
          settings
              .value("LargeKey", QKeySequence(Qt::ShiftModifier | Qt::Key_Plus))
              .value<QKeySequence>();
      settings.endGroup();

      settings.beginGroup("Down");
      ProjectSettings::instance().scalePageDownSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_Minus))
              .value<QKeySequence>();
      ProjectSettings::instance().scalePageDownLargeKey =
          settings
              .value(
                  "LargeKey",
                  QKeySequence(Qt::ShiftModifier | Qt::Key_Underscore)
              )
              .value<QKeySequence>();
      settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Movement");
    {
      settings.beginGroup("Left");
      ProjectSettings::instance().leftSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_H))
              .value<QKeySequence>();
      ProjectSettings::instance().leftLargeKey =
          settings
              .value("LargeKey", QKeySequence(Qt::ShiftModifier | Qt::Key_H))
              .value<QKeySequence>();
      settings.endGroup();

      settings.beginGroup("Down");
      ProjectSettings::instance().downSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_J))
              .value<QKeySequence>();
      ProjectSettings::instance().downLargeKey =
          settings
              .value("LargeKey", QKeySequence(Qt::ShiftModifier | Qt::Key_J))
              .value<QKeySequence>();
      settings.endGroup();

      settings.beginGroup("Up");
      ProjectSettings::instance().upSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_K))
              .value<QKeySequence>();
      ProjectSettings::instance().upLargeKey =
          settings
              .value("LargeKey", QKeySequence(Qt::ShiftModifier | Qt::Key_K))
              .value<QKeySequence>();
      settings.endGroup();

      settings.beginGroup("Right");
      ProjectSettings::instance().rightSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_L))
              .value<QKeySequence>();
      ProjectSettings::instance().rightLargeKey =
          settings
              .value("LargeKey", QKeySequence(Qt::ShiftModifier | Qt::Key_L))
              .value<QKeySequence>();
      settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Rotate");
    {
      settings.beginGroup("Clockwise");
      ProjectSettings::instance().rotateClockSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_R))
              .value<QKeySequence>();
      ProjectSettings::instance().rotateClockLargeKey =
          settings
              .value("LargeKey", QKeySequence(Qt::ShiftModifier | Qt::Key_R))
              .value<QKeySequence>();
      settings.endGroup();

      settings.beginGroup("AntiClockwise");
      ProjectSettings::instance().rotateAntiClockSmallKey =
          settings.value("SmallKey", QKeySequence(Qt::Key_E))
              .value<QKeySequence>();
      ProjectSettings::instance().rotateAntiClockLargeKey =
          settings
              .value("LargeKey", QKeySequence(Qt::ShiftModifier | Qt::Key_E))
              .value<QKeySequence>();
      settings.endGroup();
    }
    settings.endGroup();
  }
  settings.endGroup();
}

void validateConfig(QSettings& settings) {
  if (settings.status() == QSettings::AccessError) {
    qDebug().noquote()
        << QString(
               "Permission to read configuration file not granted, cannot run application.\n\n"
               "Please add read permission flag to the configuration file.\n\n"
               "The file is present at the following path:\n"
               "%1"
           )
               .arg(settings.fileName());
    std::exit(EX_NOINPUT);
  } else if (settings.status() == QSettings::FormatError) {
    qDebug().noquote()
        << QString(
               "The configuration file is malformed!"
               " You may take a look at the default configuration given in the GitHub repository.\n\n"
               "The local configuration file is present at the following path:\n"
               "%1"
           )
               .arg(settings.fileName());
    std::exit(EX_CONFIG);
  }

  if ( // !settings.contains("Canvas/ZoomAmount") ||
      !settings.contains("Increments/Translate/SmallAmount")
      || !settings.contains("Increments/Translate/LargeAmount")
      || !settings.contains("Increments/Rotate/SmallAmount")
      || !settings.contains("Increments/Rotate/LargeAmount")
      || !settings.contains("Increments/Scale/SmallAmount")
      || !settings.contains("Increments/Scale/LargeAmount")
      || !settings.contains("BoundingBox/Scale")
      // || !settings.contains("BoundingBox/Height")
      // || !settings.contains("BoundingBox/Width")
      || !settings.contains("BoundingBox/LineColor")
      || !settings.contains("BoundingBox/HorizontalLinesCount")
      || !settings.contains("BoundingBox/VerticalLinesCount")
      || !settings.contains("Keybindings/Navigation/NextKey")
      || !settings.contains("Keybindings/Navigation/PrevKey")
      || !settings.contains("Keybindings/ScalePage/Up/SmallKey")
      || !settings.contains("Keybindings/ScalePage/Up/LargeKey")
      || !settings.contains("Keybindings/ScalePage/Down/SmallKey")
      || !settings.contains("Keybindings/ScalePage/Down/LargeKey")
      || !settings.contains("Keybindings/Movement/Left/SmallKey")
      || !settings.contains("Keybindings/Movement/Left/LargeKey")
      || !settings.contains("Keybindings/Movement/Right/SmallKey")
      || !settings.contains("Keybindings/Movement/Right/LargeKey")
      || !settings.contains("Keybindings/Movement/Up/SmallKey")
      || !settings.contains("Keybindings/Movement/Up/LargeKey")
      || !settings.contains("Keybindings/Movement/Down/SmallKey")
      || !settings.contains("Keybindings/Movement/Down/LargeKey")
      || !settings.contains("Keybindings/Rotate/Clockwise/SmallKey")
      || !settings.contains("Keybindings/Rotate/Clockwise/LargeKey")
      || !settings.contains("Keybindings/Rotate/AntiClockwise/SmallKey")
      || !settings.contains("Keybindings/Rotate/AntiClockwise/LargeKey")
  ) {
    qDebug().noquote()
        << QString(
               "The configuration has missing definition(s), you may want to reset the configuration to fix this issue."
               " You may take a look at the default configuration given in the GitHub repository.\n\n"
               "The local configuration file is present at the following path:\n"
               "%1"
           )
               .arg(settings.fileName());
    std::exit(EX_CONFIG);
  }
}
