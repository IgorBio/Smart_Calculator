#include "view.h"

#include "./ui_view.h"

namespace s21 {

View::View(QWidget *parent) : QMainWindow{parent}, ui_{new Ui::View} {
  SetupUi();
}

View::~View() { delete ui_; }

void View::SetupUi() {
  ui_->setupUi(this);
  connect(ui_->btn_0, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_1, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_2, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_3, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_4, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_5, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_6, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_7, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_8, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_9, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_x, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_exp, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_lbracket, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));
  connect(ui_->btn_rbracket, SIGNAL(clicked()), this, SLOT(OnSymbolClicked()));

  connect(ui_->btn_plus, SIGNAL(clicked()), this, SLOT(OnOperationClicked()));
  connect(ui_->btn_minus, SIGNAL(clicked()), this, SLOT(OnOperationClicked()));
  connect(ui_->btn_mul, SIGNAL(clicked()), this, SLOT(OnOperationClicked()));
  connect(ui_->btn_div, SIGNAL(clicked()), this, SLOT(OnOperationClicked()));
  connect(ui_->btn_mod, SIGNAL(clicked()), this, SLOT(OnOperationClicked()));
  connect(ui_->btn_pow, SIGNAL(clicked()), this, SLOT(OnOperationClicked()));

  connect(ui_->btn_sin, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_cos, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_tan, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_asin, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_acos, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_atan, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_ln, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_log, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));
  connect(ui_->btn_sqrt, SIGNAL(clicked()), this, SLOT(OnFunctionClicked()));

  connect(ui_->btn_dot, SIGNAL(clicked()), this, SLOT(OnDotClicked()));
  connect(ui_->btn_c, SIGNAL(clicked()), this, SLOT(OnClearClicked()));
  connect(ui_->btn_back, SIGNAL(clicked()), this, SLOT(OnBackspaceClicked()));
  connect(ui_->btn_equal, SIGNAL(clicked()), this, SLOT(OnEqualClicked()));
}

void View::OnSymbolClicked() {
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

void View::OnDotClicked() {
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

void View::OnOperationClicked() {
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

void View::OnFunctionClicked() {
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

void View::OnClearClicked() {
  ui_->display->setText("0");
  ui_->display_graph->setText("0");
  ui_->display_res->setText("=0.00");
}

void View::OnBackspaceClicked() {
  QString text = ui_->display->text().trimmed();
  text.chop(1);
  if (text.isEmpty()) {
    text = "0";
  }
  ui_->display->setText(text);
  ui_->display_graph->setText(text);
}

void View::OnEqualClicked() {
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
