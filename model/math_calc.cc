#include "math_calc.h"

namespace s21 {

/**
 * @brief Constructor for initializing MathCalc with an expression.
 * @param expression The mathematical expression to be evaluated.
 */
MathCalc::MathCalc(const std::string& expression) {
  rpn_ = ConvertToRPN(ParseExpression(expression));
}

/**
 * @brief Static method to calculate the result of a mathematical expression.
 * @param expression The mathematical expression to be evaluated.
 * @param x The value of the variable 'x' (default is 0.0).
 * @return The result of the expression evaluation.
 */
double MathCalc::Calculate(const std::string& expression, double x) {
  std::vector<Token> tokens = ParseExpression(expression);
  std::vector<Token> rpn = ConvertToRPN(tokens);
  return EvaluateRPN(rpn, x);
}

/**
 * @brief Calculate the result of the mathematical expression.
 * @param x The value of the variable 'x'.
 * @return The result of the expression evaluation.
 */
double MathCalc::Calculate(double x) { return EvaluateRPN(rpn_, x); }

/**
 * @brief Static method to parse a mathematical expression into tokens.
 * @param expression The mathematical expression to be parsed.
 * @return A vector of tokens representing the parsed expression.
 */
std::vector<Token> MathCalc::ParseExpression(const std::string& expression) {
  std::vector<Token> tokens;
  std::size_t pos = 0;

  while (pos < expression.length()) {
    char ch = expression[pos];

    if (std::isdigit(ch) || ch == '.') {
      InsertOmittedMul(tokens, true);
      pos = ParseNumber(expression, pos, tokens);
    } else if (std::isalpha(ch)) {
      pos = ParseAlpha(expression, pos, tokens);
    } else if (ch == '(') {
      InsertOmittedMul(tokens);
      tokens.push_back(Token(TokenType::kOpenBracket, "("));
      ++pos;
    } else if (ch == ')') {
      tokens.push_back(Token(TokenType::kCloseBracket, ")"));
      ++pos;
    } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
      pos = ParseOperator(expression, pos, tokens);
    } else if (std::isspace(ch)) {
      ++pos;
    } else {
      throw std::invalid_argument("Invalid character: " + std::string(1, ch));
    }
  }

  return tokens;
}

/**
 * @brief Static method to convert a vector of tokens to Reverse Polish Notation
 * (RPN).
 * @param tokens The vector of tokens representing the expression.
 * @return A vector of tokens in RPN.
 */
std::vector<Token> MathCalc::ConvertToRPN(const std::vector<Token>& tokens) {
  std::vector<Token> rpn;
  std::stack<Token> operators;

  for (const Token& token : tokens) {
    if (token.IsNumber() || token.IsVariable()) {
      rpn.push_back(token);
    } else if (token.IsFunction() || token.IsOpenBracket()) {
      operators.push(token);
    } else if (token.IsCloseBracket()) {
      while (!operators.empty() && !operators.top().IsOpenBracket()) {
        rpn.push_back(operators.top());
        operators.pop();
      }
      if (operators.empty() || !operators.top().IsOpenBracket()) {
        throw std::invalid_argument("Invalid bracket sequence");
      }
      operators.pop();
      if (!operators.empty() && operators.top().IsFunction()) {
        rpn.push_back(operators.top());
        operators.pop();
      }
    } else if (token.IsOperator()) {
      while (!operators.empty() && operators.top().IsOperator() &&
             (token.GetPriority() < operators.top().GetPriority() ||
              (token.GetPriority() == operators.top().GetPriority() &&
               !operators.top().IsRightAssociative()))) {
        rpn.push_back(operators.top());
        operators.pop();
      }
      operators.push(token);
    }
  }

  while (!operators.empty()) {
    if (operators.top().IsOpenBracket() || operators.top().IsCloseBracket()) {
      throw std::invalid_argument("Invalid bracket sequence");
    }
    rpn.push_back(operators.top());
    operators.pop();
  }

  return rpn;
}

/**
 * @brief Static method to evaluate a mathematical expression in RPN.
 * @param rpn The vector of tokens in Reverse Polish Notation.
 * @param x The value of the variable 'x'.
 * @return The result of the RPN expression evaluation.
 */
double MathCalc::EvaluateRPN(const std::vector<Token>& rpn, double x) {
  std::stack<double> operands;

  for (const Token& token : rpn) {
    if (token.IsNumber()) {
      operands.push(std::stod(token.GetToken()));
    } else if (token.IsVariable()) {
      operands.push(x);
    } else if (token.IsOperator()) {
      ProcessOperator(token, operands);
    } else if (token.IsFunction()) {
      ProcessFunction(token, operands);
    }
  }

  if (operands.size() != 1) {
    throw std::invalid_argument("Invalid expression");
  }

  return operands.top();
}

