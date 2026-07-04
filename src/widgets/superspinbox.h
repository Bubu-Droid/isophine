#ifndef SUPERSPINBOX_H
#define SUPERSPINBOX_H

#include <QDoubleSpinBox>
#include <QEvent>
#include <QWheelEvent>

class SuperSpinBox : public QDoubleSpinBox {
  Q_OBJECT

public:
  SuperSpinBox(QWidget *parent = nullptr);

protected:
  bool eventFilter(QObject *watched, QEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  bool m_isDragging;
  int m_lastMouseX;
};
#endif // SUPERSPINBOX_H