#ifndef SMARTCALC_VIEW_VIEW_H_
#define SMARTCALC_VIEW_VIEW_H_

#include <QMainWindow>

#include "chart.h"
#include "controller.h"
#include "validator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {

class View : public QMainWindow {
  Q_OBJECT

 public:
  explicit View(QWidget* parent = nullptr);
  ~View();

 private:
  static constexpr qsizetype kNumPoints{10000};
  static constexpr qreal kLineWidth{2};

  Ui::View* ui_;
  Chart* chart_;

  void SetupUi();
  void SetupChart();
  void ResetUi();
  void PressButton();
  void PressClear();
  void PressEqual();

  void Plot();
  QValueAxis* SetupAxis(const QString& name);
};
}  // namespace s21

#endif  // SMARTCALC_VIEW_VIEW_H_