/**
 * @brief Static method to parse a number token from the expression.
 * @param expression The mathematical expression.
 * @param pos The current position in the expression.
 * @param tokens The vector to store parsed tokens.
 * @return The new position after parsing the number token.
 */
std::size_t MathCalc::ParseNumber(const std::string& expression,
                                  std::size_t pos, std::vector<Token>& tokens) {
  std::size_t start = pos;

  while (pos < expression.length()) {
    char ch = expression[pos];
    if (!std::isdigit(ch) && ch != '.' && std::toupper(ch) != 'E') {
      break;
    }
    if (std::toupper(ch) == 'E') {
      ++pos;
      if (pos < expression.length()) {
        char next_ch = expression[pos];
        if (next_ch == '+' || next_ch == '-') {
          ++pos;
        }
      }
    }
    ++pos;
  }

  std::string tok = expression.substr(start, pos - start);

  if (!ValidateNumber(tok)) {
    throw std::invalid_argument("Invalid number: " + tok);
  }

  tokens.push_back(Token(TokenType::kNumber, tok));

  return pos;
}

/**
 * @brief Static method to parse an alpha token (function or variable) from the
 * expression.
 * @param expression The mathematical expression.
 * @param pos The current position in the expression.
 * @param tokens The vector to store parsed tokens.
 * @return The new position after parsing the alpha token.
 */
std::size_t MathCalc::ParseAlpha(const std::string& expression, std::size_t pos,
                                 std::vector<Token>& tokens) {
  std::size_t start = pos;
  while (pos < expression.length() && std::isalpha(expression[pos]) &&
         expression.substr(start, pos - start) != "mod" &&
         expression.substr(start, pos - start) != "x") {
    ++pos;
  }
  std::string tok = expression.substr(start, pos - start);

  if (!ValidateAlpha(tok)) {
    throw std::invalid_argument("Invalid token: " + tok);
  }

  if (tok == "x") {
    InsertOmittedMul(tokens);
    tokens.push_back(Token(TokenType::kVariable, tok));
  } else if (tok == "mod") {
    tokens.push_back(Token(TokenType::kBinaryOperator, tok, 3));
  } else {
    InsertOmittedMul(tokens);
    tokens.push_back(Token(TokenType::kFunction, tok));
  }

  return pos;
}

/**
 * @brief Static method to parse an operator token from the expression.
 * @param expression The mathematical expression.
 * @param pos The current position in the expression.
 * @param tokens The vector to store parsed tokens.
 * @return The new position after parsing the operator token.
 */
std::size_t MathCalc::ParseOperator(const std::string& expression,
                                    std::size_t pos,
                                    std::vector<Token>& tokens) {
  char op = expression[pos];
  TokenType type;
  short priority;

  bool is_unary = (tokens.size() == 0 || expression[pos - 1] == '(' ||
                   tokens.back().IsBinaryOperator());

  switch (op) {
    case '+':
    case '-':
      type = is_unary ? TokenType::kUnaryOperator : TokenType::kBinaryOperator;
      priority = is_unary ? 4 : 1;
      break;
    case '*':
    case '/':
      type = TokenType::kBinaryOperator;
      priority = 2;
      break;
    case '^':
      type = TokenType::kBinaryOperator;
      priority = 3;
      break;
    default:
      throw std::invalid_argument("Invalid operator: " + std::string(1, op));
  }

  tokens.push_back(Token(type, std::string(1, op), priority));
  ++pos;

  return pos;
}

/**
 * @brief Inserts an omitted multiplication operator in the vector of tokens if
 * necessary.
 * @param tokens The vector of tokens to insert the operator into.
 * @param flg A flag indicating insertion before number.
 */
void MathCalc::InsertOmittedMul(std::vector<Token>& tokens, bool flg) {
  if ((!flg && !tokens.empty() &&
       (tokens.back().IsNumber() || tokens.back().IsCloseBracket() ||
        tokens.back().IsVariable())) ||
      (flg && !tokens.empty() && tokens.back().IsCloseBracket())) {
    tokens.push_back(Token(TokenType::kBinaryOperator, "*", 3));
  }
}

/**
 * @brief Static method to validate a number token.
 * @param token The token to validate.
 * @return True if the token is a valid number, otherwise false.
 */
