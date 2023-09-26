#include <iostream>

#include "model/math_calc.h"

using namespace s21;

int main() {
  std::string expression = "  x 55";

  std::vector<Token> tokens = MathCalc::ParseExpression(expression);

  for (const Token& token : tokens) {
    std::cout << "Type: " << static_cast<int>(token.GetType())
              << "\tValue: " << token.GetToken()
              << "\tPriority: " << token.GetPriority() << std::endl;
  }

  std::cout << MathCalc::Calculate(expression, 1) << "\n";
  return 0;
}
