#pragma once

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
  qreal xOffset = 0;
  qreal yOffset = 0;
  qreal rotationAmount = 0;
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
  QString pdfPath =
      "/home/bubu/Desktop/college/num-ana/sem-2/bose-gupta-na-2.pdf";
  // QString pdfPath = "/home/bubu/Tests/latex-tests-2/main.pdf";

  qreal boundBoxScale = 1.0;
  qreal boundBoxHeight = 1122.67;
  qreal boundBoxWidth = 793.33;
  int boundBoxHorLinesCount = 5;
  int boundBoxVerLinesCount = 3;
  int currentPageNo = 0;
  qreal zoomCanvasAmount = 0.25;

  qreal moveSmallAmount = 1.0;
  qreal moveLargeAmount = 10.0;
  qreal rotateSmallAmount = 0.1;
  qreal rotateLargeAmount = 1.0;
  qreal scalePageSmallAmount = 0.01;
  qreal scalePageLargeAmount = 0.1;

  std::vector<PageTransform> pageTransformVector;

  Qt::Key nextKey = Qt::Key_N;
  Qt::Key prevKey = Qt::Key_P;

  Qt::Key scalePageUpSmallKey = Qt::Key_Equal;
  Qt::Key scalePageUpLargeKey = Qt::Key_Plus;
  Qt::Key scalePageDownSmallKey = Qt::Key_Minus;
  Qt::Key scalePageDownLargeKey = Qt::Key_Underscore;

  char leftSmallKey = 'h';
  char leftLargeKey = 'H';
  char downSmallKey = 'j';
  char downLargeKey = 'J';
  char upSmallKey = 'k';
  char upLargeKey = 'K';
  char rightSmallKey = 'l';
  char rightLargeKey = 'L';

  char rotateClockSmallKey = 'r';
  char rotateClockLargeKey = 'R';

  char rotateAntiClockSmallKey = 'e';
  char rotateAntiClockLargeKey = 'E';
};
