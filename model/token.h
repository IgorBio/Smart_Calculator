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
  bool IsFunction() const { return type_ == TokenType::kFunction; }

 private:
  TokenType type_;
  std::string token_;
  short priority_;
};
}  // namespace s21

#endif  // SMARTCALC_MODEL_TOKEN_H_
