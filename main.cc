#include <iostream>

#include "model/deposit_calc.h"

using namespace s21;

int main() {
  std::string date1 = "01-02-2023";

  std::string date2 = DepositCalc::AddDays(date1, 59);

  std::cout << date2 << "\n";
  return 0;
}
