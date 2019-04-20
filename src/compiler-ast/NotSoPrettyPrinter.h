//
// Created by kai on 4/20/19.
//

#ifndef VMTEST_NOTSOPRETTYPRINTER_H
#define VMTEST_NOTSOPRETTYPRINTER_H


#include <Visitor.h>
#include "Expr.h"

class NotSoPrettyPrinter : public Visitor{
public:
    std::string print(ExprPtr expr);

    virtual void visit(BinaryExpr &el) override;
    virtual void visit(UnaryExpr &el) override;
    virtual void visit(LiteralExpr &el) override;

private:
    std::string cache_ = "";
};



#endif //VMTEST_NOTSOPRETTYPRINTER_H
