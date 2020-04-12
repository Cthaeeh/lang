//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_LEXER_H
#define VMTEST_LEXER_H

#include <string>
#include <vector>
#include <Token.h>  

/**
 * Ultra basic lexer
 * TODO unicode support
 * TODO support literals
 */
namespace aeeh{
namespace frontend{

  std::vector<ast::Token> lex(const std::string &code);

}
}

#endif //VMTEST_LEXER_H
