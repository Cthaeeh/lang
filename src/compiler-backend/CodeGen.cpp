//
// Created by kai on 4/21/19.
//

#include <BinaryExpr.h>
#include <UnaryExpr.h>
#include <LiteralExpr.h>
#include "CodeGen.h"

Chunk CodeGen::generate(ExprPtr expr)
{
    chunk.clear();
    expr->accept(*this);
    return chunk;
}

void CodeGen::visit(BinaryExpr &el)
{
    el.left->accept(*this);
    el.right->accept(*this);

    switch (el.op.type()) {
        case Token::PLUS:
            chunk.appendSimpleOp(OP_ADD,el.op.line());
            break;
        case Token::MINUS:
            chunk.appendSimpleOp(OP_SUBTRACT,el.op.line());
            break;
        case Token::STAR:
            chunk.appendSimpleOp(OP_MULTIPLY,el.op.line());
            break;
        case Token::SLASH:
            chunk.appendSimpleOp(OP_DIVIDE,el.op.line());
            break;
        default:
            throw std::runtime_error("Invalid operand of binary expr -> parser what did u do ?");
    }
}

void CodeGen::visit(UnaryExpr &el)
{
    el.left->accept(*this);
    switch (el.op.type()) {
        case Token::MINUS:
            chunk.appendSimpleOp(OP_NEGATE, el.op.line());
            break;
        default:
            throw std::runtime_error("Invalid operand of binary expr -> parser what did u do ?");
    }
}

void CodeGen::visit(LiteralExpr &el)
{
    chunk.appendConstant(std::stod(el.literal.lexeme()), el.literal.line());
}
