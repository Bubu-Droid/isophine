#include "pageviewer.h"

#include <QGuiApplication>
#include <QPainter>
#include <QPainterStateGuard>
#include <QPen>
#include <cmath>

#include "project_settings.h"

PageViewer::PageViewer(QWidget* parent) :
    QWidget(parent),
    m_pageCount(0),
    m_ppiX(QGuiApplication::primaryScreen()->physicalDotsPerInchX()),
    m_ppiY(QGuiApplication::primaryScreen()->physicalDotsPerInchY()) {
  ProjectSettings::instance().pdfDocument = new QPdfDocument(this);

  ProjectSettings::instance().boundBoxHeight = 842 * (m_ppiX / 72.0);
  ProjectSettings::instance().boundBoxWidth = 595 * (m_ppiY / 72.0);

  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  pdfDocument->load(ProjectSettings::instance().pdfPath);
  m_pageCount = pdfDocument->pageCount();

  ProjectSettings::instance().pageTransformVector.resize(
      pdfDocument->pageCount()
  );

  loadPage();
}

void PageViewer::loadPage() {
  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  QSize currentPageSize = QSize(
      qRound(
          pdfDocument->pagePointSize(currentPageNo).width() * (m_ppiX / 72.0)
      ),
      qRound(
          pdfDocument->pagePointSize(currentPageNo).height() * (m_ppiY / 72.0)
      )
  );

  m_currentImage =
      QPixmap::fromImage(pdfDocument->render(currentPageNo, currentPageSize));
}

void PageViewer::keyPressEvent(QKeyEvent* event) {
  int& currentPageNo = ProjectSettings::instance().currentPageNo;
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;
  PageTransform& currentPageTransform =
      ProjectSettings::instance().pageTransformVector[currentPageNo];

  qreal& moveSmallAmount = ProjectSettings::instance().moveSmallAmount;
  qreal& moveLargeAmount = ProjectSettings::instance().moveLargeAmount;
  qreal& rotateSmallAmount = ProjectSettings::instance().rotateSmallAmount;
  qreal& rotateLargeAmount = ProjectSettings::instance().rotateLargeAmount;
  qreal& scalePageSmallAmount =
      ProjectSettings::instance().scalePageSmallAmount;
  qreal& scalePageLargeAmount =
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
    // I know, I know, I'm probably violating the DRY principle.
    // How do I make this nicer, suggestions?
    // I hate switch cases, btw. Maybe I can use templates or
    // maybe write a throwaway function.
    // Well, I'm too lazy to fix this rn, will fix this sometime later.
    if (keyEvent->key() == nextKey) {
      if (currentPageNo < m_pageCount - 1) {
        currentPageNo++;
        loadPage();
        update();
        emit pageTransformChanged();
      }
      return;
    } else if (keyEvent->key() == prevKey) {
      if (currentPageNo > 0) {
        currentPageNo--;
        loadPage();
        update();
        emit pageTransformChanged();
      }
      return;
    } else if (keyEvent->text() == leftSmallKey) {
      if (currentPageTransform.xOffset - moveSmallAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.xOffset -= moveSmallAmount;
      }
      update();
      emit xOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == leftLargeKey) {
      if (currentPageTransform.xOffset - moveLargeAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.xOffset -= moveLargeAmount;
      }
      update();
      emit xOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == downSmallKey) {
      if (currentPageTransform.yOffset + moveSmallAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.yOffset += moveSmallAmount;
      }
      update();
      emit yOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == downLargeKey) {
      if (currentPageTransform.yOffset + moveLargeAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.yOffset += moveLargeAmount;
      }
      update();
      emit yOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == upSmallKey) {
      if (currentPageTransform.yOffset - moveSmallAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.yOffset -= moveSmallAmount;
      }
      update();
      emit yOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == upLargeKey) {
      if (currentPageTransform.yOffset - moveLargeAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.yOffset -= moveLargeAmount;
      }
      update();
      emit yOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == rightSmallKey) {
      if (currentPageTransform.xOffset + moveSmallAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.xOffset += moveSmallAmount;
      }
      update();
      emit xOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == rightLargeKey) {
      if (currentPageTransform.xOffset + moveLargeAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.xOffset += moveLargeAmount;
      }
      update();
      emit xOffsetChangedByKey();
      return;
    } else if (keyEvent->text() == rotateClockSmallKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount + rotateSmallAmount,
          360.0f
      );
      update();
      emit rotationChangedByKey();
      return;
    } else if (keyEvent->text() == rotateClockLargeKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount + rotateLargeAmount,
          360.0f
      );
      update();
      emit rotationChangedByKey();
      return;
    } else if (keyEvent->text() == rotateAntiClockSmallKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount - rotateSmallAmount,
          360.0f
      );
      update();
      emit rotationChangedByKey();
      return;
    } else if (keyEvent->text() == rotateAntiClockLargeKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount - rotateLargeAmount,
          360.0f
      );
      update();
      emit rotationChangedByKey();
      return;
    } else if (keyEvent->key() == scalePageUpSmallKey) {
      if (currentPageTransform.scaleAmount + scalePageSmallAmount
          <= MAX_SCALE) {
        currentPageTransform.scaleAmount += scalePageSmallAmount;
      }
      update();
      emit scaleChangedByKey();
      return;
    } else if (keyEvent->key() == scalePageUpLargeKey) {
      if (currentPageTransform.scaleAmount + scalePageLargeAmount
          <= MAX_SCALE) {
        currentPageTransform.scaleAmount += scalePageLargeAmount;
      }
      update();
      emit scaleChangedByKey();
      return;
    } else if (keyEvent->key() == scalePageDownSmallKey) {
      if (currentPageTransform.scaleAmount - scalePageSmallAmount
          >= MIN_SCALE) {
        currentPageTransform.scaleAmount -= scalePageSmallAmount;
      }
      update();
      emit scaleChangedByKey();
      return;
    } else if (keyEvent->key() == scalePageDownLargeKey) {
      if (currentPageTransform.scaleAmount - scalePageLargeAmount
          >= MIN_SCALE) {
        currentPageTransform.scaleAmount -= scalePageLargeAmount;
      }
      update();
      emit scaleChangedByKey();
      return;
    }
  }

  QWidget::keyPressEvent(event);
}

