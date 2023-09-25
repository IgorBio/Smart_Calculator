#include "math_calc.h"

namespace s21 {

std::vector<Token> MathCalc::ParseExpression(const std::string& expression) {
  std::vector<Token> tokens;
  std::size_t pos = 0;

  while (pos < expression.length()) {
    char ch = expression[pos];

    if (std::isdigit(ch) || ch == '.') {
      pos = ParseNumber(expression, pos, tokens);
    } else if (std::isalpha(ch)) {
      pos = ParseAlpha(expression, pos, tokens);
    } else if (ch == '(') {
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

std::size_t MathCalc::ParseNumber(const std::string& expression,
                                  std::size_t pos, std::vector<Token>& tokens) {
  std::size_t start = pos;
  while (pos < expression.length() &&
         (std::isdigit(expression[pos]) || expression[pos] == '.')) {
    ++pos;
  }
  std::string tok = expression.substr(start, pos - start);

  if (!ValidateNumber(tok)) {
    throw std::invalid_argument("Invalid number: " + tok);
  }

  tokens.push_back(Token(TokenType::kNumber, tok));

  return pos;
}

std::size_t MathCalc::ParseAlpha(const std::string& expression, std::size_t pos,
                                 std::vector<Token>& tokens) {
  std::size_t start = pos;
  while (pos < expression.length() && std::isalpha(expression[pos]) &&
         expression.substr(start, pos - start) != "mod") {
    ++pos;
  }
  std::string tok = expression.substr(start, pos - start);

  if (!ValidateAlpha(tok)) {
    throw std::invalid_argument("Invalid token: " + tok);
  }

  if (tok == "x") {
    tokens.push_back(Token(TokenType::kVariable, tok));
  } else if (tok == "e") {
    tokens.push_back(Token(TokenType::kSci, tok));
  } else if (tok == "mod") {
    tokens.push_back(Token(TokenType::kBinaryOperator, tok));
  } else {
    tokens.push_back(Token(TokenType::kFunction, tok));
  }

  return pos;
}

std::size_t MathCalc::ParseOperator(const std::string& expression,
                                    std::size_t pos,
                                    std::vector<Token>& tokens) {
  char op = expression[pos];
  TokenType type;
  short priority;

  bool is_unary = (pos == 0 || expression[pos - 1] == '(' ||
                   tokens.back().IsBinaryOperator() || tokens.back().IsSci());

  switch (op) {
    case '+':
    case '-':
      type = is_unary ? TokenType::kUnaryOperator : TokenType::kBinaryOperator;
      priority = is_unary ? 1 : 2;
      break;
    case '*':
    case '/':
      type = TokenType::kBinaryOperator;
      priority = 3;
      break;
    case '^':
      type = TokenType::kBinaryOperator;
      priority = 4;
      break;
    default:
      throw std::invalid_argument("Invalid operator: " + std::string(1, op));
  }

  tokens.push_back(Token(type, std::string(1, op), priority));
  ++pos;

  return pos;
}

bool MathCalc::ValidateNumber(const std::string& token) {
  bool has_dot = false;
  for (char ch : token) {
    if (!std::isdigit(ch) && !(ch == '.' && !has_dot)) {
      return false;
    }
    if (ch == '.') {
      has_dot = true;
    }
  }
  return true;
}

bool MathCalc::ValidateAlpha(const std::string& token) {
  static const std::vector<std::string> valid_tokens = {
      "sin",  "cos", "tan", "asin", "acos", "atan",
      "sqrt", "ln",  "log", "x",    "e",    "mod"};

  return std::any_of(valid_tokens.cbegin(), valid_tokens.cend(),
                     [&token](const std::string& tok) { return tok == token; });
}

std::vector<Token> MathCalc::ConvertToRPN(const std::vector<Token>& tokens) {
  std::vector<Token> rpn;
  std::stack<Token> operators;

  for (const Token& token : tokens) {
    if (token.IsNumber() || token.IsVariable() || token.IsSci()) {
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
    } else if (token.IsBinaryOperator() || token.IsUnaryOperator()) {
      while (!operators.empty() &&
             (operators.top().IsBinaryOperator() ||
              (operators.top().IsUnaryOperator() &&
               token.GetPriority() <= operators.top().GetPriority()))) {
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

}  // namespace s21