#ifndef SMARTCALC_MODEL_DEPOSIT_CALC_H_
#define SMARTCALC_MODEL_DEPOSIT_CALC_H_

#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {

/**
 * @class DepositCalc
 * @brief A class for calculating deposit payment details.
 *
 * The `DepositCalc` class provides methods for calculating deposit payments
 * and interest details based on the provided deposit parameters. It supports
 * various deposit types, including regular and periodic contributions.
 */
class DepositCalc {
 public:
  enum class PaymentPeriod {
    kAtMaturity,
    kDaily,
    kWeekly,
    kMonthly,
    kQuarterly,
    kSemiAnnually,
    kAnnually
  };

  enum class Regularity {
    kOneTime,
    kMonthly,
    kBiMonthly,
    kQuarterly,
    kSemiAnnually,
    kAnnually
  };

  /**
   * @struct Transaction
   * @brief Structure representing a financial transaction.
   *
   * This structure contains information about a financial transaction,
   * including its regularity, date, and the transaction amount.
   */
  struct Transaction {
    Regularity regularity;
    std::string date;
    double sum;
  };

  /**
   * @struct DepositInfo
   * @brief Structure for holding deposit-related parameters.
   *
   * This structure encapsulates details about a deposit, including the
   * principal sum, the deposit term in months, the starting date, annual
   * interest rate, annual tax rate, payment period, capitalization choice,
   * replenishments, and withdrawals.
   */
  struct DepositInfo {
    double sum;
    int term;
    std::string date;
    double rate;
    double tax_rate = 13.0;
    PaymentPeriod period;
    bool capitalize;
    std::vector<Transaction> replenishments;
    std::vector<Transaction> withdrawals;
  };

  static constexpr double kKeyRate = 7.5;
  static constexpr double kTaxDeduction = 1000000.0 * kKeyRate / 100.0;

  /**
   * @struct TaxInfo
   * @brief Structure for holding deposit tax details.
   *
   * This structure stores detailed information about taxes on deposits,
   * including year, income, possible deduction, income after deduction, amount
   * accrued taxes and the date on which the tax must be paid.
   */
  struct TaxInfo {
    std::string year;
    double income;
    double deduction = kTaxDeduction;
    double income_after_deduction;
    double tax_amount;
    std::string pay_before;
  };

  /**
   * @struct PaymentPlan
   * @brief Structure for holding deposit payment plan details.
   *
   * This structure stores the details of the deposit payment plan, including
   * dates, earned interests, remaining balances, and applicable tax amount.
   */
  struct PaymentPlan {
    std::vector<std::string> dates;
    std::vector<double> interests;
    std::vector<double> transactions;
    std::vector<double> balances;
    std::vector<TaxInfo> tax_info;
  };

  static PaymentPlan Calculate(const DepositInfo& info);

 public:
  static std::vector<std::string> GenerateInterestDates(
      const DepositInfo& info);
  static std::vector<std::pair<std::string, double>> GenerateTransactions(
      const DepositInfo& info);
  static std::tm StringToDate(const std::string& date);
  static int TermToDays(const std::string& date, int term);
  static std::string AddDays(const std::string& date, int days);
  static std::string AddMonths(const std::string& date, int months);
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_DEPOSIT_CALC_H_
