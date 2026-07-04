#include <QApplication>

#include "isophine_editor.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  IsophineEditor w;
  w.show();
  return QApplication::exec();
}