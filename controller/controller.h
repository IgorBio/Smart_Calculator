#ifndef SMARTCALC_CONTROLLER_CONTROLLER_H_
#define SMARTCALC_CONTROLLER_CONTROLLER_H_

#include <memory>

#include "credit_calc.h"
#include "deposit_calc.h"
#include "math_calc.h"

namespace s21 {

class Controller {
 public:
  static double Calculate(const std::string& expression, double x = 0.0);
  static std::vector<double> Calculate(const std::string& expression,
                                       const std::vector<double>& x);
  static CreditCalc::PaymentPlan Calculate(const CreditCalc::CreditInfo& info);
  static DepositCalc::PaymentPlan Calculate(
      const DepositCalc::DepositInfo& info);
};

}  // namespace s21

#endif  // SMARTCALC_CONTROLLER_CONTROLLER_H_
