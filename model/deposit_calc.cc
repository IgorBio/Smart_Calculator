#include "deposit_calc.h"

namespace s21 {

/**
 * @brief Calculates the payment plan for a given deposit information.
 *
 * This method calculates the payment plan based on the provided DepositInfo.
 * It generates interest dates and transactions, processes them, and calculates
 * the balance, interest, and transactions for each date.
 *
 * @param info The deposit information including principal amount, interest
 * rate, and transaction details.
 * @return PaymentPlan object containing the payment plan details.
 *
 * The payment plan includes dates, accrued interests, balance changes, payouts,
 * and balances for each date. If the info.capitalize flag is true, the function
 * capitalizes the interest.
 */
DepositCalc::PaymentPlan DepositCalc::Calculate(const DepositInfo& info) {
  PaymentPlan plan;
  auto interest_dates = GenerateInterestDates(info);
  auto transactions = GenerateTransactions(info);

  auto interests_it = interest_dates.begin();
  auto transactions_it = transactions.begin();

  double balance = info.sum;
  std::string prev_date = info.date;
  double cumulated_interest = 0.0;

  auto process_interests = [&]() {
    std::string current_date = *interests_it;
    double interest =
        cumulated_interest +
        CalculateInterest(prev_date, current_date, info.rate, balance);
    cumulated_interest = 0.0;
    ++interests_it;
    return std::make_pair(current_date, interest);
  };

  auto process_transactions = [&]() {
    std::string current_date = transactions_it->first;
    double transaction = transactions_it->second;
    cumulated_interest +=
        CalculateInterest(prev_date, current_date, info.rate, balance);
    ++transactions_it;
    return std::make_pair(current_date, transaction);
  };

  while (interests_it != interest_dates.end() ||
         transactions_it != transactions.end()) {
    std::string current_date;
    double interest = 0.0;
    double transaction = 0.0;

    if (interests_it != interest_dates.end() &&
        transactions_it != transactions.end()) {
      if (CompareDates(*interests_it, transactions_it->first) < 0) {
        std::tie(current_date, interest) = process_interests();
      } else {
        std::tie(current_date, transaction) = process_transactions();
      }
    } else if (interests_it != interest_dates.end()) {
      std::tie(current_date, interest) = process_interests();
    } else {
      std::tie(current_date, transaction) = process_transactions();
    }

    plan.dates.push_back(current_date);
    plan.interests.push_back(interest);
    plan.transactions.push_back(transaction);
    if (info.capitalize) {
      balance += interest + transaction;
    } else {
      balance += transaction;
    }
    plan.balances.push_back(balance);
    prev_date = current_date;
  }
  plan.transactions[0] += info.sum;
  plan.tax_info = CalculateTax(plan, info);

  return plan;
}

/**
 * @brief Generates a vector of interest payment dates based on the provided
 * DepositInfo.
 *
 * This function generates interest payment dates according to the specified
 * payment period in the DepositInfo. The generated dates include the initial
 * date, accrued interest dates, and the maturity date.
 *
 * @param info The deposit information including initial date, term, and payment
 * period details.
 * @return A vector of strings representing the interest payment dates.
 *
 * The function considers various payment periods such as daily, weekly,
 * monthly, quarterly, semi-annually, annually, and at maturity. The generated
 * dates are inclusive of the initial and maturity dates.
 */
std::vector<std::string> DepositCalc::GenerateInterestDates(
    const DepositInfo& info) {
  std::vector<std::string> interest_dates;
  std::string date = info.date;

  auto generate_daily = [&](int step) {
    int days = TermToDays(info.date, info.term);
    for (int delta = 0; delta < days; delta += step) {
      interest_dates.push_back(AddDays(info.date, delta));
    }
  };

  auto generate_monthly = [&](int step) {
    for (int delta = 0; delta < info.term; delta += step) {
      interest_dates.push_back(date);
      date = AddMonths(date, step);
    }
  };

  if (info.period == PaymentPeriod::kAtMaturity) {
    interest_dates.push_back(date);
  } else if (info.period == PaymentPeriod::kDaily) {
    generate_daily(1);
  } else if (info.period == PaymentPeriod::kWeekly) {
    generate_daily(7);
  } else if (info.period == PaymentPeriod::kMonthly) {
    generate_monthly(1);
  } else if (info.period == PaymentPeriod::kQuarterly) {
    generate_monthly(3);
  } else if (info.period == PaymentPeriod::kSemiAnnually) {
    generate_monthly(6);
  } else if (info.period == PaymentPeriod::kAnnually) {
    generate_monthly(12);
  }

  interest_dates.push_back(AddMonths(info.date, info.term));
  return interest_dates;
}

/**
 * @brief Generates transactions map based on the provided DepositInfo.
 *
 * This function generates a map of transactions where keys represent
 * transaction dates and values represent the corresponding transaction amounts.
 * Transactions include both replenishments and withdrawals, adjusted based on
 * the specified regularity and term in the DepositInfo.
 *
 * @param info The deposit information including initial date, term, and
 * transaction details.
 * @return A map with transaction dates as keys and transaction amounts as
 * values.
 *
 * The function considers various regularities such as one-time, monthly,
 * bi-monthly, quarterly, semi-annually, and annually. Transactions are
 * generated based on the specified regularity and term in the DepositInfo.
 */
std::map<std::string, double, DepositCalc::DatesComparator>
DepositCalc::GenerateTransactions(const DepositInfo& info) {
  std::map<std::string, double, DatesComparator> transactions_map;

  auto get_step = [&info](Regularity regularity) {
    switch (regularity) {
      case Regularity::kOneTime:
        return info.term;
      case Regularity::kMonthly:
        return 1;
      case Regularity::kBiMonthly:
        return 2;
      case Regularity::kQuarterly:
        return 3;
      case Regularity::kSemiAnnually:
        return 6;
      case Regularity::kAnnually:
        return 12;
      default:
        return 1;
    }
  };

  auto process_transactions =
      [&](const std::vector<Transaction>& transaction_list, double sign) {
        for (const auto& transaction : transaction_list) {
          std::string date = transaction.date;
          int step = get_step(transaction.regularity);
          while (CompareDates(date, AddMonths(info.date, info.term)) <= 0) {
            transactions_map[date] += sign * transaction.sum;
            date = AddMonths(date, step, transaction.date);
          }
        }
      };

  process_transactions(info.replenishments, 1.0);
  process_transactions(info.withdrawals, -1.0);

  return transactions_map;
}

/**
 * @brief Calculates the total interest accrued between two dates based on the
 * given interest rate and initial balance.
 *
 * This function calculates the total interest accrued on a balance between two
 * specified dates using a provided interest rate. The interest is computed
 * based on the actual number of days in the interval.
 *
 * @param date1 The start date of the interest calculation period.
 * @param date2 The end date of the interest calculation period.
 * @param rate The annual interest rate in percentage.
 * @param balance The initial balance on which interest is calculated.
 * @return The total interest accrued between date1 and date2, rounded to two
 * decimal places.
 *
 * The function utilizes the actual number of days in the interval, considering
 * leap years and varying month lengths.
 */
double DepositCalc::CalculateInterest(const std::string& date1,
                                      const std::string& date2, double rate,
                                      double balance) {
  double total_interest = 0.0;
  std::string current_date = date1;

  while (CompareDates(current_date, date2) < 0) {
    std::string next_date = FindNextYear(current_date);
    if (CompareDates(next_date, date2) >= 0) {
      next_date = date2;
    }

    int days_in_interval = DaysBetweenDates(current_date, next_date);
    total_interest +=
        balance * days_in_interval * rate / 100 / DaysInYear(current_date);
    current_date = next_date;
  }

  return std::round(total_interest * 100) / 100;
}

/**
 * @brief Adds a specified number of days to the given date and returns the
 * resulting date in the format "%d-%m-%Y".
 *
 * This function takes a date in the format "%d-%m-%Y" and adds the specified
 * number of days to it, returning the resulting date as a string in the same
 * format. The input date is parsed, and the calculated date is formatted using
 * the same format. If the input date format is invalid, an exception is thrown.
 *
 * @param date The input date in the format "%d-%m-%Y".
 * @param days The number of days to be added to the input date.
 * @return The resulting date after adding the specified number of days in the
 * format "%d-%m-%Y".
 * @throws std::invalid_argument If the input date format is invalid.
 */
std::string DepositCalc::AddDays(const std::string& date, int days) {
  std::tm tm = {};
  std::istringstream ss(date);
  ss >> std::get_time(&tm, "%d-%m-%Y");
  if (ss.fail()) {
    throw std::invalid_argument("Invalid date format");
  }

  auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  tp += std::chrono::hours(24 * days);
  std::time_t new_time = std::chrono::system_clock::to_time_t(tp);
  std::tm* new_tm = std::localtime(&new_time);

  std::ostringstream oss;
  oss << std::put_time(new_tm, "%d-%m-%Y");
  return oss.str();
}

/**
 * @brief Adds a specified number of months to the given date and returns the
 * resulting date in the format "%d-%m-%Y".
 *
 * This function takes a date in the format "%d-%m-%Y" and adds the specified
 * number of months to it, returning the resulting date as a string in the same
 * format. The input date is parsed, and the calculated date is formatted using
 * the same format. If the input date format is invalid, the behavior is
 * undefined.
 *
 * @param date The input date in the format "%d-%m-%Y".
 * @param months The number of months to be added to the input date.
 * @return The resulting date after adding the specified number of months in the
 * format "%d-%m-%Y".
 */
std::string DepositCalc::AddMonths(const std::string& date, int months) {
  std::tm tm = StringToDate(date);

  tm.tm_mon += months;
  if (tm.tm_mon > 11) {
    tm.tm_year += tm.tm_mon / 12;
    tm.tm_mon %= 12;
  }

  std::time_t new_time = std::mktime(&tm);
  std::tm* new_tm = std::localtime(&new_time);

  std::ostringstream oss;
  oss << std::put_time(new_tm, "%d-%m-%Y");
  return oss.str();
}

/**
 * @brief Adds a specified number of months to the given date and returns the
 * resulting date in the format "%d-%m-%Y", considering a specified start date.
 *
 * This function takes a date in the format "%d-%m-%Y" and adds the specified
 * number of months to it, considering a start date. If the day component of the
 * start date is valid (within the month), it is used as a reference for the day
 * component of the resulting date. If not, the last day of the resulting month
 * is used. The calculated date is then formatted as a string in the format
 * "%d-%m-%Y". If the input date or start date format is invalid, the behavior
 * is undefined.
 *
 * @param date The input date in the format "%d-%m-%Y".
 * @param months The number of months to be added to the input date.
 * @param start_date The reference start date in the format "%d-%m-%Y".
 * @return The resulting date after adding the specified number of months in the
 * format "%d-%m-%Y".
 */
std::string DepositCalc::AddMonths(const std::string& date, int months,
                                   const std::string& start_date) {
  std::tm tm = StringToDate(date);
  std::tm start_tm = StringToDate(start_date);

  tm.tm_mon += months;
  if (tm.tm_mon > 11) {
    tm.tm_year += tm.tm_mon / 12;
    tm.tm_mon %= 12;
  }

  if (start_tm.tm_mday > 0 && start_tm.tm_mday <= DaysInMonth(start_tm)) {
    tm.tm_mday = std::min(start_tm.tm_mday, DaysInMonth(tm));
  } else {
    tm.tm_mday = DaysInMonth(tm);
  }

  std::time_t new_time = std::mktime(&tm);
  std::tm* new_tm = std::localtime(&new_time);

  std::ostringstream oss;
  oss << std::put_time(new_tm, "%d-%m-%Y");
  return oss.str();
}

/**
 * @brief Calculates the number of days in the specified month considering leap
 * years.
 *
 * This function takes a std::tm structure representing a specific month and
 * year and calculates the number of days in that month, considering leap years
 * for February. It returns the total number of days in the specified month.
 *
 * @param time A std::tm structure representing the month and year.
 * @return The number of days in the specified month, considering leap years for
 * February.
 */
int DepositCalc::DaysInMonth(const std::tm& time) {
  static const std::vector<int> days_in_month = {31, 28, 31, 30, 31, 30,
                                                 31, 31, 30, 31, 30, 31};
  int month = time.tm_mon;

  int days = days_in_month[month];
  if (month == 1 && IsLeapYear(time.tm_year + 1900)) {
    days = 29;
  }

  return days;
}

/**
 * @brief Parses the input date string and converts it to a std::tm structure.
 *
 * This function takes a date string in the format "%d-%m-%Y" and parses it to
 * create a std::tm structure representing the date. If the input string has an
 * invalid format, it throws a std::runtime_error.
 *
 * @param date The input date string in the format "%d-%m-%Y".
 * @return A std::tm structure representing the parsed date.
 * @throws std::runtime_error If the input date format is invalid.
 */
std::tm DepositCalc::StringToDate(const std::string& date) {
  std::istringstream ss(date);
  std::tm tm = {};
  ss >> std::get_time(&tm, "%d-%m-%Y");
  if (ss.fail()) {
    throw std::runtime_error("Invalid date format");
  }
  return tm;
}

/**
 * @brief Converts a given term in months to the corresponding number of days.
 *
 * This function calculates the number of days between the input date and the
 * date after adding the specified term in months.
 *
 * @param date The input date string in the format "%d-%m-%Y".
 * @param term The term in months to be converted to days.
 * @return The number of days corresponding to the given term in months.
 */
int DepositCalc::TermToDays(const std::string& date, int term) {
  auto date1 = StringToDate(date);
  auto date2 = StringToDate(AddMonths(date, term));

  std::time_t time1 = std::mktime(&date1);
  std::time_t time2 = std::mktime(&date2);
  int days = std::difftime(time2, time1) / kSecondsPerDay;

  return days;
}

/**
 * @brief Compares two date strings in the format "%d-%m-%Y".
 *
 * This function compares two date strings to determine their order.
 *
 * @param date1 The first date string to be compared.
 * @param date2 The second date string to be compared.
 * @return -1 if date1 is earlier than date2, 1 if date1 is later than date2,
 * and 0 if they are equal.
 */
int DepositCalc::CompareDates(const std::string& date1,
                              const std::string& date2) {
  std::tm tm1 = {};
  std::tm tm2 = {};

  std::istringstream ss1(date1);
  ss1 >> std::get_time(&tm1, "%d-%m-%Y");

  std::istringstream ss2(date2);
  ss2 >> std::get_time(&tm2, "%d-%m-%Y");

  if (ss1.fail() || ss2.fail()) {
    throw std::runtime_error("Invalid date format");
  }

  if (std::mktime(&tm1) < std::mktime(&tm2)) {
    return -1;
  } else if (std::mktime(&tm1) > std::mktime(&tm2)) {
    return 1;
  } else {
    return 0;
  }
}

/**
 * @brief Calculates the number of days between two date strings in the format
 * "%d-%m-%Y".
 *
 * This function calculates the number of days between two given date strings.
 *
 * @param date1 The first date string.
 * @param date2 The second date string.
 * @return The number of days between date1 and date2.
 * @throws std::runtime_error If the conversion fails for any date string.
 */
int DepositCalc::DaysBetweenDates(const std::string& date1,
                                  const std::string& date2) {
  std::tm tm1 = StringToDate(date1);
  std::tm tm2 = StringToDate(date2);

  std::time_t time1 = std::mktime(&tm1);
  std::time_t time2 = std::mktime(&tm2);

  if (time1 == -1 || time2 == -1) {
    throw std::runtime_error("Invalid date format");
  }

  int days = std::difftime(time2, time1) / kSecondsPerDay;

  return days;
}

/**
 * @brief Calculates the number of days in the year of the given date string in
 * the format "%d-%m-%Y".
 *
 * This function calculates the number of days in the year of the given date
 * string.
 *
 * @param date The date string in the format "%d-%m-%Y".
 * @return The number of days in the year of the given date.
 */
int DepositCalc::DaysInYear(const std::string& date) {
  std::tm tm = StringToDate(date);
  if (tm.tm_mon == 11 && tm.tm_mday == 31) tm.tm_year += 1;
  int year = tm.tm_year + 1900;

  return IsLeapYear(year) ? 366 : 365;
}

/**
 * @brief Checks if the given year is a leap year.
 *
 * This function determines whether a given year is a leap year.
 *
 * @param year The year to be checked for leap year status.
 * @return true if the year is a leap year, false otherwise.
 */
bool DepositCalc::IsLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * @brief Finds the date corresponding to the end of the year from the given
 * date.
 *
 * This function takes a date in the format "dd-mm-yyyy" and calculates the date
 * corresponding to the end of the year (31st December).
 *
 * @param date The input date in the format "dd-mm-yyyy".
 * @return A string representing the date at the end of the next year in the
 * format "dd-mm-yyyy".
 */
std::string DepositCalc::FindNextYear(const std::string& date) {
  std::tm tm = StringToDate(date);
  if (tm.tm_mon == 11 && tm.tm_mday == 31) tm.tm_year += 1;
  tm.tm_mon = 11;
  tm.tm_mday = 31;

  std::time_t new_time = std::mktime(&tm);
  std::tm* new_tm = std::localtime(&new_time);

  std::ostringstream oss;
  oss << std::put_time(new_tm, "%d-%m-%Y");
  return oss.str();
}

/**
 * @brief Extracts the year from a date string in the format "dd-mm-yyyy".
 *
 * @param date The date string in the format "dd-mm-yyyy".
 * @return std::string The extracted year as a string.
 * @throws std::runtime_error If the input date format is invalid.
 */
std::string DepositCalc::ExtractYear(const std::string& date) {
  std::istringstream ss(date);
  std::tm tm = {};
  ss >> std::get_time(&tm, "%d-%m-%Y");
  if (ss.fail()) {
    throw std::runtime_error("Invalid date format");
  }
  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y");
  return oss.str();
}

/**
 * @brief Calculates the tax information for each year within the payment plan
 * period.
 *
 * This function takes the payment plan and calculates the tax information for
 * each year within the payment period. It considers the income, deductions, tax
 * amount, and the date of the final payment for each year.
 *
 * @param plan The payment plan containing dates, interest accrued, transaction
 * amounts, and balances.
 * @return std::vector<TaxInfo> A vector of TaxInfo structures, each
 * representing the tax information for a specific year.
 */
std::vector<DepositCalc::TaxInfo> DepositCalc::CalculateTax(
    const PaymentPlan& plan, const DepositInfo& info) {
  std::vector<TaxInfo> tax_info;
  if (plan.dates.empty()) {
    return tax_info;
  }

  double income = 0.0;
  std::string current_year = ExtractYear(plan.dates.front());

  for (std::size_t i = 0; i < plan.dates.size(); ++i) {
    std::string year = ExtractYear(plan.dates[i]);
    if (current_year == year) {
      income += plan.interests[i];
    } else {
      if (income > 0) {
        TaxInfo tax;
        tax.year = current_year;
        tax.income = income;
        tax.deduction = kTaxDeduction;
        tax.deduction_income = std::max(0.0, tax.income - tax.deduction);
        tax.tax_sum = std::round(tax.deduction_income * info.tax_rate) / 100;
        tax.pay_before = tax.tax_sum > 0 ? "1 December " + year : "";
        tax_info.push_back(tax);
      }

      income = 0.0;
      current_year = year;
    }
  }
  TaxInfo tax;
  tax.year = current_year;
  tax.income = income;
  tax.deduction = kTaxDeduction;
  tax.deduction_income = std::max(0.0, tax.income - tax.deduction);
  tax.tax_sum = std::round(tax.deduction_income * info.tax_rate) / 100;
  tax.pay_before =
      tax.tax_sum > 0
          ? "1 December " + std::to_string(std::stoi(current_year) + 1)
          : "";
  tax_info.push_back(tax);

  return tax_info;
}

/**
 * @brief Converts the payment plan data into a formatted string representation.
 *
 * This function takes the payment plan information and the deposit information
 * and converts it into a formatted string containing details such as date,
 * interest accrued, balance change, payout, and balance at each step. The total
 * interest and the final balance are also included in the output string.
 *
 * @param plan The payment plan containing dates, interest accrued, transaction
 * amounts, and balances.
 * @param info The deposit information including the initial date, sum, rate,
 * term, and period.
 * @return A formatted string representing the payment plan with detailed
 * information.
 */
std::string DepositCalc::PlanToString(const PaymentPlan& plan,
                                      const DepositInfo& info) {
  std::ostringstream oss;
  oss << std::setw(15) << std::left << "Date" << std::setw(20) << std::left
      << "Interest accrued" << std::setw(25) << std::left << "Balance change"
      << std::setw(20) << std::left << "Payout" << std::setw(20) << std::left
      << "Balance" << std::endl;

  for (std::size_t i = 0; i < plan.dates.size(); ++i) {
    oss << std::setw(15) << std::left << plan.dates[i] << std::fixed
        << std::setprecision(2) << std::setw(20) << std::left
        << plan.interests[i];

    double balance_change = info.capitalize
                                ? (plan.transactions[i] + plan.interests[i])
                                : plan.transactions[i];

    oss << std::setw(25) << std::left << balance_change << std::setw(20)
        << std::left << plan.interests[i] << std::setw(20) << std::left
        << plan.balances[i] << std::endl;
  }

  double total_interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);

