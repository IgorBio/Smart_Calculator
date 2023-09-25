#include "smart_calc.h"

namespace smart_calc {

double SmartCalc::Calculate(const Expression& expression, double x) {
  std::vector<Token> rpn = ConvertToRPN(expression);
  return EvaluateRPN(rpn, x);
}

std::vector<Token> SmartCalc::ConvertToRPN(const Expression& expression) {
  std::vector<Token> rpn;
  std::stack<char> operators;

  std::istringstream iss(expression);
  Token token;

  while (iss >> token) {
    if (IsFunction(token)) {
      ProcessFunction(token, operators);
    } else if (IsOperator(token[0])) {
      ProcessOperator(token[0], operators, rpn);
    } else if (token == "(") {
      operators.push('(');
    } else if (token == ")") {
      ProcessClosingBracket(operators, rpn);
    } else {
      // Токен является операндом или числом
      rpn.push_back(token);
    }
  }

  ProcessRemainingOperators(operators, rpn);

  return rpn;
}

void SmartCalc::ProcessFunction(const Token& token,
                                std::stack<char>& operators) {
  operators.push(token[0]);
}

void SmartCalc::ProcessOperator(char token, std::stack<char>& operators,
                                std::vector<Token>& rpn) {
  while (!operators.empty() && IsOperator(operators.top()) &&
         GetOperatorPriority(operators.top()) >= GetOperatorPriority(token)) {
    rpn.push_back(Token(1, operators.top()));
    operators.pop();
  }
  operators.push(token);
}

void SmartCalc::ProcessClosingBracket(std::stack<char>& operators,
                                      std::vector<Token>& rpn) {
  while (!operators.empty() && operators.top() != '(') {
    rpn.push_back(Token(1, operators.top()));
    operators.pop();
  }
  if (!operators.empty() && operators.top() == '(') {
    operators.pop();
  } else {
    throw std::runtime_error("Неправильное выражение с скобками.");
  }
}

void SmartCalc::ProcessRemainingOperators(std::stack<char>& operators,
                                          std::vector<Token>& rpn) {
  while (!operators.empty()) {
    if (operators.top() == '(' || operators.top() == ')') {
      throw std::runtime_error("Неправильное выражение с скобками.");
    }
    rpn.push_back(Token(1, operators.top()));
    operators.pop();
  }
}

int SmartCalc::GetOperatorPriority(char op) {
  switch (op) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    case '^':
      return 3;
    default:
      return 0;
  }
}

double SmartCalc::EvaluateRPN(const std::vector<Token>& rpn, double x) {
  std::stack<double> values;

  for (const Token& token : rpn) {
    if (IsOperator(token[0])) {
      if (values.size() < 2) {
        throw std::runtime_error(
            "Неправильное количество операндов для оператора.");
      }
      double operand2 = values.top();
      values.pop();
      double operand1 = values.top();
      values.pop();

      switch (token[0]) {
        case '+':
          values.push(operand1 + operand2);
          break;
        case '-':
          values.push(operand1 - operand2);
          break;
        case '*':
          values.push(operand1 * operand2);
          break;
        case '/':
          if (operand2 == 0.0) {
            throw std::runtime_error("Деление на ноль.");
          }
          values.push(operand1 / operand2);
          break;
        case '^':
          values.push(std::pow(operand1, operand2));
          break;
        default:
          throw std::runtime_error("Неизвестный оператор.");
          break;
      }
    } else if (IsFunction(token)) {
      if (values.empty()) {
        throw std::runtime_error("Недостаточно операндов для функции.");
      }
      double operand = values.top();
      values.pop();

      // Выполнить соответствующую функцию и положить результат в стек
      if (token == "cos") {
        values.push(std::cos(operand));
      } else if (token == "sin") {
        values.push(std::sin(operand));
      } else if (token == "tan") {
        values.push(std::tan(operand));
      } else if (token == "acos") {
        values.push(std::acos(operand));
      } else if (token == "asin") {
        values.push(std::asin(operand));
      } else if (token == "atan") {
        values.push(std::atan(operand));
      } else if (token == "sqrt") {
        if (operand < 0.0) {
          throw std::runtime_error("Извлечение корня из отрицательного числа.");
        }
        values.push(std::sqrt(operand));
      } else if (token == "ln") {
        if (operand <= 0.0) {
          throw std::runtime_error("Логарифм от не положительного числа.");
        }
        values.push(std::log(operand));
      } else if (token == "log") {
        if (operand <= 0.0) {
          throw std::runtime_error("Логарифм от не положительного числа.");
        }
        values.push(std::log10(operand));
      } else {
        throw std::runtime_error("Неизвестная функция.");
      }
    } else {
      // Токен является операндом (числом), положить его в стек
      double operand = 0.0;
      try {
        operand = std::stod(token);
      } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка преобразования числа.");
      }
      values.push(operand);
    }
  }

  if (values.size() != 1) {
    throw std::runtime_error("Неправильное количество операндов и операторов.");
  }

  return values.top();
}

void SmartCalc::ApplyOperator(std::stack<double>& values, char op) {
  if (values.size() < 2) {
    throw std::runtime_error("Недостаточно операндов для оператора.");
  }

  double operand2 = values.top();
  values.pop();

  double operand1 = values.top();
  values.pop();

  double result;

  switch (op) {
    case '+':
      result = operand1 + operand2;
      break;
    case '-':
      result = operand1 - operand2;
      break;
    case '*':
      result = operand1 * operand2;
      break;
    case '/':
      if (operand2 == 0.0) {
        throw std::runtime_error("Деление на ноль.");
      }
      result = operand1 / operand2;
      break;
    case '^':
      result = std::pow(operand1, operand2);
      break;
    default:
      throw std::runtime_error("Неизвестный оператор.");
  }

  // Поместить результат обратно в стек
  values.push(result);
}

void SmartCalc::ApplyFunction(std::stack<double>& values, const Token& func,
                              double x) {
  if (values.empty()) {
    throw std::runtime_error("Недостаточно операндов для функции.");
  }

  double operand = values.top();
  values.pop();
  double result = 0.0;

  if (func == "cos") {
    result = std::cos(operand);
  } else if (func == "sin") {
    result = std::sin(operand);
  } else if (func == "tan") {
    result = std::tan(operand);
  } else if (func == "acos") {
    result = std::acos(operand);
  } else if (func == "asin") {
    result = std::asin(operand);
  } else if (func == "atan") {
    result = std::atan(operand);
  } else if (func == "sqrt") {
    if (operand < 0.0) {
      throw std::runtime_error("Извлечение корня из отрицательного числа.");
    }
    result = std::sqrt(operand);
  } else if (func == "ln") {
    if (operand <= 0.0) {
      throw std::runtime_error("Логарифм от неположительного числа.");
    }
    result = std::log(operand);
  } else if (func == "log") {
    if (operand <= 0.0) {
      throw std::runtime_error("Логарифм от неположительного числа.");
    }
    result = std::log10(operand);
  } else {
    throw std::runtime_error("Неизвестная функция.");
  }

  // Поместить результат обратно в стек
  values.push(result);
}

bool SmartCalc::IsOperator(char op) {
  return (op == '+' || op == '-' || op == '*' || op == '/' || op == '^');
}

bool SmartCalc::IsFunction(const Token& token) {
  return (token == "cos" || token == "sin" || token == "tan" ||
          token == "acos" || token == "asin" || token == "atan" ||
          token == "sqrt" || token == "ln" || token == "log");
}
}  // namespace smart_calc