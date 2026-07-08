#include "superspinbox.h"

#include <QLineEdit>
#include <QMouseEvent>

SuperSpinBox::SuperSpinBox(QWidget* parent) :
    QDoubleSpinBox(parent),
    m_isDragging(false),
    m_lastMouseX(0) {
  lineEdit()->installEventFilter(this);
}

void SuperSpinBox::wheelEvent(QWheelEvent* event) {
  event->accept();
}

void SuperSpinBox::focusOutEvent(QFocusEvent* event) {
  lineEdit()->deselect();
  QDoubleSpinBox::focusOutEvent(event);
}

bool SuperSpinBox::eventFilter(QObject* object, QEvent* event) {
  if (object == lineEdit()) {
    if (event->type() == QEvent::MouseButtonDblClick) {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton) {
        if (!lineEdit()->hasFocus()) {
          m_isDragging = false;
          lineEdit()->setFocus();
          lineEdit()->selectAll();
          return true;
        }
      }
    } else if (event->type() == QEvent::MouseButtonPress) {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton) {
        if (!lineEdit()->hasFocus()) {
          m_isDragging = true;
          m_lastMouseX = mouseEvent->globalPosition().x();
          return true;
        }
      }
    } else if (event->type() == QEvent::MouseMove) {
      if (m_isDragging) {
        lineEdit()->setCursor(Qt::SizeHorCursor);
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        int currentX = mouseEvent->globalPosition().x();
        int deltaX = currentX - m_lastMouseX;
        if (deltaX != 0) {
          setValue(value() + (deltaX * singleStep()));
          m_lastMouseX = currentX;
        }
        return true;
      }
    } else if (event->type() == QEvent::MouseButtonRelease) {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::LeftButton && m_isDragging) {
        lineEdit()->setCursor(Qt::ArrowCursor);
        m_isDragging = false;
        return true;
      }
    }
  }

  return QDoubleSpinBox::eventFilter(object, event);
}
