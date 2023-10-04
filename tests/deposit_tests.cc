#include <gtest/gtest.h>

#include <iostream>

#include "deposit_calc.h"

using namespace s21;

TEST(DepositCalcTest, OneMonth) {
  DepositCalc::DepositInfo info{
      870000.00, 1,  "02-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kMonthly,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "02-10-2023");
  EXPECT_EQ(plan.dates.back(), "02-11-2023");
  EXPECT_EQ(plan.dates.size(), 2);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 6650.14, 1e-2);
  EXPECT_EQ(plan.interests.size(), 2);
  EXPECT_NEAR(interest, 6650.14, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 2);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 876650.14, 1e-2);
  EXPECT_EQ(plan.balances.size(), 2);
}

TEST(DepositCalcTest, TwoMonths) {
  DepositCalc::DepositInfo info{
      870000.00, 2,  "03-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kMonthly,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "03-10-2023");
  EXPECT_EQ(plan.dates.back(), "03-12-2023");
  EXPECT_EQ(plan.dates.size(), 3);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 6484.81, 1e-2);
  EXPECT_EQ(plan.interests.size(), 3);
  EXPECT_NEAR(interest, 13134.95, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 3);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 883134.95, 1e-2);
  EXPECT_EQ(plan.balances.size(), 3);
}

TEST(DepositCalcTest, TwentyMonths) {
  DepositCalc::DepositInfo info{
      870000.00, 12, "03-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kMonthly,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "03-10-2023");
  EXPECT_EQ(plan.dates.back(), "03-10-2024");
  EXPECT_EQ(plan.dates.size(), 13);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 6969.09, 1e-2);
  EXPECT_EQ(plan.interests.size(), 13);
  EXPECT_NEAR(interest, 81668.49, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 13);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 951668.49, 1e-2);
  EXPECT_EQ(plan.balances.size(), 13);
}

TEST(DepositCalcTest, SixtyMonths) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "31-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kMonthly,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "31-10-2023");
  EXPECT_EQ(plan.dates.back(), "31-10-2028");
  EXPECT_EQ(plan.dates.size(), 61);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 9975.40, 1e-2);
  EXPECT_EQ(plan.interests.size(), 61);
  EXPECT_NEAR(interest, 492196.57, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 61);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 1362196.57, 1e-2);
  EXPECT_EQ(plan.balances.size(), 61);
}

TEST(DepositCalcTest, QuarterlyPayments) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "30-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kQuarterly,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 21);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 30035.64, 1e-2);
  EXPECT_EQ(plan.interests.size(), 21);
  EXPECT_NEAR(interest, 487698.07, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 21);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 1357698.07, 1e-2);
  EXPECT_EQ(plan.balances.size(), 21);
}

TEST(DepositCalcTest, SemiannuallyPayments) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "30-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kSemiAnnually,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 11);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 58182.95, 1e-2);
  EXPECT_EQ(plan.interests.size(), 11);
  EXPECT_NEAR(interest, 481137.35, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 11);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 1351137.35, 1e-2);
  EXPECT_EQ(plan.balances.size(), 11);
}

TEST(DepositCalcTest, AnnuallyPayments) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "30-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kAnnually,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 6);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 110578.14, 1e-2);
  EXPECT_EQ(plan.interests.size(), 6);
  EXPECT_NEAR(interest, 468654.14, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 6);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 1338654.14, 1e-2);
  EXPECT_EQ(plan.balances.size(), 6);
}

TEST(DepositCalcTest, WeeklyPayments) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "30-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kWeekly,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 262);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 2343.77, 1e-2);
  EXPECT_EQ(plan.interests.size(), 262);
  EXPECT_NEAR(interest, 493959.90, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 262);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 1363959.90, 1e-2);
  EXPECT_EQ(plan.balances.size(), 262);
}

TEST(DepositCalcTest, DailyPayments) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "30-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kDaily,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 1828);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 335.43, 1e-2);
  EXPECT_EQ(plan.interests.size(), 1828);
  EXPECT_NEAR(interest, 494412.86, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 1828);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 1364412.86, 1e-2);
  EXPECT_EQ(plan.balances.size(), 1828);
}

TEST(DepositCalcTest, AtMaturity) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "30-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kAtMaturity,
      true,      {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 2);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 391536.34, 1e-2);
  EXPECT_EQ(plan.interests.size(), 2);
  EXPECT_NEAR(interest, 391536.34, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 2);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 1261536.34, 1e-2);
  EXPECT_EQ(plan.balances.size(), 2);
}

TEST(DepositCalcTest, NoCapitalize) {
  DepositCalc::DepositInfo info{
      870000.00, 60, "30-10-2023",
      9,         13, DepositCalc::PaymentPeriod::kDaily,
      false,     {}, {}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 1828);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 213.93, 1e-2);
  EXPECT_EQ(plan.interests.size(), 1828);
  EXPECT_NEAR(interest, 391532.74, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 1828);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 870000.00, 1e-2);
  EXPECT_EQ(plan.balances.size(), 1828);
}

TEST(DepositCalcTest, Transactions) {
  DepositCalc::DepositInfo info{
      870000.00,
      60,
      "30-10-2023",
      9,
      13,
      DepositCalc::PaymentPeriod::kMonthly,
      true,
      {DepositCalc::Transaction{DepositCalc::Regularity::kOneTime, "31-12-2023",
                                100000},
       DepositCalc::Transaction{DepositCalc::Regularity::kMonthly, "31-10-2023",
                                200000}},
      {DepositCalc::Transaction{DepositCalc::Regularity::kBiMonthly,
                                "29-02-2024", 150000}}};

  auto plan = DepositCalc::Calculate(info);
  double interest =
      std::accumulate(plan.interests.begin(), plan.interests.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "30-10-2023");
  EXPECT_EQ(plan.dates.back(), "30-10-2028");
  EXPECT_EQ(plan.dates.size(), 145);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 81088.51, 1e-2);
  EXPECT_EQ(plan.interests.size(), 145);
  EXPECT_NEAR(interest, 2687293.62, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 145);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 11307293.62, 1e-2);
  EXPECT_EQ(plan.balances.size(), 145);
}

TEST(DepositCalcTest, PlanToString) {
  DepositCalc::DepositInfo info{
      870000.00,
      12,
      "30-10-2023",
      9,
      13,
      DepositCalc::PaymentPeriod::kDaily,
      true,
      {DepositCalc::Transaction{DepositCalc::Regularity::kOneTime, "31-12-2023",
                                100000},
       DepositCalc::Transaction{DepositCalc::Regularity::kMonthly, "31-10-2023",
                                200000}},
      {DepositCalc::Transaction{DepositCalc::Regularity::kBiMonthly,
                                "29-02-2024", 150000}}};

  auto plan = DepositCalc::Calculate(info);

  // std::cout << DepositCalc::PlanToString(plan, info) << "\n";
}
