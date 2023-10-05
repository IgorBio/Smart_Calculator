#ifndef SMARTCALC_VIEW_VIEW_H_
#define SMARTCALC_VIEW_VIEW_H_

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>

#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {

class View : public QMainWindow {
  Q_OBJECT

 public:
  explicit View(std::unique_ptr<Controller> controller,
                QWidget *parent = nullptr);
  ~View();

 private:
  Ui::View *ui_;
  std::unique_ptr<Controller> controller_;
};

}  // namespace s21

#endif  // SMARTCALC_VIEW_VIEW_H_
