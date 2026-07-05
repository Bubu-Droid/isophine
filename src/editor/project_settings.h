#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <qguiapplication.h>
#include <qnamespace.h>

#include <QGuiApplication>
#include <QPdfDocument>
#include <QString>
#include <vector>

struct PageTransform {
  int xOffset = 0;
  int yOffset = 0;
  float rotationAmount = 0;
  float scaleAmount = 1.0f;
};

class ProjectSettings {
private:
  ProjectSettings() {}

public:
  static ProjectSettings& instance() {
    static ProjectSettings uniqueInstance;
    return uniqueInstance;
  }

  // QString pdfPath = "/home/bubu/Tests/latex-tests-2/main.pdf";
  QString pdfPath =
      "/home/bubu/Desktop/college/num-ana/sem-2/bose-gupta-na-2.pdf";
  QPdfDocument* pdfDocument = nullptr;

  float boundBoxScale = 1.0f;
  float boundBoxHeight = 0;
  float boundBoxWidth = 0;
  int boundBoxHorLinesCount = 5;
  int boundBoxVerLinesCount = 4;
  int currentPageNo = 0;
  // the value is allowed to go to a negative value, fix this
  float zoomCanvasAmount = 0.25f;

  int moveSmallAmount = 1;
  int moveLargeAmount = 10;
  float rotateSmallAmount = 0.1f;
  float rotateLargeAmount = 1.0f;
  float scalePageSmallAmount = 0.01f;
  float scalePageLargeAmount = 0.1f;

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

#endif // PROJECT_SETTINGS_H
