#pragma once

#include "ast/ConstantExpression.h"
#include "ast/Expression.h"
#include "lexer/Token.h"

#include <memory>
#include <type_traits>

namespace chocopy::ast {

template <typename ExpressionKind>
  requires std::is_same_v<std::decay_t<ExpressionKind>, ast::Expression> ||
           std::is_same_v<std::decay_t<ExpressionKind>, ast::ConstantExpression>
class BinaryExpression : public ExpressionKind {
public:
  BinaryExpression(std::unique_ptr<ExpressionKind> lhs, const Token& op,
                   std::unique_ptr<ExpressionKind> rhs)
      : m_lhs(std::move(lhs)), m_op(op), m_rhs(std::move(rhs)) {
#ifndef NDEBUG
    if constexpr (std::is_same_v<std::decay_t<ExpressionKind>,
                                 ast::Expression>) {
      assert(op.getType() == TokenType::AND || op.getType() == TokenType::OR);
    } else {
      assert(op.isBinOp() && op.getType() != TokenType::AND &&
             op.getType() != TokenType::OR);
    }
#endif
  };

  [[nodiscard]] const std::unique_ptr<ExpressionKind>& getLHS() const {
    return m_lhs;
  };
  [[nodiscard]] const Token& getOperator() const { return m_op; }
  [[nodiscard]] const std::unique_ptr<ExpressionKind>& getRHS() const {
    return m_rhs;
  };

  [[nodiscard]] llvm::SMRange getLocation() const override {
    return {m_lhs->getLocation().Start, m_rhs->getLocation().End};
  }

  std::any accept(ASTVisitor& visitor) const override;

private:
  std::unique_ptr<ExpressionKind> m_lhs;
  const Token& m_op;
  std::unique_ptr<ExpressionKind> m_rhs;
};
} // namespace chocopy::ast