#include "controller.h"

namespace s21 {
double Controller::Calculate(const std::string& expression, double x) {
  return MathCalc::Calculate(expression, x);
}

std::vector<double> Controller::Calculate(const std::string& expression,
                                          const std::vector<double>& x) {
  return MathCalc::Calculate(expression, x);
}

CreditCalc::PaymentPlan Controller::Calculate(
    const CreditCalc::CreditInfo& info) {
  return CreditCalc::Calculate(info);
}

DepositCalc::PaymentPlan Controller::Calculate(
    const DepositCalc::DepositInfo& info) {
  return DepositCalc::Calculate(info);
}

}  // namespace s21
