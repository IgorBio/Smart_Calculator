#include <gtest/gtest.h>

#include "credit_calc.h"

using namespace s21;

TEST(CreditCalcTest, AnnuityCredit) {
  double sum = 2800000.0;
  double rate = 5.0;
  int term = 60.0;

  auto [payments, overpaiment, total_payment] =
      CreditCalc::Calculate(sum, rate, term, true);
  EXPECT_NEAR(payments[0], 52839.45, 1e-2);
  EXPECT_NEAR(overpaiment, 370367.00, 1e-2);
  EXPECT_NEAR(total_payment, 3170367.00, 1e-2);
}

TEST(CreditCalcTest, DifferentiatedCredit) {
  double sum = 2800000.0;
  double rate = 5.0;
  int term = 60.0;

  auto [payments, overpaiment, total_payment] =
      CreditCalc::Calculate(sum, rate, term, false);

  EXPECT_NEAR(payments[0], 58333.33, 1e-2);
  EXPECT_NEAR(payments[59], 46861.11, 1e-2);
  EXPECT_NEAR(overpaiment, 355833.33, 1e-2);
  EXPECT_NEAR(total_payment, 3155833.33, 1e-2);
}