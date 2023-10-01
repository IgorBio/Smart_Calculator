#include "credit_calc.h"

namespace s21 {

/**
 * @brief Calculate credit payment details based on the provided parameters.
 *
 * This method calculates either annuity or differentiated credit payments
 * based on the provided credit parameters.
 *
 * @param params The credit parameters including sum, rate, term, and type.
 * @return A PaymentPlan structure containing payment details.
 * @throws std::invalid_argument if any of the input parameters are invalid.
 */
CreditCalc::PaymentPlan CreditCalc::Calculate(CreditParams& params) {
  if (params.sum <= 0.0 || params.rate <= 0.0 || params.term <= 0) {
    throw std::invalid_argument("Invalid credit parameters");
  }

  PaymentPlan plan;
  double monthly_rate = params.rate / 12.0 / 100.0;
  plan.dates = GenerateDates(params.term);

  if (params.type == CreditType::kAnnuity) {
    plan.payments = CalculateAnnuity(params);
  } else {
    plan.payments = CalculateDifferentiated(params);
  }

  double balance = params.sum;
  for (int i = 0; i < params.term; ++i) {
    double interest = balance * monthly_rate;
    double principal = plan.payments[i] - interest;
    plan.interests.push_back(interest);
    plan.principals.push_back(principal);
    plan.balances.push_back(balance -= principal);
  }

  return plan;
}

/**
 * @brief Calculate annuity credit payments.
 *
 * This method calculates annuity credit payments based on the provided credit
 * parameters.
 *
 * @param params The credit parameters including sum, rate, and term.
 * @return A vector of monthly payments.
 */
std::vector<double> CreditCalc::CalculateAnnuity(CreditParams& params) {
  std::vector<double> payments;
  double monthly_payment =
      params.sum *
      (params.rate / 12.0 / 100.0 *
       std::pow(1 + params.rate / 12.0 / 100.0, params.term)) /
      (std::pow(1 + params.rate / 12.0 / 100.0, params.term) - 1);
  monthly_payment = std::round(monthly_payment * 100.0) / 100.0;
  payments.resize(params.term, monthly_payment);
  return payments;
}

/**
 * @brief Calculate differentiated credit payments.
 *
 * This method calculates differentiated credit payments based on the provided
 * credit parameters.
 *
 * @param params The credit parameters including sum, rate, and term.
 * @return A vector of monthly payments.
 */
std::vector<double> CreditCalc::CalculateDifferentiated(CreditParams& params) {
  std::vector<double> payments;
  double principal = params.sum / params.term;
  double balance = params.sum;

  for (int i = 0; i < params.term; ++i) {
    double interest = balance * params.rate / 12.0 / 100.0;
    double monthly_payment = std::round((principal + interest) * 100.0) / 100.0;
    payments.push_back(monthly_payment);
    balance -= principal;
  }

  return payments;
}

/**
 * @brief Generate dates for the credit payment plan.
 *
 * This method generates dates for each payment in the credit payment plan.
 *
 * @param term The loan term in months.
 * @return A vector of strings representing dates in the format "Month Year".
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
