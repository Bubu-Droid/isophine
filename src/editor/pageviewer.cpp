#include "pageviewer.h"

#include <qpdfdocument.h>

#include <QGuiApplication>
#include <QPainter>
#include <QPainterStateGuard>
#include <QPdfDocument>
#include <QPen>
#include <QPixmap>
#include <QSizeF>
#include <QString>

#include "project_settings.h"

// add initializers here
PageViewer::PageViewer(QWidget* parent) :
    QWidget(parent),
    m_ppiX(QGuiApplication::primaryScreen()->physicalDotsPerInchX()),
    m_ppiY(QGuiApplication::primaryScreen()->physicalDotsPerInchY()) {
  loadPage(ProjectSettings::instance().currentPageNo);
}

void PageViewer::loadPage(int pageNumber) {
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  QPdfDocument* pdfDocument = ProjectSettings::instance().pdfDocument;

  // holy cow, this is so abstracted that even I'm being confused looking
  // at what I've written -- not that I don't like it >:))
  QSize currentPageSize =
      QSizeF(
          pdfDocument->pagePointSize(currentPageNo).width() * (m_ppiX / 72.0f),
          pdfDocument->pagePointSize(currentPageNo).height() * (m_ppiY / 72.0f)
      )
          .toSize();

  m_currentImage =
      QPixmap::fromImage(pdfDocument->render(currentPageNo, currentPageSize));
}

void PageViewer::keyPressEvent(QKeyEvent* event) {
  // if (event->)
  QWidget::keyPressEvent(event);
}

void PageViewer::paintEvent(QPaintEvent* event) {
  float& boundBoxHeight = ProjectSettings::instance().boundBoxHeight;
  float& boundBoxWidth = ProjectSettings::instance().boundBoxWidth;
  float boundBoxHalfWidth = boundBoxWidth / 2.0f;
  float boundBoxHalfHeight = boundBoxHeight / 2.0f;
  float& boundBoxScale = ProjectSettings::instance().boundBoxScale;
  int& boundBoxHorLinesCount =
      ProjectSettings::instance().boundBoxHorLinesCount;
  int& boundBoxVerLinesCount =
      ProjectSettings::instance().boundBoxVerLinesCount;

  static const float viewerPaintDimension = 2000.0f;
  float centerX = width() / 2.0f;
  float centerY = height() / 2.0f;
  float side = qMin(width(), height());
  float painterScale = (side) * (boundBoxScale / viewerPaintDimension);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.translate(centerX, centerY);
  painter.scale(painterScale, painterScale);

  float imgStartX = -m_currentImage.width() / 2.0f;
  float imgStartY = -m_currentImage.height() / 2.0f;

  {
    QPainterStateGuard guard(&painter);
    painter.rotate(30.0);
    painter.drawPixmap(imgStartX, imgStartY, m_currentImage);
  }

  QPoint boundBoxTopLeftCorner = QPoint(-boundBoxHalfWidth, boundBoxHalfHeight);
  QPoint boundBoxBottomLeftCorner =
      QPoint(-boundBoxHalfWidth, -boundBoxHalfHeight);
  QPoint boundBoxBottomRightCorner =
      QPoint(boundBoxHalfWidth, -boundBoxHalfHeight);
  QPoint boundBoxTopRightCorner = QPoint(boundBoxHalfWidth, boundBoxHalfHeight);

  QPoint boundBox[4] = {
      boundBoxTopLeftCorner,
      boundBoxBottomLeftCorner,
      boundBoxBottomRightCorner,
      boundBoxTopRightCorner
  };

  QPen boundBoxBorderPen(palette().color(QPalette::Accent));
  boundBoxBorderPen.setWidth(2);
  boundBoxBorderPen.setCosmetic(true);

  QPen boundBoxGridPen(palette().color(QPalette::Accent));
  boundBoxGridPen.setWidth(1);
  boundBoxGridPen.setCosmetic(true);

  painter.setBrush(Qt::NoBrush);

  painter.setPen(boundBoxBorderPen);
  painter.drawPolygon(boundBox, 4);

  painter.setPen(boundBoxGridPen);

  float boundBoxHorLinesGap = boundBoxHeight / boundBoxHorLinesCount;
  float boundBoxVerLinesGap = boundBoxWidth / boundBoxVerLinesCount;

  for (int i = 0; i < boundBoxHorLinesCount - 1; ++i) {
    QPoint leftDrawPoint =
        boundBoxTopLeftCorner - QPoint(0, boundBoxHorLinesGap * (i + 1));
    QPoint rightDrawPoint =
        boundBoxTopRightCorner - QPoint(0, boundBoxHorLinesGap * (i + 1));
    painter.drawLine(leftDrawPoint, rightDrawPoint);
  }

  for (int i = 0; i < boundBoxVerLinesCount - 1; ++i) {
    QPoint topDrawPoint =
        boundBoxTopLeftCorner + QPoint(boundBoxVerLinesGap * (i + 1), 0);
    QPoint bottomDrawPoint =
        boundBoxBottomLeftCorner + QPoint(boundBoxVerLinesGap * (i + 1), 0);
    painter.drawLine(topDrawPoint, bottomDrawPoint);
  }
}
