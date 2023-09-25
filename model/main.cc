#include <iostream>

#include "math_calc.h"

using namespace s21;

int main() {
  std::string expression = "sqrt(9876) + 1234mod25 - 2.543e+2 + sin(x) / 2^3";

  std::vector<Token> tokens = MathCalc::ParseExpression(expression);
  //   std::cout << MathCalc::ValidateInfixNotation(tokens) << "\n";

  for (const Token& token : tokens) {
    std::cout << "Type: " << static_cast<int>(token.GetType())
              << ", Value: " << token.GetToken()
              << "\t, Priority: " << token.GetPriority() << std::endl;
  }

  return 0;
}