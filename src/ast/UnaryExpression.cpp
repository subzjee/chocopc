#include "ast/UnaryExpression.h"
#include "ast/ASTVisitor.h"
#include "ast/ConstantExpression.h"
#include "ast/Expression.h"

namespace chocopy::ast {
template <>
std::any
UnaryExpression<ast::ConstantExpression>::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
template <>
std::any UnaryExpression<ast::Expression>::accept(ASTVisitor& visitor) const {
  return visitor.visit(*this);
}
} // namespace chocopy::ast