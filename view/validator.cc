#include "validator.h"

namespace s21 {

QHash<QString, std::function<QString(const QString&, const QString&)>>
    Validator::token_mapping = {
        {"1", &Validator::Digit},       {"2", &Validator::Digit},
        {"3", &Validator::Digit},       {"4", &Validator::Digit},
        {"5", &Validator::Digit},       {"6", &Validator::Digit},
        {"7", &Validator::Digit},       {"8", &Validator::Digit},
        {"9", &Validator::Digit},       {"0", &Validator::Digit},
        {"x", &Validator::Digit},       {"e", &Validator::Exp},
        {"(", &Validator::Bracket},     {")", &Validator::Bracket},
        {"+", &Validator::Operation},   {"-", &Validator::Operation},
        {"*", &Validator::Operation},   {"/", &Validator::Operation},
        {"^", &Validator::Operation},   {"mod", &Validator::Operation},
        {"sin", &Validator::Function},  {"cos", &Validator::Function},
        {"tan", &Validator::Function},  {"asin", &Validator::Function},
        {"acos", &Validator::Function}, {"atan", &Validator::Function},
        {"ln", &Validator::Function},   {"log", &Validator::Function},
        {"sqrt", &Validator::Function}, {".", &Validator::Dot},
        {"⌫", &Validator::Backspace}};

QString Validator::Validate(const QString& expression, const QString& token) {
  return token_mapping[token](expression, token);
}

QString Validator::Digit(const QString& expression, const QString& token) {
  return (expression == "0") ? token : expression + token;
}

QString Validator::Dot(const QString& expression, const QString& token) {
  QRegularExpression miss_zero("[^.0-9]$");
  if (miss_zero.match(expression).hasMatch()) {
    return expression + "0.";
  }
  QRegularExpression no_dot("\\s*\\d+\\.\\d*\\s*$");
  if (!no_dot.match(expression).hasMatch()) {
    return expression + token;
  }
  return expression;
}

QString Validator::Exp(const QString& expression, const QString& token) {
  if (expression != "0" &&
      (expression.back().isDigit() || expression.right(1) == ".")) {
    return expression + token;
  }
  return expression;
}

QString Validator::Bracket(const QString& expression, const QString& token) {
  if (expression == "0" && token == "(") {
    return token;
  }
  if ((token == ")" && (expression == "0" || expression.right(1) == "(" ||
                        LastIsOperation(expression))) ||
      expression.right(1) == "e") {
    return expression;
  }
  return expression + token;
}

QString Validator::Operation(const QString& expression, const QString& token) {
  QString modified = expression;
  if (expression == "0" && LastIsUnary(token)) {
    return token;
  } else if (LastIsUnary(expression) && IsUnary(token)) {
    modified.chop(1);
    return modified + token;
  } else if (LastIsUnary(expression) && IsBinary(token)) {
    return expression;
  } else if (LastIsBinary(expression) && IsUnary(token)) {
    return expression + token;
  } else if (LastIsBinary(expression) && IsBinary(token)) {
    LastIsMod(expression) ? modified.chop(5) : modified.chop(3);
    return modified + " " + token + " ";
  } else if (!PlaceBinary(expression) && IsBinary(token)) {
    return expression;
  } else if (PlaceUnary(expression)) {
    return expression + token;
  } else if (PlaceBinary(expression)) {
    return expression + " " + token + " ";
  }
  return expression;
}

QString Validator::Function(const QString& expression, const QString& token) {
  QString space =
      (expression.right(1) == " " || expression.right(1) == "(") ? "" : " ";
  QString text = space + token + "(";
  return (expression == "0") ? text : expression + text;
}

QString Validator::Backspace(const QString& expression, const QString& token) {
  Q_UNUSED(token);
  QString modified = expression;
  QString last = LastToken(modified);
  modified.chop(last.size());
  if (modified.isEmpty()) {
    modified = "0";
  }
  return modified.trimmed();
}

bool Validator::IsUnary(const QString& token) {
  return token == "+" || token == "-";
}

bool Validator::IsBinary(const QString& token) {
  return token == "*" || token == "/" || token == "^" || token == "mod";
}

bool Validator::LastIsOperation(const QString& expression) {
  return LastIsUnary(expression) || LastIsBinary(expression);
}

bool Validator::LastIsUnary(const QString& expression) {
  return expression.right(1) == "+" || expression.right(1) == "-";
}
bool Validator::LastIsBinary(const QString& expression) {
  return expression.right(2) == "+ " || expression.right(2) == "- " ||
         expression.right(2) == "* " || expression.right(2) == "/ " ||
         expression.right(2) == "^ " || expression.right(4) == "mod ";
}

bool Validator::LastIsMod(const QString& expression) {
  return expression.right(4) == "mod ";
}

bool Validator::PlaceUnary(const QString& expression) {
  return expression.right(1) == "(" || expression.right(1) == "e" ||
         expression.right(1) == " ";
}

bool Validator::PlaceBinary(const QString& expression) {
  return expression.right(1) != "(" && expression.right(1) != "e";
}

QString Validator::LastToken(const QString& expression) {
  static QRegularExpression regex(
      R"((\d)|([+\-*/]|\bmod\s|\bmod\b)|(ln|log|sin|cos|tan|asin|acos|atan|sqrt)|([()xe.]))");
  QRegularExpressionMatchIterator it = regex.globalMatch(expression);
  QString last_token;

  while (it.hasNext()) {
    QRegularExpressionMatch match = it.next();
    last_token = match.captured(0);
  }

  return last_token;
}

}  // namespace s21
