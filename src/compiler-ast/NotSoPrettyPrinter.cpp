//
// Created by kai on 4/20/19.
//

#include "NotSoPrettyPrinter.h"
#include <LiteralExpr.h>
#include <BinaryExpr.h>
#include <UnaryExpr.h>

std::string NotSoPrettyPrinter::print(ExprPtr expr)
{
    expr->accept(*this);

    return cache_;
}

void NotSoPrettyPrinter::visit(BinaryExpr &el)
{
    std::string result = "[left: ";
    el.left->accept(*this);
    result += cache_;
    result += " op: " + el.op.toString();
    result += " right: ";
    el.right->accept(*this);
    result += cache_;
    result += " ]";
    cache_ = result;
}

void NotSoPrettyPrinter::visit(UnaryExpr &el)
{
    std::string result = "[left: ";
    el.left->accept(*this);
    result += cache_;
    result += " op: " + el.op.toString();
    result += " ]";
    cache_ = result;
}

void NotSoPrettyPrinter::visit(LiteralExpr &el)
{
    cache_ = "[literal: " + el.literal.toString() + "]";
}
