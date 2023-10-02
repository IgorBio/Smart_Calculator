#include <gtest/gtest.h>

#include <numeric>

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
  double transactions =
      std::accumulate(plan.transactions.begin(), plan.transactions.end(), 0.0);
  EXPECT_EQ(plan.dates.front(), "02-10-2023");
  EXPECT_EQ(plan.dates.back(), "02-11-2023");
  EXPECT_EQ(plan.dates.size(), 2);
  EXPECT_NEAR(plan.interests.front(), 0.0, 1e-2);
  EXPECT_NEAR(plan.interests.back(), 6650.14, 1e-2);
  EXPECT_EQ(plan.interests.size(), 2);
  EXPECT_NEAR(interest, 6650.14, 1e-2);
  EXPECT_NEAR(transactions, 0.0, 1e-2);
  EXPECT_EQ(plan.transactions.size(), 2);
  EXPECT_NEAR(plan.balances.front(), 870000.00, 1e-2);
  EXPECT_NEAR(plan.balances.back(), 876650.14, 1e-2);
  EXPECT_EQ(plan.balances.size(), 2);
}
