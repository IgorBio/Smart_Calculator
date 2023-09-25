#ifndef SMARTCALC_MODEL_MATH_CALC_H_
#define SMARTCALC_MODEL_MATH_CALC_H_

#include <algorithm>
#include <cctype>
#include <cmath>
#include <stack>
#include <stdexcept>
#include <vector>

#include "token.h"

namespace s21 {

/**
 * @class MathCalc
 * @brief A class for mathematical expression evaluation.
 *
 * This class provides functionality to parse and evaluate mathematical
 * expressions involving numbers, variables, functions, and operators.
 */
class MathCalc {
 public:
  explicit MathCalc(const std::string& expression);

  static double Calculate(const std::string& expression, double x = 0.0);
  double Calculate(double x);

 private:
  static std::vector<Token> ParseExpression(const std::string& expression);
  static std::vector<Token> ConvertToRPN(const std::vector<Token>& tokens);
  static double EvaluateRPN(const std::vector<Token>& rpn, double x);
  static std::string RemoveSpaces(const std::string& expression);
  static std::size_t ParseNumber(const std::string& expression, std::size_t pos,
                                 std::vector<Token>& tokens);
  static std::size_t ParseAlpha(const std::string& expression, std::size_t pos,
                                std::vector<Token>& tokens);
  static std::size_t ParseOperator(const std::string& expression,
                                   std::size_t pos, std::vector<Token>& tokens);
  static bool ValidateNumber(const std::string& token);
  static bool ValidateAlpha(const std::string& token);
  static void ProcessOperator(const Token& token, std::stack<double>& operands);
  static void ProcessFunction(const Token& token, std::stack<double>& operands);

  std::vector<Token> rpn_;
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_MATH_CALC_H_
