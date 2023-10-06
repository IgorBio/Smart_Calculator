#include <QApplication>

#include "view.h"
using namespace s21;

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "C");
  QApplication app(argc, argv);
  View view;
  view.show();
  return QApplication::exec();
}
