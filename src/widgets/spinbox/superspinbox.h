#pragma once

#include <QDoubleSpinBox>
#include <QEvent>
#include <QFocusEvent>
#include <QWheelEvent>

class SuperSpinBox: public QDoubleSpinBox {
  Q_OBJECT

public:
  explicit SuperSpinBox(QWidget* parent = nullptr);

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void focusOutEvent(QFocusEvent* event) override;

private:
  bool m_isDragging;
  int m_lastMouseX;
};
