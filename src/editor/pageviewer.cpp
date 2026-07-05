#define KEY_EVENT_RETURN \
  update(); \
  emit pageTransformChangedByKey(); \
  keyEvent->accept(); \
  return
// TODO: too lazy to fix this rn, will fix later
// also, update + return = upturn :) gonna use this

#include "pageviewer.h"

#include <qnamespace.h>
#include <qpdfdocument.h>

#include <QGuiApplication>
#include <QPainter>
#include <QPainterStateGuard>
#include <QPdfDocument>
#include <QPen>
#include <QPixmap>
#include <QSizeF>
#include <QString>
#include <print>
#include <vector>

#include "project_settings.h"

// add initializers here
PageViewer::PageViewer(QWidget* parent) :
    QWidget(parent),
    m_ppiX(QGuiApplication::primaryScreen()->physicalDotsPerInchX()),
    m_ppiY(QGuiApplication::primaryScreen()->physicalDotsPerInchY()) {
  ProjectSettings::instance().pdfDocument = new QPdfDocument(this);

  ProjectSettings::instance().boundBoxHeight = 842 * (m_ppiX / 72.0f);
  ProjectSettings::instance().boundBoxWidth = 595 * (m_ppiY / 72.0f);

  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  pdfDocument->load(ProjectSettings::instance().pdfPath);

  ProjectSettings::instance().pageTransformVector.resize(
      pdfDocument->pageCount()
  );
  m_pageCount = pdfDocument->pageCount();
  loadPage();
}

