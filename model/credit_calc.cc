#include "credit_calc.h"

namespace s21 {

std::tuple<std::vector<double>, double, double> CreditCalc::Calculate(
    double sum, double rate, int term, bool is_annuity) {
  if (sum <= 0.0 || rate <= 0.0 || term <= 0) {
    throw std::invalid_argument("Invalid credit parameters");
  }

  std::vector<double> payments;
  double monthly_rate = rate / 12.0 / 100.0;

  if (is_annuity) {
    payments = CalculateAnnuity(sum, monthly_rate, term);
  } else {
    payments = CalculateDifferentiated(sum, monthly_rate, term);
  }

  double total_payment = std::accumulate(payments.begin(), payments.end(), 0.0);
  double total_overpayment = total_payment - sum;

  return std::make_tuple(payments, total_overpayment, total_payment);
}

std::vector<double> CreditCalc::CalculateAnnuity(double sum,
                                                 double monthly_rate,
                                                 int term) {
  std::vector<double> payments;
  double monthly_payment = sum *
                           (monthly_rate * std::pow(1 + monthly_rate, term)) /
                           (std::pow(1 + monthly_rate, term) - 1);
  monthly_payment = std::round(monthly_payment * 100.0) / 100.0;
  payments.resize(term, monthly_payment);
  return payments;
}

std::vector<double> CreditCalc::CalculateDifferentiated(double sum,
                                                        double monthly_rate,
                                                        int term) {
  std::vector<double> payments;
  double monthly_principal = sum / term;
  double remaining_balance = sum;

  for (int i = 0; i < term; ++i) {
    double monthly_interest = remaining_balance * monthly_rate;
    double monthly_payment =
        std::round((monthly_principal + monthly_interest) * 100.0) / 100.0;
    payments.push_back(monthly_payment);
    remaining_balance -= monthly_principal;
  }

  return payments;
}

}  // namespace s21