void PageViewer::paintEvent(QPaintEvent* event) {
  qreal& boundBoxHeight = ProjectSettings::instance().boundBoxHeight;
  qreal& boundBoxWidth = ProjectSettings::instance().boundBoxWidth;
  qreal boundBoxHalfWidth = boundBoxWidth / 2.0;
  qreal boundBoxHalfHeight = boundBoxHeight / 2.0;
  qreal& boundBoxScale = ProjectSettings::instance().boundBoxScale;
  int& boundBoxHorLinesCount =
      ProjectSettings::instance().boundBoxHorLinesCount;
  int& boundBoxVerLinesCount =
      ProjectSettings::instance().boundBoxVerLinesCount;

  int& currentPageNo = ProjectSettings::instance().currentPageNo;
  std::vector<PageTransform>& pageTransformVector =
      ProjectSettings::instance().pageTransformVector;

  PageTransform& currentPageTransform = pageTransformVector[currentPageNo];

  qreal viewerPaintDimension = 1200.0;
  qreal centerX = width() / 2.0;
  qreal centerY = height() / 2.0;
  qreal side = qMin(width(), height());
  qreal painterScale = (side) * (boundBoxScale / viewerPaintDimension);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.translate(centerX, centerY);
  painter.scale(painterScale, painterScale);

  qreal imgStartX = (-m_currentImage.width() / 2.0);
  qreal imgStartY = (-m_currentImage.height() / 2.0);

  // What a cute fact I just figured out!
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

  qreal boundBoxHorLinesGap = boundBoxHeight / (boundBoxHorLinesCount + 1);
  qreal boundBoxVerLinesGap = boundBoxWidth / (boundBoxVerLinesCount + 1);

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
