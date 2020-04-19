//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_EXPR_H
#define VMTEST_EXPR_H

#include <variant>
#include <memory>
#include "Token.h"

namespace aeeh {
namespace ast {

struct BinaryExpr;
struct UnaryExpr;
struct LiteralExpr;

using Expr = std::variant<BinaryExpr, UnaryExpr, LiteralExpr>;
using ExprPtr = std::shared_ptr<Expr>;

struct BinaryExpr{
  BinaryExpr(const Expr &left,
             const Token &op,
             const Expr &right)
    :left(std::make_shared<Expr>(left)),
     op(op),
     rigth(std::make_shared<Expr>(right))
  {}
     
  ExprPtr left;
  Token op;       // TODO think about if this could be further typified.
  ExprPtr rigth;
};


struct LiteralExpr{
  LiteralExpr(const Token &literal)
     :literal(literal)
  {}
  Token literal; 
};

struct UnaryExpr{
  UnaryExpr(const Token &op,
            const Expr &right)
     :op(op),
      right(std::make_shared<Expr>(right))
  {}
  Token op; 
  ExprPtr right;
};

inline Expr makeBinaryExpr(const Expr& left, const Token &op, const Expr& right)
{
  return BinaryExpr(left, op, right);
}

inline Expr makeLiteralExpr(const Token &literal)
{
  return LiteralExpr(literal);
}

inline Expr makeUnaryExpr(const Token &op, const Expr &right)
{
  return UnaryExpr(op, right);
}

}
}

#endif //VMTEST_EXPR_H
