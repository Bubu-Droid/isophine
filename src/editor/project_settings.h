#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <QPdfDocument>
#include <QString>
#include <vector>

struct PageTransform {
  int xOffset;
  int yOffset;
  float rotationAmount;
  float scaleAmount;
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
  float boundBoxHeight = 1740.0f;
  float boundBoxWidth = 1240.0f;
  int boundBoxHorLinesCount = 4;
  int boundBoxVerLinesCount = 5;
  int currentPageNo = 1;
  // the value is allowed to go to a negative value, fix this
  float zoomCanvasAmount = 0.25f;

  std::vector<PageTransform> pageTransformVector;
};

#endif // PROJECT_SETTINGS_H
