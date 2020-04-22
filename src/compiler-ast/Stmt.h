#ifndef VMTEST_STMT_H
#define VMTEST_STMT_H

#include "Token.h"
#include <memory>
#include <variant>
#include <vector>
#include "Expr.h"
#include <optional>

namespace aeeh {
namespace ast {

struct FuncStmt;
struct ExprStmt;
struct BlockStmt;

using Stmt = std::variant<FuncStmt, ExprStmt, BlockStmt>;
using StmtPtr = std::shared_ptr<Stmt>;
using BlockPtr = std::shared_ptr<BlockStmt>;

struct Parameter
{
  Token name;
  std::optional<Token> type;
};

struct FuncStmt{
  FuncStmt(const Token &name,
           const std::vector<Parameter> parameters,
           const BlockStmt &block)
     :name(name),
      parameters(parameters),
      block(std::make_shared<BlockStmt>(block))
  {}

  Token name;
  std::vector<Parameter> parameters;
  BlockPtr block;
};

struct BlockStmt{
  BlockStmt(const std::vector<StmtPtr> &stmts)
     :stmts(stmts)
  {}
  const std::vector<StmtPtr> stmts;
};

struct ExprStmt{
  ExprStmt(const Expr &expr)
     :expr(std::make_shared<Expr>(expr))
  {}
  ExprPtr expr;
};

}
} // namespace aeeh

#endif // VMTEST_EXPR_H
