#include <gtest/gtest.h>

#include "math_calc.h"

using namespace s21;

TEST(MathCalcTest, Calculate) {
  double x = 25.0;
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

  result = MathCalc::Calculate("x + cos(x - 5) / x", x);
  EXPECT_DOUBLE_EQ(result, x + cos(x - 5) / x);

  result = MathCalc::Calculate("2xcos(3x)x4x", x);
  EXPECT_DOUBLE_EQ(result, 2 * x * cos(3 * x) * x * 4 * x);
}

TEST(MathCalcTest, ExponentialNotation) {
  double result = MathCalc::Calculate("1.0e2");
  EXPECT_DOUBLE_EQ(result, 1.0e2);

  result = MathCalc::Calculate("1.2e+3");
  EXPECT_DOUBLE_EQ(result, 1.2e+3);

  result = MathCalc::Calculate(".2e4");
  EXPECT_DOUBLE_EQ(result, .2e4);

  result = MathCalc::Calculate("3.4e-5");
  EXPECT_DOUBLE_EQ(result, 3.4e-5);

  result = MathCalc::Calculate("1.2 * 3.4e+5 * 6.7");
  EXPECT_DOUBLE_EQ(result, 1.2 * 3.4e+5 * 6.7);
}

TEST(MathCalcTest, Brackets) {
  double x = 25.0;
  double result = MathCalc::Calculate("sin(cos(tan(sqrt(ln(log(x))))))", x);
  EXPECT_DOUBLE_EQ(result, sin(cos(tan(sqrt(log(log10(x)))))));

  result = MathCalc::Calculate("(2 - 9) / (7.5 + 2.0)");
  EXPECT_DOUBLE_EQ(result, (2 - 9) / (7.5 + 2.0));
}

TEST(MathCalcTest, Operations) {
  double x = 25.0;
  double result = MathCalc::Calculate("1234.5 + 678.9");
  EXPECT_DOUBLE_EQ(result, 1234.5 + 678.9);

  result = MathCalc::Calculate("-1234.5 - 2x", x);
  EXPECT_DOUBLE_EQ(result, -1234.5 - 2 * x);

  result = MathCalc::Calculate("1234.5 * -678.9 / -2.223");
  EXPECT_DOUBLE_EQ(result, 1234.5 * -678.9 / -2.223);

  result = MathCalc::Calculate(" 123456789 + (+98765432.1)");
  EXPECT_DOUBLE_EQ(result, 123456789 + (+98765432.1));

  result = MathCalc::Calculate("(123.4 * 567.8) / (234.5 * 678.9)");
  EXPECT_DOUBLE_EQ(result, (123.4 * 567.8) / (234.5 * 678.9));

  result = MathCalc::Calculate(" -x - -2x + x * (x + x)", x);
  EXPECT_DOUBLE_EQ(result, -x - -2 * x + x * (x + x));
}

TEST(MathCalcTest, Power) {
  double x = 5.0;
  double result = MathCalc::Calculate("1234.5 ^ 4.1");
  EXPECT_DOUBLE_EQ(result, pow(1234.5, 4.1));

  result = MathCalc::Calculate("2 ^ 3 ^ 4");
  EXPECT_DOUBLE_EQ(result, pow(2, pow(3, 4)));

  result = MathCalc::Calculate("(4 ^ 3) ^ 2");
  EXPECT_DOUBLE_EQ(result, pow(pow(4, 3), 2));

  result = MathCalc::Calculate("-1234.5 ^ 0.0");
  EXPECT_DOUBLE_EQ(result, pow(-1234.5, 0.0));

  result = MathCalc::Calculate("1234.5 ^ -2");
  EXPECT_DOUBLE_EQ(result, pow(1234.5, -2));

  result = MathCalc::Calculate("123456789 ^ (+0.42)");
  EXPECT_DOUBLE_EQ(result, pow(123456789, 0.42));

  result = MathCalc::Calculate("(12.34 ^ 0.5678) ^ (23.45 ^ 0.6789)");
  EXPECT_DOUBLE_EQ(result, pow(pow(12.34, 0.5678), pow(23.45, 0.6789)));

  result = MathCalc::Calculate("-x ^ 2 * 2 ^ x", x);
  EXPECT_DOUBLE_EQ(result, pow(-x, 2) * pow(2, x));
}

