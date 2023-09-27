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
 * MathCalc provides methods for parsing, converting, and evaluating
 * mathematical expressions. It supports basic arithmetic operations, functions,
 * variables, and can evaluate expressions with or without variables. The class
 * utilizes Reverse Polish Notation (RPN) and the Shunting-Yard algorithm for
 * expression processing.
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
  static std::size_t ParseNumber(const std::string& expression, std::size_t pos,
                                 std::vector<Token>& tokens);
  static std::size_t ParseAlpha(const std::string& expression, std::size_t pos,
                                std::vector<Token>& tokens);
  static std::size_t ParseOperator(const std::string& expression,
                                   std::size_t pos, std::vector<Token>& tokens);
  static void InsertOmittedMul(std::vector<Token>& tokens, bool flg = false);
  static bool ValidateNumber(const std::string& token);
  static bool ValidateAlpha(const std::string& token);
  static bool ValidateSpaces(const std::string& expression, std::size_t pos);
  static void ProcessOperator(const Token& token, std::stack<double>& operands);
  static void ProcessFunction(const Token& token, std::stack<double>& operands);
  static void ProcessBrackets(std::stack<Token>& operators,
                              std::vector<Token>& rpn);
  static void ProcessOperators(const Token& token, std::stack<Token>& operators,
                               std::vector<Token>& rpn);
  static void ProcessRemainingOperators(std::stack<Token>& operators,
                                        std::vector<Token>& rpn);

  std::vector<Token> rpn_;
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_MATH_CALC_H_
