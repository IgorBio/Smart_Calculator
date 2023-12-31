#include "view.h"

#include "ui_view.h"

namespace s21 {

View::View(QWidget *parent) : QMainWindow{parent}, ui_{new Ui::View} {
  setlocale(LC_ALL, "C");
  SetupUi();
  SetupChart();
}

View::~View() { delete ui_; }

void View::SetupUi() {
  ui_->setupUi(this);
  QList<QPushButton *> buttons = {
      ui_->btn_0,        ui_->btn_1,        ui_->btn_2,    ui_->btn_3,
      ui_->btn_4,        ui_->btn_5,        ui_->btn_6,    ui_->btn_7,
      ui_->btn_8,        ui_->btn_9,        ui_->btn_x,    ui_->btn_exp,
      ui_->btn_lbracket, ui_->btn_rbracket, ui_->btn_plus, ui_->btn_minus,
      ui_->btn_mul,      ui_->btn_div,      ui_->btn_mod,  ui_->btn_pow,
      ui_->btn_sin,      ui_->btn_cos,      ui_->btn_tan,  ui_->btn_asin,
      ui_->btn_acos,     ui_->btn_atan,     ui_->btn_ln,   ui_->btn_log,
      ui_->btn_sqrt,     ui_->btn_dot,      ui_->btn_back};

  for (auto button : buttons) {
    connect(button, &QPushButton::clicked, this, [this]() { PressButton(); });
  }

  connect(ui_->btn_c, &QPushButton::clicked, this, [this]() { PressClear(); });
  connect(ui_->btn_eq, &QPushButton::clicked, this, [this]() { PressEqual(); });
  connect(ui_->btn_plot, &QPushButton::clicked, this, [this]() { Plot(); });
  connect(ui_->btn_run_credit, &QPushButton::clicked, this,
          [this]() { RunCredit(); });
  connect(ui_->check_cap, &QCheckBox::stateChanged,
          [=](int state) { ui_->cb_cap->setEnabled(state == Qt::Checked); });
}

void View::ResetUi() {
  QString display_text = ui_->display->text();
  if (display_text == "0") {
    QString style = ui_->display_res->styleSheet();
    style.replace("color: #ff4a50;", "color: #43eb99;");
    style.replace("font: 22px;", "font: 26px;");
    ui_->display_res->setStyleSheet(style);
    ui_->display_input->setText("");
    ui_->display_res_graph->setText("");
    ui_->display_res->setText("=0.00");
  }
}

void View::PressButton() {
  ResetUi();
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QString text = Validator::Validate(ui_->display->text(), button->text());
    ui_->display->setText(text);
    ui_->display_graph->setText(text);
  }
}

void View::PressClear() {
  ResetUi();
  ui_->display->setText("0");
  ui_->display_graph->setText("0");
  ui_->display_res->setText("=0.00");
  ui_->display_res_graph->setText("");
  ui_->display_input->setText("");
}

void View::PressEqual() {
  ResetUi();
  QString style = ui_->display_res->styleSheet();
  try {
    qreal result = Controller::Calculate(ui_->display->text().toStdString(),
                                         ui_->x_value->value());

    if (fabs(result) < 1e-6 || fabs(result) > 1e10) {
      std::stringstream ss;
      ss << result << std::scientific;
      ui_->display_res->setText("=" + QString::fromStdString(ss.str()));
    } else {
      ui_->display_res->setText("=" + QString::number(result, 'f', 6));
    }
    ui_->display_input->setText(ui_->display->text());
    style.replace("color: #ff4a50;", "color: #43eb99;");
    style.replace("font: 22px;", "font: 26px;");
    ui_->display_res->setStyleSheet(style);
    ui_->display->setText("0");
  } catch (const std::exception &err) {
    ui_->display_input->setText(ui_->display->text());
    ui_->display->setText("0");
    ui_->display_graph->setText("0");
    style.replace("color: #43eb99;", "color: #ff4a50;");
    style.replace("font: 26px;", "font: 22px;");
    ui_->display_res->setStyleSheet(style);
    ui_->display_res->setText(err.what());
  }
}

void View::SetupChart() {
  chart_ = new Chart(SetupAxis("x"), SetupAxis("y"));
  ui_->chart_field->layout()->addWidget(chart_);
  chart_->SetRangeX(ui_->x_min->value(), ui_->x_max->value());
  chart_->SetRangeY(ui_->y_min->value(), ui_->y_max->value());
}

