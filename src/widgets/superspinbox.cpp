#include "superspinbox.h"

#include <QMouseEvent>
#include <QLineEdit>

SuperSpinBox::SuperSpinBox(QWidget *parent) : QDoubleSpinBox(parent), m_isDragging(false), m_lastMouseX(0) {
  setFocusPolicy(Qt::NoFocus);
  setButtonSymbols(QAbstractSpinBox::NoButtons);
  setCursor(Qt::SizeHorCursor);

  lineEdit()->setCursor(Qt::SizeHorCursor);
  lineEdit()->installEventFilter(this);
}

void SuperSpinBox::wheelEvent(QWheelEvent *event) {
  event->accept();
}

bool SuperSpinBox::eventFilter(QObject *watched, QEvent *event) {
  if (watched == lineEdit()) {
    if (event->type() == QEvent::MouseButtonDblClick) {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton) {
        setFocus(Qt::MouseFocusReason);
        return false;
      }
    }
    else if (event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMouseX = mouseEvent->globalPosition().x();
        return true;
      }
    } else if (event->type() == QEvent::MouseMove) {
      if (m_isDragging) {
        QMouseEvent *mouseEvent  = static_cast<QMouseEvent*>(event);
        int currentX = mouseEvent->globalPosition().x();
        int deltaX = currentX - m_lastMouseX;
        if (deltaX != 0) {
          setValue(value() + (deltaX * singleStep()));
          m_lastMouseX = currentX;
        }

        return true;
      }
    } else if (event->type() == QEvent::MouseButtonRelease) {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton && m_isDragging) {
        m_isDragging = false;
        return true;
      }
    }
  }

  return QDoubleSpinBox::eventFilter(watched, event);
}