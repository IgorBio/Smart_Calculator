#include "view.h"

#include "./ui_view.h"

namespace s21 {

View::View(std::unique_ptr<s21::Controller> controller, QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::View),
      controller_(std::move(controller)) {}

View::~View() { delete ui_; }

}  // namespace s21