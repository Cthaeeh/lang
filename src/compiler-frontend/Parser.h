//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_PARSER_H
#define VMTEST_PARSER_H

#include <Expr.h>
#include "Lexer.h"

class Parser {
public:

    Parser(const TokensConstPtr tokens);

    std::shared_ptr<Expr> parse();

private:

    const TokensConstPtr tokens_;
};


#endif //VMTEST_PARSER_H
