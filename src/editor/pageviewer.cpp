#include "pageviewer.h"

#include <qnamespace.h>

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
    m_ppiY(QGuiApplication::primaryScreen()->physicalDotsPerInchY()) {}

void PageViewer::setPageCount(int pageCount) {
  if (m_pageCount != pageCount) {
    m_pageCount = pageCount;
  }
}

void PageViewer::loadPage() {
  QPdfDocument*& pdfDocument = ProjectSettings::instance().pdfDocument;
  int& currentPageNo = ProjectSettings::instance().currentPageNo;

  m_currentPageSize = QSize(
      qRound(
          pdfDocument->pagePointSize(currentPageNo).width() * (m_ppiX / 72.0)
      ),
      qRound(
          pdfDocument->pagePointSize(currentPageNo).height() * (m_ppiY / 72.0)
      )
  );
  m_currentImage =
      QPixmap::fromImage(pdfDocument->render(currentPageNo, m_currentPageSize));
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

  QKeySequence& nextKey = ProjectSettings::instance().nextKey;
  QKeySequence& prevKey = ProjectSettings::instance().prevKey;

  QKeySequence& scalePageUpSmallKey =
      ProjectSettings::instance().scalePageUpSmallKey;
  QKeySequence& scalePageUpLargeKey =
      ProjectSettings::instance().scalePageUpLargeKey;
  QKeySequence& scalePageDownSmallKey =
      ProjectSettings::instance().scalePageDownSmallKey;
  QKeySequence& scalePageDownLargeKey =
      ProjectSettings::instance().scalePageDownLargeKey;

  QKeySequence& leftSmallKey = ProjectSettings::instance().leftSmallKey;
  QKeySequence& leftLargeKey = ProjectSettings::instance().leftLargeKey;
  QKeySequence& downSmallKey = ProjectSettings::instance().downSmallKey;
  QKeySequence& downLargeKey = ProjectSettings::instance().downLargeKey;
  QKeySequence& upSmallKey = ProjectSettings::instance().upSmallKey;
  QKeySequence& upLargeKey = ProjectSettings::instance().upLargeKey;
  QKeySequence& rightSmallKey = ProjectSettings::instance().rightSmallKey;
  QKeySequence& rightLargeKey = ProjectSettings::instance().rightLargeKey;

  QKeySequence& rotateClockSmallKey =
      ProjectSettings::instance().rotateClockSmallKey;
  QKeySequence& rotateClockLargeKey =
      ProjectSettings::instance().rotateClockLargeKey;
  QKeySequence& rotateAntiClockSmallKey =
      ProjectSettings::instance().rotateAntiClockSmallKey;
  QKeySequence& rotateAntiClockLargeKey =
      ProjectSettings::instance().rotateAntiClockLargeKey;

  // What a huge heartbreak I had when I realized
  // that we have something called QKeySequence and that I now have to
  // re-write the logic of appsettingsdialog + keyPressEvent.
  //
  // Woah, it wasn't really that difficult. All I needed was a bunch of
  // vim substitutes and boom, I was done!
  if (event->type() == QEvent::KeyPress) {
    QKeyCombination keyCombination =
        static_cast<QKeyEvent*>(event)->keyCombination();
    QKeySequence keySequence =
        QKeySequence(keyCombination.keyboardModifiers() | keyCombination.key());

    // I know, I know, I'm probably violating the DRY principle.
    // How do I make this nicer, suggestions?
    // I hate switch cases, btw. Maybe I can use templates or
    // maybe write a throwaway function.
    // Well, I'm too lazy to fix this rn, will fix this sometime later.
    if (keySequence == nextKey) {
      if (currentPageNo < m_pageCount - 1) {
        ++currentPageNo;
        loadPage();
        emit pageTransformChanged();
      }
      return;
    } else if (keySequence == prevKey) {
      if (currentPageNo > 0) {
        --currentPageNo;
        loadPage();
        emit pageTransformChanged();
      }
      return;
    } else if (keySequence == leftSmallKey) {
      if (currentPageTransform.xOffset - moveSmallAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.xOffset -= moveSmallAmount;
      }
      emit xOffsetChangedByKey();
      return;
    } else if (keySequence == leftLargeKey) {
      if (currentPageTransform.xOffset - moveLargeAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.xOffset -= moveLargeAmount;
      }
      emit xOffsetChangedByKey();
      return;
    } else if (keySequence == downSmallKey) {
      if (currentPageTransform.yOffset + moveSmallAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.yOffset += moveSmallAmount;
      }
      emit yOffsetChangedByKey();
      return;
    } else if (keySequence == downLargeKey) {
      if (currentPageTransform.yOffset + moveLargeAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.yOffset += moveLargeAmount;
      }
      emit yOffsetChangedByKey();
      return;
    } else if (keySequence == upSmallKey) {
      if (currentPageTransform.yOffset - moveSmallAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.yOffset -= moveSmallAmount;
      }
      emit yOffsetChangedByKey();
      return;
    } else if (keySequence == upLargeKey) {
      if (currentPageTransform.yOffset - moveLargeAmount >= -MAX_OFFSET_MAG) {
        currentPageTransform.yOffset -= moveLargeAmount;
      }
      emit yOffsetChangedByKey();
      return;
    } else if (keySequence == rightSmallKey) {
      if (currentPageTransform.xOffset + moveSmallAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.xOffset += moveSmallAmount;
      }
      emit xOffsetChangedByKey();
      return;
    } else if (keySequence == rightLargeKey) {
      if (currentPageTransform.xOffset + moveLargeAmount <= MAX_OFFSET_MAG) {
        currentPageTransform.xOffset += moveLargeAmount;
      }
      emit xOffsetChangedByKey();
      return;
    } else if (keySequence == rotateClockSmallKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount + rotateSmallAmount,
          360.0f
      );
      emit rotationChangedByKey();
      return;
    } else if (keySequence == rotateClockLargeKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount + rotateLargeAmount,
          360.0f
      );
      emit rotationChangedByKey();
      return;
    } else if (keySequence == rotateAntiClockSmallKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount - rotateSmallAmount,
          360.0f
      );
      emit rotationChangedByKey();
      return;
    } else if (keySequence == rotateAntiClockLargeKey) {
      currentPageTransform.rotationAmount = std::fmod(
          currentPageTransform.rotationAmount - rotateLargeAmount,
          360.0f
      );
      emit rotationChangedByKey();
      return;
    } else if (keySequence == scalePageUpSmallKey) {
      if (currentPageTransform.scaleAmount + scalePageSmallAmount
          <= MAX_SCALE) {
        currentPageTransform.scaleAmount += scalePageSmallAmount;
      }
      emit scaleChangedByKey();
      return;
    } else if (keySequence == scalePageUpLargeKey) {
      if (currentPageTransform.scaleAmount + scalePageLargeAmount
          <= MAX_SCALE) {
        currentPageTransform.scaleAmount += scalePageLargeAmount;
      }
      emit scaleChangedByKey();
      return;
    } else if (keySequence == scalePageDownSmallKey) {
      if (currentPageTransform.scaleAmount - scalePageSmallAmount
          >= MIN_SCALE) {
        currentPageTransform.scaleAmount -= scalePageSmallAmount;
      }
      emit scaleChangedByKey();
      return;
    } else if (keySequence == scalePageDownLargeKey) {
      if (currentPageTransform.scaleAmount - scalePageLargeAmount
          >= MIN_SCALE) {
        currentPageTransform.scaleAmount -= scalePageLargeAmount;
      }
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

  QBrush bgBrush(QColor(255, 255, 255));
  QPainter painter(this);
  painter.setBackgroundMode(Qt::OpaqueMode);
  painter.setBackground(bgBrush);
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
    painter.fillRect(
        imgStartX,
        imgStartY,
        m_currentPageSize.width(),
        m_currentPageSize.height(),
        Qt::white
    );
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

  QPen boundBoxBorderPen(ProjectSettings::instance().boundBoxLineColor);
  boundBoxBorderPen.setWidth(1);
  boundBoxBorderPen.setCosmetic(true);

  QPen boundBoxGridPen(ProjectSettings::instance().boundBoxLineColor);
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