TEST(MathCalcTest, Modulus) {
  double x = 25.0;
  double result = MathCalc::Calculate("1234.5 mod 678.9");
  EXPECT_DOUBLE_EQ(result, fmod(1234.5, 678.9));

  result = MathCalc::Calculate("1234.5 mod -678.9");
  EXPECT_DOUBLE_EQ(result, fmod(1234.5, -678.9));

  result = MathCalc::Calculate("123456789 mod (+98765432.1)");
  EXPECT_DOUBLE_EQ(result, fmod(123456789, 98765432.1));

  result = MathCalc::Calculate("(123.4 mod 0.5678) mod (234.5 mod 0.6789)");
  EXPECT_DOUBLE_EQ(result, fmod(fmod(123.4, 0.5678), fmod(234.5, 0.6789)));

  result = MathCalc::Calculate("50modx", x);
  EXPECT_DOUBLE_EQ(result, fmod(50, x));

  result = MathCalc::Calculate("50mod-x", x);
  EXPECT_DOUBLE_EQ(result, fmod(50, -x));

  result = MathCalc::Calculate("-x mod 18 mod (27 mod x)", x);
  EXPECT_DOUBLE_EQ(result, fmod(fmod(-25, 1825), fmod(27, 25)));
}

TEST(MathCalcTest, Functions) {
  double x = 0.25;
  double result = MathCalc::Calculate("cos(1) + -sin(2) * -tan(3)");
  EXPECT_DOUBLE_EQ(result, cos(1) + -sin(2) * -tan(3));

  result = MathCalc::Calculate("(acos(0.4) / asin(0.5)) - +atan(0.6)");
  EXPECT_DOUBLE_EQ(result, (acos(0.4) / asin(0.5)) - +atan(0.6));

  result = MathCalc::Calculate("-ln(7) + (-log(8)) mod sqrt(9.1)");
  EXPECT_DOUBLE_EQ(result, -log(7) + fmod((-log10(8)), sqrt(9.1)));

  result = MathCalc::Calculate("-sin(-x)-cos(x)", x);
  EXPECT_DOUBLE_EQ(result, -sin(-x) - cos(x));

  result = MathCalc::Calculate("25.modsqrt(35)/-x", x);
  EXPECT_DOUBLE_EQ(result, fmod(25, sqrt(35)) / -0.25);

  result = MathCalc::Calculate("tan(1.570796)");
  EXPECT_DOUBLE_EQ(result, tan(1.570796));

  result = MathCalc::Calculate("tan(0)");
  EXPECT_DOUBLE_EQ(result, tan(0));
}

TEST(MathCalcTest, ComplexInput) {
  double x = 25.0;

  double result = MathCalc::Calculate("x + x / 5 - x * 2 + x ^ 2 - x mod 4", x);
  EXPECT_DOUBLE_EQ(result, x + x / 5 - x * 2 + pow(x, 2) - fmod(x, 4));

  result = MathCalc::Calculate(
      "cos(tan( 7.0521+tan(cos(asin(   0.0945))-tan(cos( 4.6893)  ))-  "
      "sin(9.1049- sin(cos(tan(tan( cos(6.8795 ) )  ) )))))");
  EXPECT_NEAR(result,
              cos(tan(7.0521 + tan(cos(asin(0.0945)) - tan(cos(4.6893))) -
                      sin(9.1049 - sin(cos(tan(tan(cos(6.8795)))))))),
              1e-8);

  result = MathCalc::Calculate(
      "tan( 3.764) ^sin( 9.445-sin( 7.2889 /  8.0438 -cos( "
      "sin(cos(tan(8.4448))) - 4.482)  ) / tan(cos(cos(sin(cos( cos(2.003)) )  "
      ") / 0.1315) ))  -  8.8453/ 0.3612");
  EXPECT_NEAR(
      result,
      pow(tan(3.764),
          sin(9.445 -
              sin(7.2889 / 8.0438 - cos(sin(cos(tan(8.4448))) - 4.482)) /
                  tan(cos(cos(sin(cos(cos(2.003)))) / 0.1315)))) -
          8.8453 / 0.3612,
      1e-8);
}

TEST(MathCalcTest, Exception) {
  EXPECT_THROW(MathCalc::Calculate(""), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate(" "), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("5*a"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("5!"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("5 7"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("(+-/)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("5**7"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("**"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("3*"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("/7"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("."), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("()"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("sinb(x)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("tan(2q)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("sqrt(r)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("5fmod4"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("x 55"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2 + 5.5.5"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2mod"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2asin"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("2*s in(3)"), std::invalid_argument);

  EXPECT_THROW(MathCalc::Calculate("mod3"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("3mob2"), std::invalid_argument);

  EXPECT_THROW(MathCalc::Calculate("e"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("e-1"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2e"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2e+3.4"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("cos(12)e-3"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2e*cos(34)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("1.2ee-3"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate(".e1"), std::invalid_argument);

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

  EXPECT_THROW(MathCalc::Calculate("1 / 0.0"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("asin(2)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("acos(3)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("sqrt(-4)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("ln(0.0)"), std::invalid_argument);
  EXPECT_THROW(MathCalc::Calculate("log(-1)"), std::invalid_argument);
}
