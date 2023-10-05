#ifndef SMARTCALC_MODEL_DEPOSIT_CALC_H_
#define SMARTCALC_MODEL_DEPOSIT_CALC_H_

#include <chrono>
#include <cmath>
#include <iomanip>
#include <map>
#include <numeric>
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
   * @brief Represents tax information for a specific year.
   *
   * This struct stores information related to taxes for a particular year,
   * including income, tax deduction, income after deduction, tax amount, and
   * payment date.
   */
  struct TaxInfo {
    std::string year;
    double income;
    double deduction;
    double deduction_income;
    double tax_sum;
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

  /**
   * @struct DatesComparator
   * @brief Functor for comparing dates represented as strings.
   *
   * This struct defines a comparator for dates represented as strings. It uses
   * the CompareDates function to compare two dates and returns true if the
   * first date is earlier than the second date.
   */
  struct DatesComparator {
    bool operator()(const std::string& date1, const std::string& date2) const {
      return CompareDates(date1, date2) == -1;
    }
  };

  static PaymentPlan Calculate(const DepositInfo& info);
  static std::string PlanToString(const PaymentPlan& plan,
                                  const DepositInfo& info);
  static std::string TaxToString(const std::vector<TaxInfo>& tax_info);

 private:
  static constexpr int kSecondsPerDay = 24 * 60 * 60;

  static std::vector<std::string> GenerateInterestDates(
      const DepositInfo& info);
  static std::map<std::string, double, DatesComparator> GenerateTransactions(
      const DepositInfo& info);
  static double CalculateInterest(const std::string& date1,
                                  const std::string& date2, double rate,
                                  double balance);
  static std::tm StringToDate(const std::string& date);
  static int CompareDates(const std::string& date1, const std::string& date2);
  static int TermToDays(const std::string& date, int term);
  static std::string AddDays(const std::string& date, int days);
  static std::string AddMonths(const std::string& date, int months);
  static std::string AddMonths(const std::string& date, int months,
                               const std::string& start_date);
  static int DaysInYear(const std::string& date);
  static int DaysInMonth(const std::tm& time);
  static int DaysBetweenDates(const std::string& date1,
                              const std::string& date2);
  static std::string FindNextYear(const std::string& date);
  static std::string ExtractYear(const std::string& date);
  static bool IsLeapYear(int year);
  static std::vector<TaxInfo> CalculateTax(const PaymentPlan& plan,
                                           const DepositInfo& info);
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_DEPOSIT_CALC_H_
