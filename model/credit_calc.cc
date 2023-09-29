#include "credit_calc.h"

namespace s21 {

/**
 * @brief Calculate credit payments, overpayment and total payment.
 *
 * This method calculates either annuity or differentiated credit payments
 * based on the provided credit parameters.
 *
 * @param sum The principal loan amount.
 * @param rate The annual interest rate (in percentage).
 * @param term The loan term in months.
 * @param is_annuity Set to true for annuity payments, false for differentiated.
 *
 * @return A tuple containing:
 *   - A vector of monthly payments.
 *   - The total overpayment (interest paid).
 *   - The total payment (including the principal).
 *
 * @throws std::invalid_argument if any of the input parameters are invalid.
 */
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

/**
 * @brief Calculate annuity credit payments.
 *
 * This method calculates annuity credit payments based on the provided credit
 * parameters.
 *
 * @param sum The principal loan amount.
 * @param monthly_rate The monthly interest rate (decimal).
 * @param term The loan term in months.
 *
 * @return A vector of monthly payments, each rounded to two decimal places.
 */
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

/**
 * @brief Calculate differentiated credit payments.
 *
 * This method calculates differentiated credit payments based on the provided
 * credit parameters. Differentiated payments consist of varying principal and
 * interest amounts over the loan term.
 *
 * @param sum The principal loan amount.
 * @param monthly_rate The monthly interest rate (decimal).
 * @param term The loan term in months.
 *
 * @return A vector of monthly payments, each rounded to two decimal places.
 */
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
