#include <sysexits.h>

#include <QApplication>
#include <QFile>
#include <QSettings>

#include "dashboard.h"
#include "isophine_editor.h"

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
  Dashboard window;
  window.show();
  // IsophineEditor w;
  // w.show();
  return QApplication::exec();
}

void createConfigIfNeeded(QSettings& settings) {
  if (QFile(settings.fileName()).exists()) {
    return;
  }

  settings.beginGroup("Canvas");
  { settings.setValue("ZoomAmount", 0.25); }
  settings.endGroup();

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
    settings.setValue("Height", 1122.67);
    settings.setValue("Width", 793.33);
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

  if (!settings.contains("Canvas/ZoomAmount")
      || !settings.contains("Increments/Translate/SmallAmount")
      || !settings.contains("Increments/Translate/LargeAmount")
      || !settings.contains("Increments/Rotate/SmallAmount")
      || !settings.contains("Increments/Rotate/LargeAmount")
      || !settings.contains("Increments/Scale/SmallAmount")
      || !settings.contains("Increments/Scale/LargeAmount")
      || !settings.contains("BoundingBox/Scale")
      || !settings.contains("BoundingBox/Height")
      || !settings.contains("BoundingBox/Width")
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
      || !settings.contains("Keybindings/Rotate/AntiClockwise/LargeKey")) {
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