QValueAxis *View::SetupAxis(const QString &name) {
  QValueAxis *axis(new QValueAxis());
  axis->setGridLinePen(
      QPen(QColor(134, 143, 155, 255), 1, Qt::DashLine, Qt::RoundCap));
  axis->setLinePenColor(QColor(200, 200, 200, 255));
  axis->setTickCount(10);

  axis->setLabelsColor(QColor(200, 200, 200, 255));
  QFont labels_font = axis->labelsFont();
  labels_font.setPointSize(8);
  axis->setLabelsFont(labels_font);

  axis->setTitleText(name);
  QFont title_font = axis->titleFont();
  title_font.setPointSize(18);
  axis->setTitleFont(title_font);

  QBrush title_brush(QColor(61, 222, 183, 255));
  axis->setTitleBrush(title_brush);

  return axis;
}

void View::Plot() {
  ResetUi();
  chart_->Clear();
  chart_->SetRangeX(ui_->x_min->value(), ui_->x_max->value());
  chart_->SetRangeY(ui_->y_min->value(), ui_->y_max->value());

  QLineSeries *series = new QLineSeries();
  QString style = ui_->display_graph->styleSheet();

  try {
    auto [x, y] = Controller::Calculate(
        ui_->display_graph->text().toStdString(), ui_->x_min->value(),
        ui_->x_max->value(), kNumPoints);

    for (qsizetype i = 0; i < kNumPoints; ++i) {
      series->append(x[i], y[i]);
    }

    series->setPen(QPen(QColor(61, 222, 183, 255), kLineWidth, Qt::SolidLine,
                        Qt::RoundCap));

    chart_->AddSeries(series);
    ui_->display_res_graph->setText("");
  } catch (const std::exception &err) {
    ui_->display_res_graph->setText(err.what());
  }
}

void View::RunCredit() {
  CreditCalc::CreditInfo info = {ui_->amount_credit->value(),
                                 ui_->rate_credit->value(),
                                 ui_->term_credit->value(),
                                 ui_->type_credit->currentText() == "Annuity"
                                     ? CreditCalc::CreditType::kAnnuity
                                     : CreditCalc::CreditType::kDifferentiated};
  CreditCalc::PaymentPlan plan = Controller::Calculate(info);

  ui_->table_credit->clear();

  ui_->table_credit->setRowCount(plan.dates.size());
  ui_->table_credit->setColumnCount(5);
  ui_->table_credit->setHorizontalHeaderLabels(
      {"Date", "Payment", "Principal", "Interest", "Balance"});

  for (std::size_t i = 0; i < plan.dates.size(); ++i) {
    ui_->table_credit->setItem(
        i, 0, new QTableWidgetItem(QString::fromStdString(plan.dates[i])));
    ui_->table_credit->setItem(
        i, 1, new QTableWidgetItem(QString::number(plan.payments[i], 'f', 2)));
    ui_->table_credit->setItem(
        i, 2,
        new QTableWidgetItem(QString::number(plan.principals[i], 'f', 2)));
    ui_->table_credit->setItem(
        i, 3, new QTableWidgetItem(QString::number(plan.interests[i], 'f', 2)));
    ui_->table_credit->setItem(
        i, 4, new QTableWidgetItem(QString::number(plan.balances[i], 'f', 2)));
  }
  ui_->table_credit->resizeColumnToContents(0);

  double total_interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  double total_payment =
      std::accumulate(plan.payments.begin(), plan.payments.end(), 0.0);

  if (info.type == CreditCalc::CreditType::kAnnuity) {
    double monthly_payment = plan.payments[0];

    ui_->lbl_monthly_payment->setText(
        QString("Monthly payment: %1").arg(monthly_payment, 0, 'f', 2));
  } else if (info.type == CreditCalc::CreditType::kDifferentiated) {
    double first_payment = plan.payments.front();
    double last_payment = plan.payments.back();

    ui_->lbl_monthly_payment->setText(QString("Monthly payment:\n%1 ... %2")
                                          .arg(first_payment, 0, 'f', 2)
                                          .arg(last_payment, 0, 'f', 2));
  }

  ui_->lbl_total_interest->setText(
      QString("Overpayment: %1").arg(total_interest, 0, 'f', 2));
  ui_->lbl_total_payment->setText(
      QString("Total payment: %1").arg(total_payment, 0, 'f', 2));
}

}  // namespace s21
