#ifndef SMARTCALC_VIEW_VIEW_H_
#define SMARTCALC_VIEW_VIEW_H_

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>

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
  explicit View(QWidget *parent = nullptr);
  ~View();

 private:
  void SetupUi();
  void ConnectSymbol();
  void ConnectDot();
  void ConnectOperation();
  void ConnectFunction();
  void ConnectClear();
  void ConnectBackspace();
  void ConnectEqual();

  Ui::View *ui_;
};

}  // namespace s21

#endif  // SMARTCALC_VIEW_VIEW_H_
