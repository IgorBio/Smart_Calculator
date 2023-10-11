#ifndef SMARTCALC_VIEW_VALIDATOR_H_
#define SMARTCALC_VIEW_VALIDATOR_H_

#include <QHash>
#include <QRegularExpression>
#include <QString>
#include <functional>

namespace s21 {

class Validator {
 public:
  static QString Validate(const QString& expression, const QString& token);

 private:
  static QHash<QString, std::function<QString(const QString&, const QString&)>>
      token_mapping;

  static QString Digit(const QString& expression, const QString& token);
  static QString Dot(const QString& expression, const QString& token);
  static QString Exp(const QString& expression, const QString& token);
  static QString Bracket(const QString& expression, const QString& token);
  static QString Operation(const QString& expression, const QString& token);
  static QString Function(const QString& expression, const QString& token);
  static QString Backspace(const QString& expression, const QString& token);
  static bool IsUnary(const QString& token);
  static bool IsBinary(const QString& token);
  static bool LastIsOperation(const QString& expression);
  static bool LastIsUnary(const QString& expression);
  static bool LastIsBinary(const QString& expression);
  static bool LastIsMod(const QString& expression);
  static bool PlaceUnary(const QString& expression);
  static bool PlaceBinary(const QString& expression);
  static QString LastToken(const QString& expression);
};
}  // namespace s21

#endif  // SMARTCALC_VIEW_VALIDATOR_H_
