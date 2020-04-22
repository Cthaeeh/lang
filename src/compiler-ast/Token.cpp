//
// Created by kai on 4/19/19.
//

#include "Token.h"

namespace aeeh{
namespace ast{

Token::Token (TokenType type,
              const std::string &lexeme,
              int line)
    :type(type),
     lexeme(lexeme),
     line(line)
{}

std::string toString(const Token &token) {
    switch (token.type) {
      default:
        return "[" + token.lexeme + "]";
    }
}

}
}
