#ifndef SMARTCALC_MODEL_CREDIT_CALC_H_
#define SMARTCALC_MODEL_CREDIT_CALC_H_

#include <cmath>
#include <ctime>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace s21 {

class CreditCalc {
 public:
  static std::tuple<std::vector<double>, double, double> Calculate(
      double sum, double rate, int term, bool is_annuity);

 private:
  static std::vector<double> CalculateAnnuity(double sum, double monthly_rate,
                                              int term);
  static std::vector<double> CalculateDifferentiated(double sum,
                                                     double monthly_rate,
                                                     int term);
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_CREDIT_CALC_H_
