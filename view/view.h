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
  explicit View(QWidget* parent = nullptr);
  ~View();

 private:
  void SetupUi();
  void ResetUi();
  void PressSymbol();
  void PressDot();
  void PressOperation();
  void PressFunction();
  void PressClear();
  void PressBackspace();
  void PressEqual();
  bool PlaceUnary(const QString& text);
  bool PlaceBinary(const QString& text);
  bool IsUnary(const QString& text);
  bool IsBinary(const QString& text);
  bool LastIsUnary(const QString& text);
  bool LastIsBinary(const QString& text);
  bool LastIsMod(const QString& text);
  QString LastToken(const QString& text);

  Ui::View* ui_;
};

}  // namespace s21

#endif  // SMARTCALC_VIEW_VIEW_H_
