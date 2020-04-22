//
// Created by kai on 4/20/19.
//

#include "NotSoPrettyPrinter.h"
#include <numeric>
#include <optional>

namespace aeeh {
namespace ast {

// TODO move this to some utility header.
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20


auto print(const Parameter &parameter) -> std::string
{
  auto result = parameter.name.lexeme;
  if (parameter.type.has_value()) {
    result += " " + parameter.type.value().lexeme;
  }
  return result;
}


auto print(const Stmt &stmt) -> std::string
{
  auto result = std::string();
  std::visit(overloaded {
            [&result](const auto &e) { result = print(e); },
        }, stmt);
  return result;
}

auto print(const FuncStmt &stmt) -> std::string
{
  auto result = "fn " + stmt.name.lexeme + "(";

  if (! stmt.parameters.empty()) {
    // Perform a sweet left fold.
    result += std::accumulate(stmt.parameters.begin(),
                    stmt.parameters.end(),
                    print(stmt.parameters[0]),
                    [](std::string s, Parameter p) {
                      return std::move(s) + ", " + print(p); 
                    });
  }
  result += ")";
  result += print(*(stmt.block));
  return result;
}

std::string print(const BlockStmt &stmt)
{
  return "{" + (stmt.stmts.empty() ? "" : ("\n" +
                    std::accumulate(stmt.stmts.begin(),
                                    stmt.stmts.end(),
                                    print(*(stmt.stmts[0])),
                                    [](std::string s, const StmtPtr &stmt) {
                                    return std::move(s) + ", " + print(*stmt); 
                    }) + "\n"))
          + "}";
}

std::string print(const ExprStmt &stmt)
{
  return print(*(stmt.expr));
}

std::string print(const Expr &expr) {
  auto result = std::string();
  std::visit(overloaded {
            [&result](const auto &e) { result = print(e); },
        }, expr);
  return result;
}

std::string print(const BinaryExpr &el) {
  return "(" + print(*(el.left)) + " " + el.op.lexeme +
         " " + print(*(el.rigth)) + ")";
}

std::string print(const UnaryExpr &el) {
  return  el.op.lexeme + print(*(el.right));
}

std::string print(const LiteralExpr &el) {
  return el.literal.lexeme;
}

} // namespace ast
} // namespace aeeh
