#ifndef SMARTCALC_SMARTCALC_H_
#define SMARTCALC_SMARTCALC_H_

#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace smart_calc {

using Token = std::string;
using Expression = std::string;

class SmartCalc {
 public:
  double Calculate(const Expression& expression, double x = 0.0);

 private:
  bool IsOperator(char token);
  bool IsFunction(const Token& token);
  int GetOperatorPriority(char op);

  void ApplyOperator(std::stack<double>& values, char op);
  void ApplyFunction(std::stack<double>& values, const Token& func, double x);
  std::vector<Token> ConvertToRPN(const Expression& expression);
  double EvaluateRPN(const std::vector<Token>& rpn, double x);

  void ProcessFunction(const Token& token, std::stack<char>& operators);
  void ProcessOperator(char token, std::stack<char>& operators,
                       std::vector<Token>& rpn);
  void ProcessClosingBracket(std::stack<char>& operators,
                             std::vector<Token>& rpn);
  void ProcessRemainingOperators(std::stack<char>& operators,
                                 std::vector<Token>& rpn);
};
}  // namespace smart_calc

#endif  // SMARTCALC_SMARTCALC_H_
