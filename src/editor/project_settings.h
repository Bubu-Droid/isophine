#pragma once

#include <QColor>
#include <QKeySequence>
#include <QPdfDocument>
#include <QString>
#include <vector>

inline constexpr qreal MAX_OFFSET_MAG = 500;
// this is for the page
inline constexpr qreal MIN_SCALE = 0.1;
inline constexpr qreal MAX_SCALE = 5;
// this is for the canvas
inline constexpr qreal MIN_ZOOM = 0.25;
inline constexpr qreal MAX_ZOOM = 5;

struct PageTransform {
  qreal xOffset = 0.0;
  qreal yOffset = 0.0;
  qreal rotationAmount = 0.0;
  qreal scaleAmount = 1.0;
  QColor pageBgColor = QColor(255, 255, 255);
};

class ProjectSettings {
private:
  ProjectSettings() {}

public:
  static ProjectSettings& instance() {
    static ProjectSettings uniqueInstance;
    return uniqueInstance;
  }

  QPdfDocument* pdfDocument = nullptr;
  QString projectOutDir;
  QString layoutPath;
  QString projectName;

  qreal boundBoxScale = 1.0;
  qreal boundBoxHeight;
  qreal boundBoxWidth;
  QColor boundBoxLineColor = QColor(255, 96, 208);
  int boundBoxHorLinesCount = 5;
  int boundBoxVerLinesCount = 3;
  int currentPageNo;
  qreal zoomCanvasAmount = 0.25;

  qreal moveSmallAmount = 1.0;
  qreal moveLargeAmount = 10.0;
  qreal rotateSmallAmount = 0.1;
  qreal rotateLargeAmount = 1.0;
  qreal scalePageSmallAmount = 0.01;
  qreal scalePageLargeAmount = 0.1;

  std::vector<PageTransform> pageTransformVector;

  QKeySequence nextKey = QKeySequence(Qt::Key_N);
  QKeySequence prevKey = QKeySequence(Qt::Key_P);

  QKeySequence scalePageUpSmallKey = QKeySequence(Qt::Key_Equal);
  QKeySequence scalePageUpLargeKey =
      QKeySequence(Qt::ShiftModifier | Qt::Key_Plus);
  QKeySequence scalePageDownSmallKey = QKeySequence(Qt::Key_Minus);
  QKeySequence scalePageDownLargeKey =
      QKeySequence(Qt::ShiftModifier | Qt::Key_Underscore);

  QKeySequence leftSmallKey = QKeySequence(Qt::Key_H);
  QKeySequence leftLargeKey = QKeySequence(Qt::ShiftModifier | Qt::Key_H);
  QKeySequence downSmallKey = QKeySequence(Qt::Key_J);
  QKeySequence downLargeKey = QKeySequence(Qt::ShiftModifier | Qt::Key_J);
  QKeySequence upSmallKey = QKeySequence(Qt::Key_K);
  QKeySequence upLargeKey = QKeySequence(Qt::ShiftModifier | Qt::Key_K);
  QKeySequence rightSmallKey = QKeySequence(Qt::Key_L);
  QKeySequence rightLargeKey = QKeySequence(Qt::ShiftModifier | Qt::Key_L);

  QKeySequence rotateClockSmallKey = QKeySequence(Qt::Key_R);
  QKeySequence rotateClockLargeKey =
      QKeySequence(Qt::ShiftModifier | Qt::Key_R);

  QKeySequence rotateAntiClockSmallKey = QKeySequence(Qt::Key_E);
  QKeySequence rotateAntiClockLargeKey =
      QKeySequence(Qt::ShiftModifier | Qt::Key_E);
};
