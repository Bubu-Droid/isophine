#pragma once

#include <QFile>
#include <QMainWindow>

QT_BEGIN_NAMESPACE

namespace Ui {
class IsophineEditor;
}

QT_END_NAMESPACE

class IsophineEditor: public QMainWindow {
  Q_OBJECT

public:
  explicit IsophineEditor(QWidget* parent = nullptr);
  ~IsophineEditor() override;

private slots:

  void on_actionQuit_triggered();

  void on_actionToggleInspector_triggered(bool checked);
  void on_actionToggleThumbnail_triggered(bool checked);

  void on_thumbnailView_activated(const QModelIndex& index);

  void on_xOffsetDoubleSpinBox_editingFinished();
  void on_xOffsetDoubleSpinBox_valueChanged(double arg1);
  void on_yOffsetDoubleSpinBox_editingFinished();
  void on_yOffsetDoubleSpinBox_valueChanged(double arg1);
  void on_scaleDoubleSpinBox_editingFinished();
  void on_scaleDoubleSpinBox_valueChanged(double arg1);
  void on_rotationDoubleSpinBox_editingFinished();
  void on_rotationDoubleSpinBox_valueChanged(double arg1);

  void on_actionZoom_In_triggered();
  void on_actionZoom_Out_triggered();

  void on_pageViewWidget_xOffsetChangedByKey();
  void on_pageViewWidget_yOffsetChangedByKey();
  void on_pageViewWidget_scaleChangedByKey();
  void on_pageViewWidget_rotationChangedByKey();

  void on_pageViewWidget_pageTransformChanged();

  void on_actionHome_triggered();
  void on_actionSave_triggered();
  void on_actionEditGlobalSettings_triggered();

signals:
  void quitApp();
  void goToHome();
  void updateTitle(const QString& title);
  void refreshSettings();

private:
  Ui::IsophineEditor* ui;
  QFile m_layoutFile;

public:
  void setPageCount(int pageCount);
  void loadThumbnails();
  void loadPage();
};
