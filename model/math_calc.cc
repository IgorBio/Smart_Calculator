#include "math_calc.h"

namespace s21 {

/**
 * @brief Constructor of the MathCalc class.
 *
 * The constructor takes a mathematical expression as a string and then
 * converts it to Reverse Polish Notation (RPN) using the ParseExpression and
 * ConvertToRPN methods, and stores the resulting RPN expression within the
 * MathCalc object.
 *
 * @param expression Mathematical expression as a string.
 */
MathCalc::MathCalc(const std::string& expression) {
  rpn_ = ConvertToRPN(ParseExpression(expression));
}

/**
 * @brief Calculate the result of the mathematical expression with a given
 * variable value.
 *
 * This method parses the input mathematical expression, converts it to Reverse
 * Polish Notation (RPN), and then evaluates the RPN expression with the
 * provided value for the variable 'x'.
 *
 * @param expression The mathematical expression to be evaluated.
 * @param x The value of the variable 'x' in the expression.
 * @return The result of evaluating the expression with the specified variable
 * value.
 */
double MathCalc::Calculate(const std::string& expression, double x) {
  std::vector<Token> tokens = ParseExpression(expression);
  std::vector<Token> rpn = ConvertToRPN(tokens);
  return EvaluateRPN(rpn, x);
}

/**
 * @brief Calculate the result of the stored mathematical expression with a
 * given variable value.
 *
 * This method evaluates the previously stored mathematical expression (which
 * was provided during object construction) with the provided value for the
 * variable 'x'.
 *
 * @param x The value of the variable 'x' in the stored expression.
 * @return The result of evaluating the stored expression with the specified
 * variable value.
 */
double MathCalc::Calculate(double x) { return EvaluateRPN(rpn_, x); }

/**
 * @brief Parses the given expression into a vector of tokens.
 *
 * This method takes an input mathematical expression as a string and tokenizes
 * it, creating a vector of tokens that represent the expression.
 *
 * @param expression The input mathematical expression to be parsed.
 * @return A vector of tokens representing the parsed expression.
 * @throws std::invalid_argument if the expression contains invalid characters
 * or if it is missing an operator between consecutive operands or variables.
 */
std::vector<Token> MathCalc::ParseExpression(const std::string& expression) {
  std::vector<Token> tokens;
  std::size_t pos = 0;

  while (pos < expression.length()) {
    char ch = expression[pos];

    if (std::isdigit(ch)) {
      InsertOmittedMul(tokens, true);
      pos = ParseNumber(expression, pos, tokens);
    } else if (ch == '.') {
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
      if (!ValidateSpaces(expression, pos)) {
        throw std::invalid_argument("Invalid expression: missing operator");
      }
      ++pos;
    } else {
      throw std::invalid_argument("Invalid character: " + std::string(1, ch));
    }
  }

  return tokens;
}

/**
 * @brief Converts a sequence of tokens from infix notation to Reverse Polish
 * Notation (RPN) using the Shunting-Yard algorithm.
 *
 * This method takes a vector of input tokens in infix notation and transforms
 * them into the equivalent expression in RPN using the Shunting-Yard algorithm.
 *
 * @param tokens A vector of input tokens in infix notation.
 * @return A vector of tokens representing the same expression in RPN.
 * @throws std::invalid_argument if the input expression contains invalid
 * brackets or operators.
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
      ProcessBrackets(operators, rpn);
    } else if (token.IsOperator()) {
      ProcessOperators(token, operators, rpn);
    }
  }

  ProcessRemainingOperators(operators, rpn);

  return rpn;
}

/**
 * @brief Evaluates an expression in Reverse Polish Notation (RPN) given a value
 * for the variable 'x'.
 *
 * @param rpn A vector of tokens representing the expression in RPN.
 * @param x The value to substitute for the variable 'x'.
 * @return The result of evaluating the expression.
 * @throws std::invalid_argument if the RPN expression is invalid or contains
 * too few or too many operands.
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
 * @brief Parses a numeric token from the expression starting at the given
 * position.
 *
 * @param expression The expression to parse.
 * @param pos The starting position for parsing.
 * @param tokens The vector to store the parsed tokens.
 * @return The new position after parsing the number.
 * @throws std::invalid_argument if the parsed number is invalid.
 */
std::size_t MathCalc::ParseNumber(const std::string& expression,
                                  std::size_t pos, std::vector<Token>& tokens) {
  std::size_t start = pos;
  auto is_exp = [](char ch) { return std::toupper(ch) == 'E'; };
  auto is_exp_sign = [](char ch) { return ch == '+' || ch == '-'; };

  while (pos < expression.length()) {
    char ch = expression[pos];
    if (!std::isdigit(ch) && ch != '.' && !is_exp(ch)) {
      break;
    }
    if (is_exp(ch)) {
      ++pos;
      if (pos < expression.length()) {
        char next_ch = expression[pos];
        if (is_exp_sign(next_ch)) {
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
 * @brief Parses an alpha token from the expression starting at the given
 * position.
 *
 * @param expression The expression string.
 * @param pos The current position within the expression.
 * @param tokens The vector to store parsed tokens.
 * @return The new position after parsing the alpha token.
 * @throws std::invalid_argument if the parsed token is not a valid alpha token.
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
    tokens.push_back(Token(TokenType::kBinaryOperator, tok, 2));
  } else {
    InsertOmittedMul(tokens);
    tokens.push_back(Token(TokenType::kFunction, tok));
  }

  return pos;
}

/**
 * @brief Parses an operator token from the expression starting at the given
 * position.
 *
 * @param expression The expression string.
 * @param pos The current position within the expression.
 * @param tokens The vector to store parsed tokens.
 * @throws std::invalid_argument if the parsed operator is not valid.
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
 *
 * @param tokens The vector of tokens to insert the operator into.
 * @param flg A flag indicating insertion before number.
 */
void MathCalc::InsertOmittedMul(std::vector<Token>& tokens, bool flg) {
  if ((!flg && !tokens.empty() &&
       (tokens.back().IsNumber() || tokens.back().IsCloseBracket() ||
        tokens.back().IsVariable())) ||
      (flg && !tokens.empty() &&
       (tokens.back().IsCloseBracket() || tokens.back().IsVariable()))) {
    tokens.push_back(Token(TokenType::kBinaryOperator, "*", 2));
  }
}

/**
 * @brief Validates if a given string represents a valid number.
 *
 * This method checks if the input string meets the criteria of a valid number,
 * including integer and floating-point numbers, as well as scientific notation.
 * It ensures that the string contains only digits, a single dot (.), and a
 * single exponent (E) character if applicable. It also checks the correct
 * placement of the dot and exponent signs within the number.
 *
 * @param token The string to be validated.
 * @return True if the string represents a valid number, False otherwise.
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
 * @brief Validates if a given string represents a valid alphanumeric token.
 *
 * This method checks if the input string is a valid alphanumeric token, which
 * can be one of the predefined valid tokens such as mathematical functions
 * ("sin", "cos", "tan"), the "mod" operator, or user-defined variables.
 *
 * @param token The string to be validated.
 * @return True if the string represents a valid alphanumeric token, False
 * otherwise.
 */
bool MathCalc::ValidateAlpha(const std::string& token) {
  static const std::vector<std::string> valid_tokens = {
      "sin",  "cos", "tan", "asin", "acos", "atan",
      "sqrt", "ln",  "log", "x",    "mod"};

  return std::any_of(valid_tokens.cbegin(), valid_tokens.cend(),
                     [&token](const std::string& tok) { return tok == token; });
}

/**
 * @brief Validates the correct placement of spaces in an expression.
 *
 * This method checks if the spaces in the input expression are correctly
 * placed. Specifically, it ensures that spaces are not present between digits
 * or between a digit and a variable (e.g., "x 5" or "2 3"), which would result
 * in an invalid expression.
 *
 * @param expression The expression to be validated.
 * @param pos The current position in the expression where validation is being
 * performed.
 * @return True if spaces are correctly placed, False if an invalid space
 * configuration is found.
 */
bool MathCalc::ValidateSpaces(const std::string& expression, std::size_t pos) {
  std::size_t start = pos;
  while (pos < expression.length() && std::isspace(expression[pos])) {
    ++pos;
  }

  if (pos < expression.length() && start > 0 &&
      (std::isdigit(expression[start - 1]) || expression[start - 1] == 'x') &&
      (std::isdigit(expression[pos]) || expression[pos] == 'x')) {
    return false;
  }

  return true;
}

/**
 * @brief Processes and evaluates a mathematical operator.
 *
 * This method takes a mathematical operator token and the stack of operands.
 * It performs the corresponding unary or binary operation on the operands
 * based on the operator token and pushes the result back onto the operand
 * stack.
 *
 * @param token The operator token to be processed.
 * @param operands A stack containing the operands for the mathematical
 * operation.
 *
 * @throws std::invalid_argument if there are not enough operands for the
 * operator, division by zero is encountered, or if an unsupported operator is
 * encountered.
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
 * @brief Processes and evaluates a mathematical function.
 *
 * This method takes a mathematical function token and the stack of operands.
 * It performs the corresponding mathematical function on the top operand
 * and pushes the result back onto the operand stack.
 *
 * @param token The function token to be processed.
 * @param operands A stack containing the operands for the function.
 *
 * @throws std::invalid_argument if there are not enough operands for the
 * function, if an invalid input is provided to a function (e.g., sqrt of a
 * negative number), or if an unsupported function is encountered.
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

/**
 * @brief Processes and evaluates brackets and associated functions.
 *
 * This method handles the processing of closing brackets and any associated
 * functions. It pops operators from the operator stack and adds them to the
 * Reverse Polish Notation (RPN) output until an opening bracket is encountered.
 * It also checks for a corresponding function call before the opening bracket
 * and includes it in the RPN if present.
 *
 * @param operators A stack containing operators.
 * @param rpn A vector representing the Reverse Polish Notation output.
 *
 * @throws std::invalid_argument if there is an invalid bracket sequence.
 */
void MathCalc::ProcessBrackets(std::stack<Token>& operators,
                               std::vector<Token>& rpn) {
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
}

/**
 * @brief Processes and evaluates operators based on their precedence and
 * associativity.
 *
 * This method handles the processing of operators by comparing their precedence
 * and associativity with those already on the operator stack. It pops operators
 * from the operator stack and adds them to the Reverse Polish Notation (RPN)
 * output until an operator with lower precedence or left associativity is
 * encountered. The current operator is then pushed onto the operator stack.
 *
 * @param token The operator token being processed.
 * @param operators A stack containing operators.
 * @param rpn A vector representing the Reverse Polish Notation output.
 */
void MathCalc::ProcessOperators(const Token& token,
                                std::stack<Token>& operators,
                                std::vector<Token>& rpn) {
  while (!operators.empty() && operators.top().IsOperator() &&
         (token.GetPriority() < operators.top().GetPriority() ||
          (token.GetPriority() == operators.top().GetPriority() &&
           !operators.top().IsRightAssociative()))) {
    rpn.push_back(operators.top());
    operators.pop();
  }
  operators.push(token);
}

/**
 * @brief Processes and adds any remaining operators to the Reverse Polish
 * Notation (RPN) output.
 *
 * This method is responsible for processing any operators left on the operator
 * stack and adding them to the RPN output. It ensures that there are no
 * unmatched brackets, and all operators are properly included in the final RPN
 * expression.
 *
 * @param operators A stack containing operators.
 * @param rpn A vector representing the Reverse Polish Notation output.
 * @throws std::invalid_argument if an invalid bracket sequence is encountered.
 */
void MathCalc::ProcessRemainingOperators(std::stack<Token>& operators,
                                         std::vector<Token>& rpn) {
  while (!operators.empty()) {
    if (operators.top().IsOpenBracket() || operators.top().IsCloseBracket()) {
      throw std::invalid_argument("Invalid bracket sequence");
    }
    rpn.push_back(operators.top());
    operators.pop();
  }
}

}  // namespace s21
