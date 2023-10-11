#include "chart.h"

namespace s21 {
Chart::Chart(QAbstractAxis *x, QAbstractAxis *y, QChartView *parent)
    : QChartView{parent}, axis_x_{x}, axis_y_{y} {
  SetupChart();
}

void Chart::SetupChart() {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setRenderHint(QPainter::Antialiasing);

  chart_.setMargins(QMargins(0, 0, 0, 0));
  chart_.setBackgroundBrush(Qt::transparent);
  chart_.legend()->hide();
  chart_.setAnimationOptions(QChart::NoAnimation);

  chart_.addAxis(axis_x_, Qt::AlignBottom);
  chart_.addAxis(axis_y_, Qt::AlignLeft);

  setChart(&chart_);
}

void Chart::Clear() {
  if (!chart_.series().empty()) {
    chart_.removeAllSeries();
  }
}

void Chart::AddSeries(QLineSeries *series) {
  chart_.zoomReset();
  chart_.removeAxis(axis_x_);
  chart_.removeAxis(axis_y_);
  chart_.addSeries(series);
  chart_.addAxis(axis_x_, Qt::AlignBottom);
  chart_.addAxis(axis_y_, Qt::AlignLeft);
  series->attachAxis(axis_x_);
  series->attachAxis(axis_y_);
}

void Chart::RemoveSeries(QXYSeries *series) {
  chart_.removeSeries(series);
  update();
}

void Chart::SetRangeX(qsizetype min, qsizetype max) {
  if (!axis_x_) {
    return;
  }
  chart_.zoomReset();
  axis_x_->setRange(min, max);
}

void Chart::SetRangeY(qsizetype min, qsizetype max) {
  if (!axis_y_) {
    return;
  }
  chart_.zoomReset();
  axis_y_->setRange(min, max);
}

void Chart::wheelEvent(QWheelEvent *event) {
  if (event->pixelDelta().y() < 0) {
    chart_.zoom(0.9);
  } else {
    chart_.zoom(1.1);
  }
}

void Chart::mousePressEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::MouseButton::MiddleButton) {
    chart_.zoomReset();
  } else {
    mouse_prev_pos_ = event->scenePosition();
  }
}

void Chart::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == Qt::MouseButton::LeftButton) {
    QPointF shift = mouse_prev_pos_ - event->scenePosition();
    chart_.scroll(shift.x(), -shift.y());
    mouse_prev_pos_ = event->scenePosition();
  }
}

}  // namespace s21
