#ifndef ISOPHINE_EDITOR_H
#define ISOPHINE_EDITOR_H

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

  void on_xOffsetDoubleSpinBox_editingFinished();

  void on_yOffsetDoubleSpinBox_editingFinished();

  void on_scaleDoubleSpinBox_editingFinished();

  void on_rotationDoubleSpinBox_editingFinished();

  void on_actionZoom_In_triggered();

  void on_actionZoom_Out_triggered();

  void on_actionToggleThumbnail_triggered(bool checked);

private:
  Ui::IsophineEditor* ui;
};
#endif // ISOPHINE_EDITOR_H
