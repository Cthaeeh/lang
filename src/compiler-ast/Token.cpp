//
// Created by kai on 4/19/19.
//

#include "Token.h"

Token::Token (Type type, std::string lexeme, int line)
    :type_(type),
     lexeme_(lexeme),
     line_(line)
{

}

std::string Token::toString() {
    switch (type_) {
        case EoF:
            return "[End of File]";
        default:
            return "[" + lexeme_ + "]";
    }
}
