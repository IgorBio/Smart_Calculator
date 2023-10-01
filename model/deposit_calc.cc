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
DepositCalc::PaymentPlan DepositCalc::Calculate(const DepositInfo& info) {
  PaymentPlan plan;
  std::vector<std::string> interest_dates = GenerateInterestDates(info);
  std::vector<std::pair<std::string, double>> transactions =
      GenerateTransactions(info);

  double balance = info.sum;
  for (size_t i = 0; i < plan.dates.size(); ++i) {
    //     double interest =
    //         CalculateInterest(balance, info.rate, info.tax_rate,
    //         info.period);
    //     double transaction = CalculateTransactions(info, plan.dates[i]);

    //     balance += interest + transaction;

    //     TaxInfo taxInfo;
    //     taxInfo.year = GetYear(plan.dates[i]);
    //     taxInfo.income = interest;
    //     taxInfo.deduction = info.deduction;
    //     taxInfo.income_after_deduction =
    //         std::max(0.0, taxInfo.income - taxInfo.deduction);
    //     taxInfo.tax_amount =
    //         CalculateTaxAmount(taxInfo.income_after_deduction, kKeyRate);
    //     taxInfo.pay_before = "01-12-" + taxInfo.year;

    //     plan.interests.push_back(interest);
    //     plan.transactions.push_back(transaction);
    //     plan.balances.push_back(balance);
    //     plan.tax_info.push_back(taxInfo);
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

  for (auto& replenish : info.replenishments) {
    if (replenish.regularity == Regularity::kOneTime) {
      transactions.emplace_back(replenish.date, replenish.sum);
    } else if (replenish.regularity == Regularity::kMonthly) {
      for (int delta = 0; delta <= info.term; ++delta) {
        transactions.emplace_back(AddMonths(replenish.date, delta),
                                  replenish.sum);
      }
    } else if (replenish.regularity == Regularity::kBiMonthly) {
      for (int delta = 0; delta <= info.term; delta += 2) {
        transactions.emplace_back(AddMonths(replenish.date, delta),
                                  replenish.sum);
      }
    } else if (replenish.regularity == Regularity::kQuarterly) {
      for (int delta = 0; delta <= info.term; delta += 3) {
        transactions.emplace_back(AddMonths(replenish.date, delta),
                                  replenish.sum);
      }
    } else if (replenish.regularity == Regularity::kSemiAnnually) {
      for (int delta = 0; delta <= info.term; delta += 6) {
        transactions.emplace_back(AddMonths(replenish.date, delta),
                                  replenish.sum);
      }
    } else if (replenish.regularity == Regularity::kAnnually) {
      for (int delta = 0; delta <= info.term; delta += 12) {
        transactions.emplace_back(AddMonths(replenish.date, delta),
                                  replenish.sum);
      }
    }
  }

  for (auto& withdrawal : info.withdrawals) {
    if (withdrawal.regularity == Regularity::kOneTime) {
      transactions.emplace_back(withdrawal.date, -withdrawal.sum);
    } else if (withdrawal.regularity == Regularity::kMonthly) {
      for (int delta = 0; delta <= info.term; ++delta) {
        transactions.emplace_back(AddMonths(withdrawal.date, delta),
                                  -withdrawal.sum);
      }
    } else if (withdrawal.regularity == Regularity::kBiMonthly) {
      for (int delta = 0; delta <= info.term; delta += 2) {
        transactions.emplace_back(AddMonths(withdrawal.date, delta),
                                  -withdrawal.sum);
      }
    } else if (withdrawal.regularity == Regularity::kQuarterly) {
      for (int delta = 0; delta <= info.term; delta += 3) {
        transactions.emplace_back(AddMonths(withdrawal.date, delta),
                                  -withdrawal.sum);
      }
    } else if (withdrawal.regularity == Regularity::kSemiAnnually) {
      for (int delta = 0; delta <= info.term; delta += 6) {
        transactions.emplace_back(AddMonths(withdrawal.date, delta),
                                  -withdrawal.sum);
      }
    } else if (withdrawal.regularity == Regularity::kAnnually) {
      for (int delta = 0; delta <= info.term; delta += 12) {
        transactions.emplace_back(AddMonths(withdrawal.date, delta),
                                  -withdrawal.sum);
      }
    }
  }
  return transactions;
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
  auto start_date = StringToDate(date);
  auto end_date = StringToDate(AddMonths(date, term));

  std::time_t time1 = std::mktime(&start_date);
  std::time_t time2 = std::mktime(&end_date);
  constexpr int kSecondsInDay = 24 * 60 * 60;
  int days = std::difftime(time2, time1) / kSecondsInDay;

  return days;
}

}  // namespace s21
