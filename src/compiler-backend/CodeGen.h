//
// Created by kai on 4/21/19.
//

#ifndef VMTEST_CODEGEN_H
#define VMTEST_CODEGEN_H


#include <Visitor.h>
#include <Expr.h>
#include "../compiler-bytecode/Chunk.h"

class CodeGen : public Visitor{

public:
    Chunk generate(ExprPtr expr);

    virtual void visit(BinaryExpr &el) override;
    virtual void visit(UnaryExpr &el) override;
    virtual void visit(LiteralExpr &el) override;

private:
    Chunk chunk;
};


#endif //VMTEST_CODEGEN_H
