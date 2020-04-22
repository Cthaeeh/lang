//
// Created by kai on 4/20/19.
//

#ifndef VMTEST_NOTSOPRETTYPRINTER_H
#define VMTEST_NOTSOPRETTYPRINTER_H

#include "Expr.h"
#include "Stmt.h"

namespace aeeh {
namespace ast {

std::string print(const Stmt &stmt);

std::string print(const FuncStmt &stmt);

std::string print(const BlockStmt &stmt);

std::string print(const ExprStmt &stmt);

std::string print(const Expr &expr);

std::string print(const BinaryExpr &el);

std::string print(const UnaryExpr &el);

std::string print(const LiteralExpr &el);

}
}

#endif //VMTEST_NOTSOPRETTYPRINTER_H
