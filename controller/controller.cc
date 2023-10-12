#include "controller.h"

namespace s21 {

double Controller::Calculate(const std::string& expression, double x) {
  return MathCalc::Calculate(expression, x);
}

std::pair<std::vector<double>, std::vector<double>> Controller::Calculate(
    const std::string& expression, double x_min, double x_max,
    std::size_t size) {
  return MathCalc::Calculate(expression, x_min, x_max, size);
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
