#include <gtest/gtest.h>

#include <numeric>

#include "credit_calc.h"

using namespace s21;

TEST(CreditCalcTest, AnnuityCredit) {
  double sum = 2800000.0;
  double rate = 5.0;
  int term = 60.0;

  auto [dates, payments, principals, interests, balances] =
      CreditCalc::Calculate(sum, rate, term, true);
  double total_payment = std::accumulate(payments.begin(), payments.end(), 0.0);
  double overpaiment = total_payment - sum;
  EXPECT_NEAR(payments[0], 52839.45, 1e-2);
  EXPECT_NEAR(payments[59], 52839.45, 1e-2);
  EXPECT_NEAR(principals[0], 41172.78, 1e-2);
  EXPECT_NEAR(principals[59], 52620.20, 1e-2);
  EXPECT_NEAR(interests[0], 11666.67, 1e-2);
  EXPECT_NEAR(interests[59], 219.25, 1e-2);
  EXPECT_NEAR(balances[0], 2758827.22, 1e-2);
  EXPECT_NEAR(balances[59], 0.29, 1e-2);
  EXPECT_NEAR(overpaiment, 370367.00, 1e-2);
  EXPECT_NEAR(total_payment, 3170367.00, 1e-2);
}

TEST(CreditCalcTest, DifferentiatedCredit) {
  double sum = 2800000.0;
  double rate = 5.0;
  int term = 60.0;

  auto [dates, payments, principals, interests, balances] =
      CreditCalc::Calculate(sum, rate, term, false);
  double total_payment = std::accumulate(payments.begin(), payments.end(), 0.0);
  double overpaiment = total_payment - sum;

  EXPECT_NEAR(payments[0], 58333.33, 1e-2);
  EXPECT_NEAR(payments[59], 46861.11, 1e-2);
  EXPECT_NEAR(principals[0], 46666.67, 1e-2);
  EXPECT_NEAR(principals[59], 46666.67, 1e-2);
  EXPECT_NEAR(interests[0], 11666.67, 1e-2);
  EXPECT_NEAR(interests[59], 194.44, 1e-2);
  EXPECT_NEAR(balances[0], 2753333.33, 1e-2);
  EXPECT_NEAR(balances[59], 0.0, 1e-2);

  EXPECT_NEAR(overpaiment, 355833.33, 1e-2);
  EXPECT_NEAR(total_payment, 3155833.33, 1e-2);
}
