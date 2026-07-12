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
  bool isSaved;

  qreal boundBoxScale = 1.0;
  qreal boundBoxHeight;
  qreal boundBoxWidth;
  QColor boundBoxLineColor = QColor(255, 96, 208);
  int boundBoxHorLinesCount = 5;
  int boundBoxVerLinesCount = 3;
  int currentPageNo;
  qreal zoomCanvasAmount = 0.25;

  qreal moveSmallAmount;
  qreal moveLargeAmount;
  qreal rotateSmallAmount;
  qreal rotateLargeAmount;
  qreal scalePageSmallAmount;
  qreal scalePageLargeAmount;

  std::vector<PageTransform> pageTransformVector;

  QKeySequence nextKey;
  QKeySequence prevKey;

  QKeySequence scalePageUpSmallKey;
  QKeySequence scalePageUpLargeKey;
  QKeySequence scalePageDownSmallKey;
  QKeySequence scalePageDownLargeKey;

  QKeySequence leftSmallKey;
  QKeySequence leftLargeKey;
  QKeySequence downSmallKey;
  QKeySequence downLargeKey;
  QKeySequence upSmallKey;
  QKeySequence upLargeKey;
  QKeySequence rightSmallKey;
  QKeySequence rightLargeKey;

  QKeySequence rotateClockSmallKey;
  QKeySequence rotateClockLargeKey;

  QKeySequence rotateAntiClockSmallKey;
  QKeySequence rotateAntiClockLargeKey;
};
