#ifndef SMARTCALC_MODEL_MATH_CALC_H_
#define SMARTCALC_MODEL_MATH_CALC_H_

#include <algorithm>
#include <cctype>
#include <stack>
#include <stdexcept>
#include <vector>

#include "token.h"

namespace s21 {

class MathCalc {
 public:
  static std::vector<Token> ParseExpression(const std::string& expression);
  static std::vector<Token> ConvertToRPN(const std::vector<Token>& tokens);

 private:
  static std::size_t ParseNumber(const std::string& expression, std::size_t pos,
                                 std::vector<Token>& tokens);
  static std::size_t ParseAlpha(const std::string& expression, std::size_t pos,
                                std::vector<Token>& tokens);
  static std::size_t ParseOperator(const std::string& expression,
                                   std::size_t pos, std::vector<Token>& tokens);
  static bool ValidateNumber(const std::string& token);
  static bool ValidateAlpha(const std::string& token);
  static bool ValidateSci(const std::string& token);
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_MATH_CALC_H_