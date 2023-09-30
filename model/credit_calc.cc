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
std::tuple<std::vector<std::string>, std::vector<double>, std::vector<double>,
           std::vector<double>, std::vector<double>>
CreditCalc::Calculate(double sum, double rate, int term, bool is_annuity) {
  if (sum <= 0.0 || rate <= 0.0 || term <= 0) {
    throw std::invalid_argument("Invalid credit parameters");
  }

  std::vector<double> payments, principals, interests, balances;
  double monthly_rate = rate / 12.0 / 100.0;
  std::vector<std::string> dates = GenerateDates(term);

  if (is_annuity) {
    payments = CalculateAnnuity(sum, monthly_rate, term);
  } else {
    payments = CalculateDifferentiated(sum, monthly_rate, term);
  }

  double balance = sum;
  for (int i = 0; i < term; ++i) {
    double interest = balance * monthly_rate;
    double principal = payments[i] - interest;
    interests.push_back(interest);
    principals.push_back(principal);
    balances.push_back(balance -= principal);
  }

  return std::make_tuple(dates, payments, principals, interests, balances);
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
  double principal = sum / term;
  double balance = sum;

  for (int i = 0; i < term; ++i) {
    double interest = balance * monthly_rate;
    double monthly_payment = std::round((principal + interest) * 100.0) / 100.0;
    payments.push_back(monthly_payment);
    balance -= principal;
  }

  return payments;
}

/**
 * @brief Generates a sequence of dates in the format "Month Year" starting from
 * the current date and extending for the specified number of months forward.
 *
 * This method creates a vector of strings representing months and years,
 * starting from the current date, and incrementing months for the specified
 * number of months forward.
 *
 * @param term The number of months in the sequence.
 * @return A vector of strings containing months and years in the format "Month
 * Year".
 */
std::vector<std::string> CreditCalc::GenerateDates(int term) {
  std::vector<std::string> dates;
  auto time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm* date = std::localtime(&time);
  int month = date->tm_mon;
  int year = date->tm_year + 1900;

  for (int i = 0; i < term; ++i) {
    std::stringstream ss;
    ss << std::put_time(date, "%B %Y");
    dates.push_back(ss.str());

    ++month;
    if (month >= 12) {
      month = 0;
      ++year;
    }

    date->tm_mon = month;
    date->tm_year = year - 1900;
    time = std::mktime(date);
    date = std::localtime(&time);
  }

  return dates;
}

}  // namespace s21
