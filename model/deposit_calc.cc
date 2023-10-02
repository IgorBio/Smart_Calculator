#include "deposit_calc.h"

namespace s21 {

/**
 * @brief Calculate deposit payment plan based on the provided deposit
 * parameters.
 *
 * This method calculates the deposit payment plan, including earned interests,
 * remaining balances, and tax amount, based on the provided deposit parameters.
 *
 * @param info The deposit parameters including sum, term, date, rate, tax
 * rate, payment period, capitalization, replenishments, and withdrawals.
 * @return A PaymentPlan structure containing payment plan details.
 */
// DepositCalc::PaymentPlan DepositCalc::Calculate(const DepositInfo& info) {
//   PaymentPlan plan;
//   std::vector<std::string> interest_dates = GenerateInterestDates(info);
//   std::vector<std::pair<std::string, double>> transactions =
//       GenerateTransactions(info);

//   plan.dates.push_back(info.date);
//   plan.balances.push_back(info.sum);
//   auto interests_it = interest_dates.begin();
//   auto transactions_it = transactions.begin();

//   double balance = info.sum;

//   while (interests_it != interest_dates.end() &&
//          transactions_it != transactions.end()) {
//     std::string interest_date = *interests_it;
//     std::pair<std::string, double> transaction = *transactions_it;

//     if (CompareDates(interest_date, transaction.first) <= 0) {
//       plan.dates.push_back(interest_date);
//       double interest = CalculateInterest(plan.dates.back(), interest_date,
//                                           info.rate, balance);
//       plan.interests.push_back(interest);
//       balance += interest;
//       plan.transactions.push_back(0.0);
//       plan.balances.push_back(balance);
//       ++interests_it;
//     } else {
//       plan.dates.push_back(transaction.first);
//       plan.interests.push_back(0.0);
//       plan.transactions.push_back(transaction.second);
//       balance += transaction.second;
//       plan.balances.push_back(balance);
//       ++transactions_it;
//     }
//   }

//   while (interests_it != interest_dates.end()) {
//     plan.dates.push_back(*interests_it);
//     double interest =
//         CalculateInterest(plan.dates.back(), *interests_it, info.rate,
//         balance);
//     plan.interests.push_back(interest);
//     balance += interest;
//     plan.transactions.push_back(0.0);
//     plan.balances.push_back(balance);
//     ++interests_it;
//   }

//   while (transactions_it != transactions.end()) {
//     plan.dates.push_back(transactions_it->first);
//     plan.interests.push_back(0.0);
//     plan.transactions.push_back(transactions_it->second);
//     balance += transactions_it->second;
//     plan.balances.push_back(balance);
//     ++transactions_it;
//   }

//   return plan;
// }

DepositCalc::PaymentPlan DepositCalc::Calculate(const DepositInfo& info) {
  PaymentPlan plan;
  std::vector<std::string> interest_dates = GenerateInterestDates(info);
  std::vector<std::pair<std::string, double>> transactions =
      GenerateTransactions(info);

  auto interests_it = interest_dates.begin();
  auto transactions_it = transactions.begin();

  double balance = info.sum;
  std::string prev_date = info.date;

  while (interests_it != interest_dates.end() ||
         transactions_it != transactions.end()) {
    std::string current_date;
    double interest = 0.0;
    double transaction = 0.0;

    if (interests_it != interest_dates.end() &&
        transactions_it != transactions.end()) {
      if (CompareDates(*interests_it, transactions_it->first) <= 0) {
        current_date = *interests_it;
        interest =
            CalculateInterest(prev_date, current_date, info.rate, balance);
        ++interests_it;
      } else {
        current_date = transactions_it->first;
        transaction = transactions_it->second;
        ++transactions_it;
      }
    } else if (interests_it != interest_dates.end()) {
      current_date = *interests_it;
      interest = CalculateInterest(prev_date, current_date, info.rate, balance);
      ++interests_it;
    } else {
      current_date = transactions_it->first;
      transaction = transactions_it->second;
      ++transactions_it;
    }

    plan.dates.push_back(current_date);
    plan.interests.push_back(interest);
    plan.transactions.push_back(transaction);
    balance += interest + transaction;
    plan.balances.push_back(balance);
    prev_date = current_date;
  }

  return plan;
}

std::vector<std::string> DepositCalc::GenerateInterestDates(
    const DepositInfo& info) {
  std::vector<std::string> interest_dates;

  if (info.period == PaymentPeriod::kAtMaturity) {
    interest_dates.push_back(info.date);
  } else if (info.period == PaymentPeriod::kDaily) {
    int days = TermToDays(info.date, info.term);
    for (int delta = 0; delta < days; ++delta) {
      interest_dates.push_back(AddDays(info.date, delta));
    }
  } else if (info.period == PaymentPeriod::kWeekly) {
    int days = TermToDays(info.date, info.term);
    for (int delta = 0; delta < days; delta += 7) {
      interest_dates.push_back(AddDays(info.date, delta));
    }
  } else if (info.period == PaymentPeriod::kMonthly) {
    for (int delta = 0; delta < info.term; ++delta) {
      interest_dates.push_back(AddMonths(info.date, delta));
    }
  } else if (info.period == PaymentPeriod::kQuarterly) {
    for (int delta = 0; delta < info.term; delta += 3) {
      interest_dates.push_back(AddMonths(info.date, delta));
    }
  } else if (info.period == PaymentPeriod::kSemiAnnually) {
    for (int delta = 0; delta < info.term; delta += 6) {
      interest_dates.push_back(AddMonths(info.date, delta));
    }
  } else if (info.period == PaymentPeriod::kAnnually) {
    for (int delta = 0; delta < info.term; delta += 12) {
      interest_dates.push_back(AddMonths(info.date, delta));
    }
  }
  interest_dates.push_back(AddMonths(info.date, info.term));

  return interest_dates;
}

std::vector<std::pair<std::string, double>> DepositCalc::GenerateTransactions(
    const DepositInfo& info) {
  std::vector<std::pair<std::string, double>> transactions;

  auto process_transactions =
      [&](const std::vector<Transaction>& transaction_list, double sign) {
        for (const auto& transaction : transaction_list) {
          int step = 1;
          if (transaction.regularity == Regularity::kOneTime) {
            step = info.term + 1;
          } else if (transaction.regularity == Regularity::kMonthly) {
            step = 1;
          } else if (transaction.regularity == Regularity::kBiMonthly) {
            step = 2;
          } else if (transaction.regularity == Regularity::kQuarterly) {
            step = 3;
          } else if (transaction.regularity == Regularity::kSemiAnnually) {
            step = 6;
          } else if (transaction.regularity == Regularity::kAnnually) {
            step = 12;
          }

          for (int delta = 0; delta <= info.term; delta += step) {
            transactions.emplace_back(AddMonths(transaction.date, delta),
                                      sign * transaction.sum);
          }
        }
      };

  process_transactions(info.replenishments, 1.0);
  process_transactions(info.withdrawals, -1.0);

  return transactions;
}

double DepositCalc::CalculateInterest(const std::string& date1,
                                      const std::string& date2, double rate,
                                      double balance) {
  double total_interest = 0.0;
  std::string current_date = date1;

  while (CompareDates(current_date, date2) <= 0) {
    std::string next_date = FindNextYear(current_date);
    if (CompareDates(next_date, date2) == 1) {
      next_date = date2;
    }

    int days_in_interval = DaysBetweenDates(current_date, next_date);
    total_interest += balance * days_in_interval * rate / 100 /
                      DaysInCurrentYear(current_date);

    current_date = FindNextDay(next_date);
  }

  return std::round(total_interest * 100) / 100;
}

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

std::tm DepositCalc::StringToDate(const std::string& date) {
  std::istringstream ss(date);
  std::tm tm = {};
  ss >> std::get_time(&tm, "%d-%m-%Y");
  if (ss.fail()) {
    throw std::runtime_error("Invalid date format");
  }
  return tm;
}

int DepositCalc::TermToDays(const std::string& date, int term) {
  auto date1 = StringToDate(date);
  auto date2 = StringToDate(AddMonths(date, term));

  std::time_t time1 = std::mktime(&date1);
  std::time_t time2 = std::mktime(&date2);
  int days = std::difftime(time2, time1) / kSecondsPerDay;

  return days;
}

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

int DepositCalc::DaysInCurrentYear(const std::string& date) {
  std::tm tm = StringToDate(date);
  int year = tm.tm_year + 1900;

  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
    return 366;
  } else {
    return 365;
  }
}

std::string DepositCalc::FindNextYear(const std::string& date) {
  std::tm tm = StringToDate(date);
  tm.tm_year += 1;
  tm.tm_mon = 0;
  tm.tm_mday = 1;

  std::time_t new_time = std::mktime(&tm);
  std::tm* new_tm = std::localtime(&new_time);

  std::ostringstream oss;
  oss << std::put_time(new_tm, "%d-%m-%Y");
  return oss.str();
}

std::string DepositCalc::FindNextDay(const std::string& date) {
  std::tm tm = StringToDate(date);
  tm.tm_mday += 1;

  std::time_t new_time = std::mktime(&tm);
  std::tm* new_tm = std::localtime(&new_time);

  std::ostringstream oss;
  oss << std::put_time(new_tm, "%d-%m-%Y");
  return oss.str();
}

}  // namespace s21