  oss << std::setw(15) << std::left << "Total" << std::fixed
      << std::setprecision(2) << std::setw(20) << std::left << total_interest
      << std::setw(25) << std::left << "-" << std::setw(20) << std::left << "-"
      << std::setw(20) << std::left << plan.balances.back() << std::endl;

  return oss.str();
}

/**
 * @brief Converts tax information to a formatted string.
 *
 * This function takes a vector of TaxInfo structures and converts the tax
 * information to a formatted string with columns: Year, Income, Deduction,
 * Income after deduction, Tax amount, and Pay before.
 *
 * @param tax_info Vector of TaxInfo structures containing tax information.
 * @return std::string Formatted string with tax information in columns.
 */
std::string DepositCalc::TaxToString(const std::vector<TaxInfo>& tax_info) {
  std::ostringstream oss;
  oss << std::setw(10) << std::left << "Year" << std::setw(15) << std::left
      << "Income" << std::setw(15) << std::left << "Deduction" << std::setw(25)
      << std::left << "Income after deduction" << std::setw(15) << std::left
      << "Tax amount" << std::setw(20) << std::left << "Pay before"
      << std::endl;

  for (const auto& tax : tax_info) {
    oss << std::setw(10) << std::left << tax.year << std::fixed
        << std::setprecision(2) << std::setw(15) << std::left << tax.income
        << std::fixed << std::setprecision(2) << std::setw(15) << std::left
        << tax.deduction << std::fixed << std::setprecision(2) << std::setw(25)
        << std::left << tax.deduction_income << std::fixed
        << std::setprecision(2) << std::setw(15) << std::left << tax.tax_sum
        << std::setw(20) << std::left << tax.pay_before << std::endl;
  }

  return oss.str();
}

}  // namespace s21
