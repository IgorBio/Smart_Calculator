#ifndef SMARTCALC_MODEL_TOKEN_H_
#define SMARTCALC_MODEL_TOKEN_H_

#include <string>

namespace s21 {

enum class TokenType {
  kNumber,
  kVariable,
  kOpenBracket,
  kCloseBracket,
  kUnaryOperator,
  kBinaryOperator,
  kFunction
};

/**
 * @class Token
 * @brief Token represents a unit of a mathematical expression.
 *
 * Token is used to encapsulate individual elements of a mathematical
 * expression, such as numbers, variables, operators, brackets, and functions.
 * It provides methods for querying the type, value, and priority of the token.
 *
 * The class is designed to work in conjunction with the MathCalc class for
 * parsing, converting, and evaluating mathematical expressions.
 */
class Token {
 public:
  Token(TokenType type, const std::string& token, short priority = 0)
      : type_(type), token_(token), priority_(priority) {}

  TokenType GetType() const { return type_; }
  const std::string& GetToken() const { return token_; }
  short GetPriority() const { return priority_; }

  bool IsNumber() const { return type_ == TokenType::kNumber; }
  bool IsVariable() const { return type_ == TokenType::kVariable; }
  bool IsOpenBracket() const { return type_ == TokenType::kOpenBracket; }
  bool IsCloseBracket() const { return type_ == TokenType::kCloseBracket; }
  bool IsUnaryOperator() const { return type_ == TokenType::kUnaryOperator; }
  bool IsBinaryOperator() const { return type_ == TokenType::kBinaryOperator; }
  bool IsOperator() const { return IsUnaryOperator() || IsBinaryOperator(); }
  bool IsFunction() const { return type_ == TokenType::kFunction; }
  bool IsRightAssociative() const { return token_ == "^"; }

 private:
  TokenType type_;
  std::string token_;
  short priority_;
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_TOKEN_H_
