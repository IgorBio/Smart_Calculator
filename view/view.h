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

 private slots:
  void OnSymbolClicked();
  void OnDotClicked();
  void OnOperationClicked();
  void OnFunctionClicked();
  void OnClearClicked();
  void OnBackspaceClicked();
  void OnEqualClicked();

 private:
  void SetupUi();

  Ui::View *ui_;
};

}  // namespace s21

#endif  // SMARTCALC_VIEW_VIEW_H_
