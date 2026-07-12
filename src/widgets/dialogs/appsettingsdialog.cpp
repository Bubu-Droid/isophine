#include "appsettingsdialog.h"

#include <QLineEdit>
#include <QSettings>

#include "buttondialogs.h"
#include "ui_appsettingsdialog.h"

AppSettingsDialog::AppSettingsDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsDialog) {
  ui->setupUi(this);
  this->setWindowTitle("Edit App Settings");

  QSettings settings;

  ui->transSmallSpinBox->setValue(
      settings.value("Increments/Translate/SmallAmount").toDouble()
  );
  ui->transLargeSpinBox->setValue(
      settings.value("Increments/Translate/LargeAmount").toDouble()
  );
  ui->rotateSmallSpinBox->setValue(
      settings.value("Increments/Rotate/SmallAmount").toDouble()
  );
  ui->rotateLargeSpinBox->setValue(
      settings.value("Increments/Rotate/LargeAmount").toDouble()
  );
  ui->scaleSmallSpinBox->setValue(
      settings.value("Increments/Scale/SmallAmount").toDouble()
  );
  ui->scaleLargeSpinBox->setValue(
      settings.value("Increments/Scale/LargeAmount").toDouble()
  );

  ui->boxScaleSpinBox->setValue(settings.value("BoundingBox/Scale").toDouble());
  m_gridLineColor = settings.value("BoundingBox/LineColor").value<QColor>();
  ui->colorPushButton->setText(m_gridLineColor.name());
  // the brainrot is really getting to me, ngl
  // insert horse, in formal suit, sitting on rock and pondering pal meme
  QPalette pal = ui->colorPushButton->palette();
  pal.setColor(QPalette::ButtonText, m_gridLineColor);
  ui->colorPushButton->setAutoFillBackground(true);
  ui->colorPushButton->setPalette(pal);
  ui->colorPushButton->update();
  ui->horLinesSpinBox->setValue(
      settings.value("BoundingBox/HorizontalLinesCount").toInt()
  );
  ui->verLinesSpinBox->setValue(
      settings.value("BoundingBox/VerticalLinesCount").toInt()
  );

  ui->nextPageKey->setKeySequence(
      settings.value("Keybindings/Navigation/NextKey").value<QKeySequence>()
  );
  ui->prevPageKey->setKeySequence(QKeySequence(
      settings.value("Keybindings/Navigation/PrevKey").value<QKeySequence>()
  ));

  ui->scaleUpSmallKey->setKeySequence(
      settings.value("Keybindings/ScalePage/Up/SmallKey").value<QKeySequence>()
  );
  ui->scaleUpLargeKey->setKeySequence(
      settings.value("Keybindings/ScalePage/Up/LargeKey").value<QKeySequence>()
  );
  ui->scaleDownSmallKey->setKeySequence(
      settings.value("Keybindings/ScalePage/Down/SmallKey")
          .value<QKeySequence>()
  );
  ui->scaleDownLargeKey->setKeySequence(
      settings.value("Keybindings/ScalePage/Down/LargeKey")
          .value<QKeySequence>()
  );
  ui->moveLeftSmallKey->setKeySequence(
      settings.value("Keybindings/Movement/Left/SmallKey").value<QKeySequence>()
  );
  ui->moveLeftLargeKey->setKeySequence(
      settings.value("Keybindings/Movement/Left/LargeKey").value<QKeySequence>()
  );
  ui->moveRightSmallKey->setKeySequence(
      settings.value("Keybindings/Movement/Right/SmallKey")
          .value<QKeySequence>()
  );
  ui->moveRightLargeKey->setKeySequence(
      settings.value("Keybindings/Movement/Right/LargeKey")
          .value<QKeySequence>()
  );
  ui->moveUpSmallKey->setKeySequence(
      settings.value("Keybindings/Movement/Up/SmallKey").value<QKeySequence>()
  );
  ui->moveUpLargeKey->setKeySequence(
      settings.value("Keybindings/Movement/Up/LargeKey").value<QKeySequence>()
  );
  ui->moveDownSmallKey->setKeySequence(
      settings.value("Keybindings/Movement/Down/SmallKey").value<QKeySequence>()
  );
  ui->moveDownLargeKey->setKeySequence(
      settings.value("Keybindings/Movement/Down/LargeKey").value<QKeySequence>()
  );
  ui->rotateClockSmallKey->setKeySequence(
      settings.value("Keybindings/Rotate/Clockwise/SmallKey")
          .value<QKeySequence>()
  );
  ui->rotateClockLargeKey->setKeySequence(
      settings.value("Keybindings/Rotate/Clockwise/LargeKey")
          .value<QKeySequence>()
  );
  ui->rotateAntiClockSmallKey->setKeySequence(
      settings.value("Keybindings/Rotate/AntiClockwise/SmallKey")
          .value<QKeySequence>()
  );
  ui->rotateAntiClockLargeKey->setKeySequence(
      settings.value("Keybindings/Rotate/AntiClockwise/LargeKey")
          .value<QKeySequence>()
  );
}

