#include <QApplication>

#include "view.h"
using namespace s21;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  std::unique_ptr<Controller> controller = std::make_unique<Controller>();
  View w(std::move(controller));
  w.show();
  return QApplication::exec();
}
