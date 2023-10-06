#include <QApplication>

#include "view.h"
using namespace s21;

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  View view;
  view.show();
  return QApplication::exec();
}
