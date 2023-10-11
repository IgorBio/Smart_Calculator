#ifndef SMARTCALC_VIEW_CHART_H_
#define SMARTCALC_VIEW_CHART_H_

#include <QWidget>
#include <QtCharts>

namespace s21 {

class Chart : public QChartView {
  Q_OBJECT

 public:
  explicit Chart(QAbstractAxis* x, QAbstractAxis* y,
                 QChartView* parent = nullptr);

  void AddSeries(QLineSeries* series);
  void RemoveSeries(QXYSeries* series);
  void SetRangeX(qsizetype min, qsizetype max);
  void SetRangeY(qsizetype min, qsizetype max);
  void Clear();

 private:
  QChart chart_;
  QAbstractAxis *axis_x_, *axis_y_;
  QLineSeries series_;
  QPointF mouse_prev_pos_;

  void SetupChart();
  void SetupAxisX();
  void SetupAxisY();

  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
};
}  // namespace s21

#endif  // SMARTCALC_VIEW_CHART_H_
