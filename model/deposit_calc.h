#ifndef SMARTCALC_MODEL_DEPOSIT_CALC_H_
#define SMARTCALC_MODEL_DEPOSIT_CALC_H_

#include <cmath>
#include <ctime>
#include <numeric>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace s21 {

class DepositCalc {
 public:
  static std::tuple<std::vector<double>, double, double> Calculate(
      double sum, double rate, int term, bool is_annuity);

 private:
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_DEPOSIT_CALC_H_
