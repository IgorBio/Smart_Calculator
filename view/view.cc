#include "view.h"

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
    connect(button, &QPushButton::clicked, this, [this]() { ConnectSymbol(); });
  }

  for (auto button : operations) {
    connect(button, &QPushButton::clicked, this,
            [this]() { ConnectOperation(); });
  }

  for (auto button : functions) {
    connect(button, &QPushButton::clicked, this,
            [this]() { ConnectFunction(); });
  }

  connect(ui_->btn_dot, &QPushButton::clicked, this,
          [this]() { ConnectDot(); });
  connect(ui_->btn_c, &QPushButton::clicked, this,
          [this]() { ConnectClear(); });
  connect(ui_->btn_back, &QPushButton::clicked, this,
          [this]() { ConnectBackspace(); });
  connect(ui_->btn_equal, &QPushButton::clicked, this,
          [this]() { ConnectEqual(); });
}

void View::ConnectSymbol() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QString button_text = button->text();
    QString current_text = ui_->display->text();
    QString new_text =
        (current_text == "0") ? button_text : current_text + button_text;
    ui_->display->setText(new_text);
    ui_->display_graph->setText(new_text);
  }
}

void View::ConnectDot() {
  QString text = ui_->display->text();
  if (!(text.endsWith('.') || (text.right(1) >= '0' && text.right(1) <= '9'))) {
    ui_->display->setText(text + "0.");
  } else {
    static QRegularExpression regex("\\s*[0-9]+\\.[0-9]+\\.\\s*$");
    if (!regex.match(text + ".").hasMatch()) {
      ui_->display->setText(text + ".");
    }
  }
}

void View::ConnectOperation() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QString button_text = " " + button->text() + " ";
    QString current_text = ui_->display->text();
    QString new_text =
        (current_text == "0") ? button_text : current_text + button_text;
    ui_->display->setText(new_text);
    ui_->display_graph->setText(new_text);
  }
}

void View::ConnectFunction() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QString button_text = " " + button->text() + "(";
    QString current_text = ui_->display->text();
    QString new_text =
        (current_text == "0") ? button_text : current_text + button_text;
    ui_->display->setText(new_text);
    ui_->display_graph->setText(new_text);
  }
}

void View::ConnectClear() {
  ui_->display->setText("0");
  ui_->display_graph->setText("0");
  ui_->display_res->setText("=0.00");
}

void View::ConnectBackspace() {
  QString text = ui_->display->text().trimmed();
  text.chop(1);
  if (text.isEmpty()) {
    text = "0";
  }
  ui_->display->setText(text);
  ui_->display_graph->setText(text);
}

void View::ConnectEqual() {
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
    style += "color: #43eb99; font: 26px;";
    ui_->display_res->setStyleSheet(style);
    ui_->display->setText("0");
  } catch (const std::exception &err) {
    ui_->display->setText("0");
    ui_->display_graph->setText("0");
    style += "color: #ff4a50; font: 22px;";
    ui_->display_res->setStyleSheet(style);
    ui_->display_res->setText(err.what());
  }
}

}  // namespace s21
