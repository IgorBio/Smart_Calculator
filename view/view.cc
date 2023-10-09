#include "view.h"

#include <QDebug>

#include "./ui_view.h"

namespace s21 {

View::View(QWidget *parent) : QMainWindow{parent}, ui_{new Ui::View} {
  setlocale(LC_ALL, "C");
  SetupUi();
}

View::~View() { delete ui_; }

void View::SetupUi() {
  ui_->setupUi(this);

  QList<QPushButton *> symbols = {
      ui_->btn_0, ui_->btn_1,   ui_->btn_2,        ui_->btn_3,       ui_->btn_4,
      ui_->btn_5, ui_->btn_6,   ui_->btn_7,        ui_->btn_8,       ui_->btn_9,
      ui_->btn_x, ui_->btn_exp, ui_->btn_lbracket, ui_->btn_rbracket};

  QList<QPushButton *> operations = {ui_->btn_plus, ui_->btn_minus,
                                     ui_->btn_mul,  ui_->btn_div,
                                     ui_->btn_mod,  ui_->btn_pow};

  QList<QPushButton *> functions = {
      ui_->btn_sin,  ui_->btn_cos, ui_->btn_tan, ui_->btn_asin, ui_->btn_acos,
      ui_->btn_atan, ui_->btn_ln,  ui_->btn_log, ui_->btn_sqrt};

  for (auto button : symbols) {
    connect(button, &QPushButton::clicked, this, [this]() { PressSymbol(); });
  }

  for (auto button : operations) {
    connect(button, &QPushButton::clicked, this,
            [this]() { PressOperation(); });
  }

  for (auto button : functions) {
    connect(button, &QPushButton::clicked, this, [this]() { PressFunction(); });
  }

  connect(ui_->btn_dot, &QPushButton::clicked, this, [this]() { PressDot(); });
  connect(ui_->btn_c, &QPushButton::clicked, this, [this]() { PressClear(); });
  connect(ui_->btn_back, &QPushButton::clicked, this,
          [this]() { PressBackspace(); });
  connect(ui_->btn_equal, &QPushButton::clicked, this,
          [this]() { PressEqual(); });
}

void View::ResetUi() {
  QString display_text = ui_->display->text();
  if (display_text == "0") {
    QString style = ui_->display_res->styleSheet();
    style.replace("color: #ff4a50;", "color: #43eb99;");
    style.replace("font: 22px;", "font: 26px;");
    ui_->display_res->setStyleSheet(style);
    ui_->display_res->setText("=0.00");
  }
}

void View::PressSymbol() {
  ResetUi();
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QString button_text = button->text();
    QString display_text = ui_->display->text();
    QString new_text =
        (display_text == "0") ? button_text : display_text + button_text;
    ui_->display->setText(new_text);
    ui_->display_graph->setText(new_text);
  }
}

void View::PressDot() {
  ResetUi();
  QString text = ui_->display->text();
  QRegularExpression miss_zero("[^.0-9]$");
  if (miss_zero.match(text).hasMatch()) {
    ui_->display->setText(text + "0.");
  } else {
    QRegularExpression no_dot("\\s*\\d+\\.\\d*\\s*$");
    if (!no_dot.match(text).hasMatch()) {
      ui_->display->setText(text + ".");
    }
  }
}

void View::PressOperation() {
  ResetUi();
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (!button) {
    return;
  }

  QString display_text = ui_->display->text();
  QString button_text = button->text();

  if (display_text == "0" && LastIsUnary(button_text)) {
    display_text = "";
  } else if (LastIsUnary(display_text) && IsUnary(button_text)) {
    display_text.chop(1);
  } else if (LastIsUnary(display_text)) {
    return;
  } else if (LastIsBinary(display_text) && LastIsUnary(button_text)) {
  } else if (LastIsBinary(display_text)) {
    display_text.chop(2);
    button_text += " ";
  } else if (LastIsMod(display_text)) {
    display_text.chop(4);
    button_text += " ";
  } else if (!PlaceBinary(display_text) && IsBinary(button_text)) {
    return;
  } else if (PlaceUnary(display_text)) {
  } else if (PlaceBinary(display_text)) {
    button_text = " " + button_text + " ";
  }

  ui_->display->setText(display_text + button_text);
  ui_->display_graph->setText(display_text + button_text);
}

void View::PressFunction() {
  ResetUi();
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (!button) {
    return;
  }

  QString display_text = ui_->display->text();
  QString space = (display_text.right(1) == " ") ? "" : " ";
  QString button_text = space + button->text() + "(";
  QString new_text =
      (display_text == "0") ? button_text : display_text + button_text;
  ui_->display->setText(new_text);
  ui_->display_graph->setText(new_text);
}

void View::PressClear() {
  ResetUi();
  ui_->display->setText("0");
  ui_->display_graph->setText("0");
  ui_->display_res->setText("=0.00");
}

void View::PressBackspace() {
  ResetUi();
  QString text = ui_->display->text();
  QString last = LastToken(text);
  text.chop(last.size());
  if (text.isEmpty()) {
    text = "0";
  }
  ui_->display->setText(text.trimmed());
  ui_->display_graph->setText(text.trimmed());
}

void View::PressEqual() {
  ResetUi();
  QString style = ui_->display_res->styleSheet();
  try {
    double result = Controller::Calculate(ui_->display->text().toStdString(),
                                          ui_->x_value->value());

    if (fabs(result) < 1e-6 || fabs(result) > 1e10) {
      std::stringstream ss;
      ss << result << std::scientific;
      ui_->display_res->setText("=" + QString::fromStdString(ss.str()));
    } else {
      ui_->display_res->setText("=" + QString::number(result, 'f', 6));
    }
    style.replace("color: #ff4a50;", "color: #43eb99;");
    style.replace("font: 22px;", "font: 26px;");
    ui_->display_res->setStyleSheet(style);
    ui_->display->setText("0");
  } catch (const std::exception &err) {
    ui_->display->setText("0");
    ui_->display_graph->setText("0");
    style.replace("color: #43eb99;", "color: #ff4a50;");
    style.replace("font: 26px;", "font: 22px;");
    ui_->display_res->setStyleSheet(style);
    ui_->display_res->setText(err.what());
  }
}

bool View::PlaceUnary(const QString &text) {
  if (text.isEmpty()) {
    return true;
  }

  return text.right(1) == "(" || text.right(1) == "e" || text.right(1) == " ";
}

bool View::PlaceBinary(const QString &text) {
  return text.right(1) != "(" && text.right(1) != "e";
}

bool View::IsUnary(const QString &text) { return text == "+" || text == "-"; }

bool View::IsBinary(const QString &text) {
  return text == "*" || text == "/" || text == "^" || text == "mod";
}

bool View::LastIsUnary(const QString &text) {
  return text.right(1) == "+" || text.right(1) == "-";
}

bool View::LastIsBinary(const QString &text) {
  return text.right(2) == "+ " || text.right(2) == "- " ||
         text.right(2) == "* " || text.right(2) == "/ " ||
         text.right(2) == "^ ";
}

bool View::LastIsMod(const QString &text) { return text.right(4) == "mod "; }

QString View::LastToken(const QString &text) {
  static QRegularExpression regex(
      R"((\d)|([+\-*/]|\bmod\s|\bmod\b)|(ln|log|sin|cos|tan|asin|acos|atan|sqrt)|([()xe.]))");
  QRegularExpressionMatchIterator it = regex.globalMatch(text);
  QString last_token;

  while (it.hasNext()) {
    QRegularExpressionMatch match = it.next();
    last_token = match.captured(0);
  }

  return last_token;
}

}  // namespace s21
