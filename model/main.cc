#include <iostream>

#include "math_calc.h"

using namespace s21;

int main() {
  std::string expression = " +1234mod25 - 1e-2+sin(x) / 2.23^3";

  //   std::vector<Token> tokens = MathCalc::ParseExpression(expression);

  //   for (const Token& token : tokens) {
  //     std::cout << "Type: " << static_cast<int>(token.GetType())
  //               << "\tValue: " << token.GetToken()
  //               << "\tPriority: " << token.GetPriority() << std::endl;
  //   }

  std::cout << MathCalc::Calculate(expression) << "\n";
  return 0;
}
