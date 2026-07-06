#pragma once

#include <QKeyEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPixmap>
#include <QWidget>

class PageViewer: public QWidget {
  Q_OBJECT

public:
  explicit PageViewer(QWidget* parent = nullptr);
  void loadPage();

signals:
  void pageTransformChangedByKey();

protected:
  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

private:
  int m_pageCount;
  QPixmap m_currentImage;
  qreal m_ppiX;
  qreal m_ppiY;
};