AppSettingsDialog::~AppSettingsDialog() {
  delete ui;
}

void AppSettingsDialog::on_colorPushButton_clicked() {
  SelectGridlineColor dialog = SelectGridlineColor(this);
  if (dialog.exec() == QDialog::Accepted) {
    m_gridLineColor = dialog.selectedColor();
    ui->colorPushButton->setText(m_gridLineColor.name());
    QPalette pal = ui->colorPushButton->palette();
    pal.setColor(QPalette::ButtonText, m_gridLineColor);
    ui->colorPushButton->setAutoFillBackground(true);
    ui->colorPushButton->setPalette(pal);
    ui->colorPushButton->update();
  }
}

void AppSettingsDialog::updateSettings() {
  QSettings settings;

  settings.beginGroup("Increments");
  {
    settings.beginGroup("Translate");
    settings.setValue("SmallAmount", ui->transSmallSpinBox->value());
    settings.setValue("LargeAmount", ui->transLargeSpinBox->value());
    settings.endGroup();

    settings.beginGroup("Rotate");
    settings.setValue("SmallAmount", ui->rotateSmallSpinBox->value());
    settings.setValue("LargeAmount", ui->rotateLargeSpinBox->value());
    settings.endGroup();

    settings.beginGroup("Scale");
    settings.setValue("SmallAmount", ui->scaleSmallSpinBox->value());
    settings.setValue("LargeAmount", ui->scaleLargeSpinBox->value());
    settings.endGroup();
  }
  settings.endGroup();

  settings.beginGroup("BoundingBox");
  {
    settings.setValue("Scale", ui->boxScaleSpinBox->value());
    settings.setValue("LineColor", m_gridLineColor);
    settings.setValue("HorizontalLinesCount", ui->horLinesSpinBox->value());
    settings.setValue("VerticalLinesCount", ui->verLinesSpinBox->value());
  }
  settings.endGroup();

  settings.beginGroup("Keybindings");
  {
    settings.beginGroup("Navigation");
    settings.setValue("NextKey", ui->nextPageKey->keySequence());
    settings.setValue("PrevKey", ui->prevPageKey->keySequence());
    settings.endGroup();

    settings.beginGroup("ScalePage");
    {
      settings.beginGroup("Up");
      settings.setValue("SmallKey", ui->scaleUpSmallKey->keySequence());
      settings.setValue("LargeKey", ui->scaleUpLargeKey->keySequence());
      settings.endGroup();

      settings.beginGroup("Down");
      settings.setValue("SmallKey", ui->scaleDownSmallKey->keySequence());
      settings.setValue("LargeKey", ui->scaleDownLargeKey->keySequence());
      settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Movement");
    {
      settings.beginGroup("Left");
      settings.setValue("SmallKey", ui->moveLeftSmallKey->keySequence());
      settings.setValue("LargeKey", ui->moveLeftLargeKey->keySequence());
      settings.endGroup();

      settings.beginGroup("Down");
      settings.setValue("SmallKey", ui->moveDownSmallKey->keySequence());
      settings.setValue("LargeKey", ui->moveDownLargeKey->keySequence());
      settings.endGroup();

      settings.beginGroup("Up");
      settings.setValue("SmallKey", ui->moveUpSmallKey->keySequence());
      settings.setValue("LargeKey", ui->moveUpLargeKey->keySequence());
      settings.endGroup();

      settings.beginGroup("Right");
      settings.setValue("SmallKey", ui->moveRightSmallKey->keySequence());
      settings.setValue("LargeKey", ui->moveRightLargeKey->keySequence());
      settings.endGroup();
    }
    settings.endGroup();

    settings.beginGroup("Rotate");
    {
      settings.beginGroup("Clockwise");
      settings.setValue("SmallKey", ui->rotateClockSmallKey->keySequence());
      settings.setValue("LargeKey", ui->rotateClockLargeKey->keySequence());
      settings.endGroup();

      settings.beginGroup("AntiClockwise");
      settings.setValue("SmallKey", ui->rotateAntiClockSmallKey->keySequence());
      settings.setValue("LargeKey", ui->rotateAntiClockLargeKey->keySequence());
      settings.endGroup();
    }
    settings.endGroup();
  }
  settings.endGroup();
}
