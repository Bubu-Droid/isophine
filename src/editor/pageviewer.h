#pragma once

#include <QKeyEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QWidget>

class PageViewer: public QWidget {
  Q_OBJECT

public:
  explicit PageViewer(QWidget* parent = nullptr);
  void loadPage();
  void setPageCount(int pageCount);

signals:
  void pageTransformChanged();

  void xOffsetChangedByKey();
  void yOffsetChangedByKey();
  void scaleChangedByKey();
  void rotationChangedByKey();

protected:
  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

private:
  int m_pageCount;
  QPixmap m_currentImage;
  qreal m_ppiX;
  qreal m_ppiY;
};
