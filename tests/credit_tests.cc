#include <gtest/gtest.h>

#include <numeric>

#include "credit_calc.h"

using namespace s21;

TEST(CreditCalcTest, AnnuityCredit) {
  CreditCalc::CreditParams params{.sum = 2800000.0,
                                  .rate = 5.0,
                                  .term = 60,
                                  .type = CreditCalc::CreditType::kAnnuity};

  auto plan = CreditCalc::Calculate(params);
  double total_payment =
      std::accumulate(plan.payments.begin(), plan.payments.end(), 0.0);
  double overpayment = total_payment - params.sum;

  EXPECT_NEAR(plan.payments[0], 52839.45, 1e-2);
  EXPECT_NEAR(plan.payments[59], 52839.45, 1e-2);
  EXPECT_NEAR(plan.principals[0], 41172.78, 1e-2);
  EXPECT_NEAR(plan.principals[59], 52620.20, 1e-2);
  EXPECT_NEAR(plan.interests[0], 11666.67, 1e-2);
  EXPECT_NEAR(plan.interests[59], 219.25, 1e-2);
  EXPECT_NEAR(plan.balances[0], 2758827.22, 1e-2);
  EXPECT_NEAR(plan.balances[59], 0.29, 1e-2);
  EXPECT_NEAR(overpayment, 370367.00, 1e-2);
  EXPECT_NEAR(total_payment, 3170367.00, 1e-2);
}

TEST(CreditCalcTest, DifferentiatedCredit) {
  CreditCalc::CreditParams params{
      .sum = 2800000.0,
      .rate = 5.0,
      .term = 60,
      .type = CreditCalc::CreditType::kDifferentiated};

  auto plan = CreditCalc::Calculate(params);
  double total_payment =
      std::accumulate(plan.payments.begin(), plan.payments.end(), 0.0);
  double overpayment = total_payment - params.sum;

  EXPECT_NEAR(plan.payments[0], 58333.33, 1e-2);
  EXPECT_NEAR(plan.payments[59], 46861.11, 1e-2);
  EXPECT_NEAR(plan.principals[0], 46666.67, 1e-2);
  EXPECT_NEAR(plan.principals[59], 46666.67, 1e-2);
  EXPECT_NEAR(plan.interests[0], 11666.67, 1e-2);
  EXPECT_NEAR(plan.interests[59], 194.44, 1e-2);
  EXPECT_NEAR(plan.balances[0], 2753333.33, 1e-2);
  EXPECT_NEAR(plan.balances[59], 0.0, 1e-2);

  EXPECT_NEAR(overpayment, 355833.33, 1e-2);
  EXPECT_NEAR(total_payment, 3155833.33, 1e-2);
}
