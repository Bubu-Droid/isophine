#ifndef PAGEVIEWER_H
#define PAGEVIEWER_H

#include <QKeyEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPixmap>
#include <QWidget>

class PageViewer: public QWidget {
  Q_OBJECT
public:
  PageViewer(QWidget* parent = nullptr);
  void loadPage();

signals:
  void pageTransformChangedByKey();

protected:
  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

private:
  int m_pageCount = 0;
  QPixmap m_currentImage;
  qreal m_ppiX;
  qreal m_ppiY;
};

#endif // PAGEVIEWER_H
