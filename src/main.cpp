#include <QApplication>

#include "dashboard.h"

// #include "isophine_editor.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Dashboard w;
  w.show();
  // IsophineEditor w;
  // w.show();
  return QApplication::exec();
}
