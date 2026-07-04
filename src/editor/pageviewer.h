#ifndef PAGEVIEWER_H
#define PAGEVIEWER_H

#include <QKeyEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QWidget>

class PageViewer: public QWidget {
  Q_OBJECT
public:
  PageViewer(QWidget* parent = nullptr);

protected:
  void paintEvent(QPaintEvent* event) override;
  void keyPressEvent(QKeyEvent* event) override;

private:
  void loadPage(int pageNumber);
  QPixmap m_currentImage;
  qreal m_ppiX;
  qreal m_ppiY;
};

#endif // PAGEVIEWER_H
