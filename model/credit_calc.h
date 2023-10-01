#ifndef SMARTCALC_MODEL_CREDIT_CALC_H_
#define SMARTCALC_MODEL_CREDIT_CALC_H_

#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
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
  enum class CreditType { kAnnuity, kDifferentiated };

  /**
   * @struct CreditParams
   * @brief Structure for holding credit parameters.
   *
   * This structure encapsulates the parameters required for credit
   * calculations, including the principal loan amount, annual interest rate,
   * loan term in months, and the type of credit (annuity or differentiated).
   */
  struct CreditParams {
    double sum;
    double rate;
    int term;
    CreditType type;
  };

  /**
   * @struct PaymentPlan
   * @brief Structure for holding payment plan details.
   *
   * This structure stores the details of the credit payment plan, including
   * dates, monthly payments, principal amounts, interest amounts, and remaining
   * balances.
   */
  struct PaymentPlan {
    std::vector<std::string> dates;
    std::vector<double> payments;
    std::vector<double> principals;
    std::vector<double> interests;
    std::vector<double> balances;
  };

  static PaymentPlan Calculate(CreditParams& params);

 private:
  static std::vector<double> CalculateAnnuity(CreditParams& params);
  static std::vector<double> CalculateDifferentiated(CreditParams& params);
  static std::vector<std::string> GenerateDates(int term);
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_CREDIT_CALC_H_
