#include <gtest/gtest.h>

#include "math_calc.h"

using namespace s21;

TEST(MathCalcTest, Calculate) {
  double result = MathCalc::Calculate("(1 + 2 - 3) * 4 / 5.0 + 6 - 7 + 8 * 9");
  EXPECT_DOUBLE_EQ(result, (1 + 2 - 3) * 4 / 5.0 + 6 - 7 + 8 * 9);

  result = MathCalc::Calculate("cos(0.1) + sin(0.2) - tan(0.3)");
  EXPECT_DOUBLE_EQ(result, cos(0.1) + sin(0.2) - tan(0.3));

  result = MathCalc::Calculate("acos(0.4) + asin(0.5) - atan(0.6)");
  EXPECT_DOUBLE_EQ(result, acos(0.4) + asin(0.5) - atan(0.6));

  result = MathCalc::Calculate("sqrt(9876) + 1234mod25 - 2.543e+2");
  EXPECT_DOUBLE_EQ(result, sqrt(9876) + fmod(1234, 25) - 2.543e+2);

  result = MathCalc::Calculate(" -1 + 2 -3 + 4 ^ 2 - 5 / 6.0 * 7mod8");
  EXPECT_DOUBLE_EQ(result, -1 + 2 - 3 + pow(4, 2) - 5 / 6.0 * fmod(7, 8));
}

TEST(MathCalcTest, Exception) {
  EXPECT_THROW(MathCalc::Calculate("5*a"), std::invalid_argument);
  //   EXPECT_THROW(MathCalc::Calculate("5 7"), std::invalid_argument);
  //   EXPECT_THROW(MathCalc::Calculate("(+-/)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("()"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("sinb(x)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("tan(2q)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("sqrt(r)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("5fmod4"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("x 55"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2 + 5.5.5"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2mod"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2asin"), std::invalid_argument);
  // EXPECT_THROW(MathCalc::Calculate("atan0.5"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("mod3"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("3mob2"), std::invalid_argument);

  EXPECT_THROW(MathCalc::Calculate("e-1"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2e"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2e+3.4"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("cos(12)e-3"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2e*cos(34)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2ee-3"), std::invalid_argument);

  EXPECT_THROW(MathCalc::Calculate("(x))"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("((x)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("log(x"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("cosx"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("sin)(x)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("tan(x))"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("asin(x("), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("acos)x)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("atan((x)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("sqrtx"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2+3)ln)/4"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("8 *)*cos(25)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("pow(3, 2)"), std::invalid_argument);
}

TEST(MathCalcTest, ExponentialNotation) {
  double result = MathCalc::Calculate("1.0e2");
  EXPECT_DOUBLE_EQ(result, 1.0e2);

  result = MathCalc::Calculate("1.2e+3");
  EXPECT_DOUBLE_EQ(result, 1.2e+3);

  result = MathCalc::Calculate("3.4e-5");
  EXPECT_DOUBLE_EQ(result, 3.4e-5);

  result = MathCalc::Calculate("1.2 * 3.4e+5 * 6.7");
  EXPECT_DOUBLE_EQ(result, 1.2 * 3.4e+5 * 6.7);
}

TEST(MathCalcTest, Brackets) {
  double x = 25.0;
  double result = MathCalc::Calculate("sin(cos(tan(sqrt(ln(log(x))))))", x);
  EXPECT_DOUBLE_EQ(result, sin(cos(tan(sqrt(log(log10(x)))))));

  result = MathCalc::Calculate("(2 - 9) / (7.5 + 2.0)", x);
  EXPECT_DOUBLE_EQ(result, (2 - 9) / (7.5 + 2.0));
}
