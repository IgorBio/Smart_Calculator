#ifndef SMARTCALC_MODEL_CREDIT_CALC_H_
#define SMARTCALC_MODEL_CREDIT_CALC_H_

#include <chrono>
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace s21 {

/**
 * @class CreditCalc
 * @brief A class for calculating credit payment details.
 *
 * The `CreditCalc` class provides static methods for calculating credit
 * payments, including annuity and differentiated payments, based on the
 * provided credit parameters. It supports both annuity and differentiated
 * repayment methods.
 */
class CreditCalc {
 public:
  static std::tuple<std::vector<std::string>, std::vector<double>,
                    std::vector<double>, std::vector<double>,
                    std::vector<double>>
  Calculate(double sum, double rate, int term, bool is_annuity);

 private:
  static std::vector<double> CalculateAnnuity(double sum, double monthly_rate,
                                              int term);
  static std::vector<double> CalculateDifferentiated(double sum,
                                                     double monthly_rate,
                                                     int term);

  static std::vector<std::string> GenerateDates(int term);
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_CREDIT_CALC_H_