bool MathCalc::ValidateNumber(const std::string& token) {
  bool has_dot = false;
  bool has_e = false;
  bool has_sign = false;

  for (char ch : token) {
    if (!std::isdigit(ch)) {
      if (ch == '.' && !has_dot && !has_e) {
        has_dot = true;
      } else if ((std::toupper(ch) == 'E') && !has_e) {
        has_e = true;
      } else if ((ch == '+' || ch == '-') && has_e && !has_sign) {
        has_sign = true;
      } else {
        return false;
      }
    }
  }

  if (has_e && (std::toupper(token.front()) == 'E' ||
                std::toupper(token.back()) == 'E')) {
    return false;
  }

  if (token == "." || token.find(".e") == 0) {
    return false;
  }

  return true;
}

/**
 * @brief Static method to validate an alpha token (function or variable).
 * @param token The token to validate.
 * @return True if the token is a valid alpha token, otherwise false.
 */
bool MathCalc::ValidateAlpha(const std::string& token) {
  static const std::vector<std::string> valid_tokens = {
      "sin",  "cos", "tan", "asin", "acos", "atan",
      "sqrt", "ln",  "log", "x",    "mod"};

  return std::any_of(valid_tokens.cbegin(), valid_tokens.cend(),
                     [&token](const std::string& tok) { return tok == token; });
}

/**
 * @brief Static method to process an operator token in RPN evaluation.
 * @param token The operator token to process.
 * @param operands The stack of operands for RPN evaluation.
 */
void MathCalc::ProcessOperator(const Token& token,
                               std::stack<double>& operands) {
  if (operands.size() < 1 && token.IsUnaryOperator()) {
    throw std::invalid_argument("Not enough operands for unary operator");
  }
  if (operands.size() < 2 && token.IsBinaryOperator()) {
    throw std::invalid_argument("Not enough operands for binary operator");
  }
  double result = 0.0;

  if (token.IsUnaryOperator()) {
    double operand = operands.top();
    operands.pop();

    if (token.GetToken() == "+") {
      result = operand;
    } else if (token.GetToken() == "-") {
      result = -operand;
    } else {
      throw std::invalid_argument("Unsupported unary operator: " +
                                  token.GetToken());
    }
  } else {
    double operand2 = operands.top();
    operands.pop();
    double operand1 = operands.top();
    operands.pop();

    if (token.GetToken() == "+") {
      result = operand1 + operand2;
    } else if (token.GetToken() == "-") {
      result = operand1 - operand2;
    } else if (token.GetToken() == "*") {
      result = operand1 * operand2;
    } else if (token.GetToken() == "/") {
      if (operand2 == 0) {
        throw std::invalid_argument("Division by zero");
      }
      result = operand1 / operand2;
    } else if (token.GetToken() == "^") {
      result = std::pow(operand1, operand2);
    } else if (token.GetToken() == "mod") {
      result = std::fmod(operand1, operand2);
    } else {
      throw std::invalid_argument("Unsupported binary operator: " +
                                  token.GetToken());
    }
  }

  operands.push(result);
}

/**
 * @brief Static method to process a function token in RPN evaluation.
 * @param token The function token to process.
 * @param operands The stack of operands for RPN evaluation.
 */
void MathCalc::ProcessFunction(const Token& token,
                               std::stack<double>& operands) {
  if (operands.empty()) {
    throw std::invalid_argument("Not enough operands for function: " +
                                token.GetToken());
  }

  double operand = operands.top();
  operands.pop();
  double result = 0.0;

  if (token.GetToken() == "sin") {
    result = std::sin(operand);
  } else if (token.GetToken() == "cos") {
    result = std::cos(operand);
  } else if (token.GetToken() == "tan") {
    result = std::tan(operand);
  } else if (token.GetToken() == "asin") {
    if (operand < -1.0 || operand > 1.0) {
      throw std::invalid_argument("Invalid input for asin");
    }
    result = std::asin(operand);
  } else if (token.GetToken() == "acos") {
    if (operand < -1.0 || operand > 1.0) {
      throw std::invalid_argument("Invalid input for acos");
    }
    result = std::acos(operand);
  } else if (token.GetToken() == "atan") {
    result = std::atan(operand);
  } else if (token.GetToken() == "sqrt") {
    if (operand < 0.0) {
      throw std::invalid_argument("Invalid input for sqrt");
    }
    result = std::sqrt(operand);
  } else if (token.GetToken() == "ln") {
    if (operand <= 0.0) {
      throw std::invalid_argument("Invalid input for ln");
    }
    result = std::log(operand);
  } else if (token.GetToken() == "log") {
    if (operand <= 0.0) {
      throw std::invalid_argument("Invalid input for log");
    }
    result = std::log10(operand);
  } else {
    throw std::invalid_argument("Unsupported function: " + token.GetToken());
  }

  operands.push(result);
}

}  // namespace s21