void PageViewer::loadPage() {
  QPdfDocument* pdfDocument = ProjectSettings::instance().pdfDocument;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

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
  int& currentPageNo = ProjectSettings::instance().currentPageNo;
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;

  int& moveSmallAmount = ProjectSettings::instance().moveSmallAmount;
  int& moveLargeAmount = ProjectSettings::instance().moveLargeAmount;
  float& rotateSmallAmount = ProjectSettings::instance().rotateSmallAmount;
  float& rotateLargeAmount = ProjectSettings::instance().rotateLargeAmount;
  float& scalePageSmallAmount =
      ProjectSettings::instance().scalePageSmallAmount;
  float& scalePageLargeAmount =
      ProjectSettings::instance().scalePageLargeAmount;

  Qt::Key& nextKey = ProjectSettings::instance().nextKey;
  Qt::Key& prevKey = ProjectSettings::instance().prevKey;

  Qt::Key& scalePageUpSmallKey =
      ProjectSettings::instance().scalePageUpSmallKey;
  Qt::Key& scalePageUpLargeKey =
      ProjectSettings::instance().scalePageUpLargeKey;
  Qt::Key& scalePageDownSmallKey =
      ProjectSettings::instance().scalePageDownSmallKey;
  Qt::Key& scalePageDownLargeKey =
      ProjectSettings::instance().scalePageDownLargeKey;

  char& leftSmallKey = ProjectSettings::instance().leftSmallKey;
  char& leftLargeKey = ProjectSettings::instance().leftLargeKey;
  char& downSmallKey = ProjectSettings::instance().downSmallKey;
  char& downLargeKey = ProjectSettings::instance().downLargeKey;
  char& upSmallKey = ProjectSettings::instance().upSmallKey;
  char& upLargeKey = ProjectSettings::instance().upLargeKey;
  char& rightSmallKey = ProjectSettings::instance().rightSmallKey;
  char& rightLargeKey = ProjectSettings::instance().rightLargeKey;

  char& rotateClockSmallKey = ProjectSettings::instance().rotateClockSmallKey;
  char& rotateClockLargeKey = ProjectSettings::instance().rotateClockLargeKey;
  char& rotateAntiClockSmallKey =
      ProjectSettings::instance().rotateAntiClockSmallKey;
  char& rotateAntiClockLargeKey =
      ProjectSettings::instance().rotateAntiClockLargeKey;

  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    // I know, I know, I'm probably violating DRY.
    // How do I make this nicer, suggestions?
    // I hate switch cases, btw.
    // TODO: handle the edge cases where i'm trying to go more left than what the sliderbox is bounded by
    // fix the same for scaling
    if (keyEvent->key() == nextKey) {
      if (currentPageNo == m_pageCount - 1) {
      } else {
        currentPageNo++;
        loadPage();
        update();
        emit pageTransformChangedByKey();
      }
      keyEvent->accept();
      return;
    } else if (keyEvent->key() == prevKey) {
      if (currentPageNo == 0) {
      } else {
        currentPageNo--;
        loadPage();
        update();
        emit pageTransformChangedByKey();
      }
      keyEvent->accept();
      return;
    } else if (keyEvent->text() == leftSmallKey) {
      pageTransformVector[currentPageNo].xOffset -= moveSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == leftLargeKey) {
      pageTransformVector[currentPageNo].xOffset -= moveLargeAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == downSmallKey) {
      pageTransformVector[currentPageNo].yOffset += moveSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == downLargeKey) {
      pageTransformVector[currentPageNo].yOffset += moveLargeAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == upSmallKey) {
      pageTransformVector[currentPageNo].yOffset -= moveSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == upLargeKey) {
      pageTransformVector[currentPageNo].yOffset -= moveLargeAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == rightSmallKey) {
      pageTransformVector[currentPageNo].xOffset += moveSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == rightLargeKey) {
      pageTransformVector[currentPageNo].xOffset += moveLargeAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == rotateClockSmallKey) {
      // TODO: use mod 360 to simplify this
      pageTransformVector[currentPageNo].rotationAmount += rotateSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == rotateClockLargeKey) {
      pageTransformVector[currentPageNo].rotationAmount += rotateLargeAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == rotateAntiClockSmallKey) {
      pageTransformVector[currentPageNo].rotationAmount -= rotateSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->text() == rotateAntiClockLargeKey) {
      pageTransformVector[currentPageNo].rotationAmount -= rotateLargeAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->key() == scalePageUpSmallKey) {
      pageTransformVector[currentPageNo].scaleAmount += scalePageSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->key() == scalePageUpLargeKey) {
      pageTransformVector[currentPageNo].scaleAmount += scalePageLargeAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->key() == scalePageDownSmallKey) {
      pageTransformVector[currentPageNo].scaleAmount -= scalePageSmallAmount;
      KEY_EVENT_RETURN;
    } else if (keyEvent->key() == scalePageDownLargeKey) {
      pageTransformVector[currentPageNo].scaleAmount -= scalePageLargeAmount;
      KEY_EVENT_RETURN;
    }
  }

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

  int& currentPageNo = ProjectSettings::instance().currentPageNo;
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;

  PageTransform& currentPageTransform = pageTransformVector[currentPageNo];

  static const float viewerPaintDimension = 1200.0f;
  float centerX = width() / 2.0f;
  float centerY = height() / 2.0f;
  float side = qMin(width(), height());
  float painterScale = (side) * (boundBoxScale / viewerPaintDimension);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.translate(centerX, centerY);
  painter.scale(painterScale, painterScale);

  float imgStartX = (-m_currentImage.width() / 2.0f);
  float imgStartY = (-m_currentImage.height() / 2.0f);

  // NOTE: What a cute fact I just figured out!
  // If you put the rotate statement before translate, then the axis of the
  // picture along which it is translated is also rotated!
  // Thus, the ordering of the statements is ABSOLUTELY CRITICAL!
  {
    QPainterStateGuard guard(&painter);
    painter.translate(
        currentPageTransform.xOffset,
        currentPageTransform.yOffset
    );
    painter.scale(
        currentPageTransform.scaleAmount,
        currentPageTransform.scaleAmount
    );
    painter.rotate(currentPageTransform.rotationAmount);
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
  boundBoxBorderPen.setWidth(1);
  boundBoxBorderPen.setCosmetic(true);

  QPen boundBoxGridPen(palette().color(QPalette::Accent));
  boundBoxGridPen.setWidth(1);
  boundBoxGridPen.setCosmetic(true);

  painter.setBrush(Qt::NoBrush);

  painter.setPen(boundBoxBorderPen);
  painter.drawPolygon(boundBox, 4);

  painter.setPen(boundBoxGridPen);

  float boundBoxHorLinesGap = boundBoxHeight / (boundBoxHorLinesCount + 1);
  float boundBoxVerLinesGap = boundBoxWidth / (boundBoxVerLinesCount + 1);

  for (int i = 0; i < boundBoxHorLinesCount; ++i) {
    QPoint leftDrawPoint =
        boundBoxTopLeftCorner - QPoint(0, boundBoxHorLinesGap * (i + 1));
    QPoint rightDrawPoint =
        boundBoxTopRightCorner - QPoint(0, boundBoxHorLinesGap * (i + 1));
    painter.drawLine(leftDrawPoint, rightDrawPoint);
  }

  for (int i = 0; i < boundBoxVerLinesCount; ++i) {
    QPoint topDrawPoint =
        boundBoxTopLeftCorner + QPoint(boundBoxVerLinesGap * (i + 1), 0);
    QPoint bottomDrawPoint =
        boundBoxBottomLeftCorner + QPoint(boundBoxVerLinesGap * (i + 1), 0);
    painter.drawLine(topDrawPoint, bottomDrawPoint);
  }
}
