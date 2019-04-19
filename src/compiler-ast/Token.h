//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_TOKEN_H
#define VMTEST_TOKEN_H

#include <string>

class Token {

public:
    enum Type {
        // Single-character tokens.
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

        // One or two character tokens.
        BANG, BANG_EQUAL,
        EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL,
        LESS, LESS_EQUAL,

        // Literals.
        IDENTIFIER, STRING, NUMBER,

        // Keywords.
        IF, ELSE, TRUE, FALSE, FOR,
        RETURN, WHILE,

        EoF
    };

    Token (Type type, std::string lexeme, int line);

    std::string toString();

private:

    const Type type_;
    const std::string lexeme_;
    const int line_;     //TODO make this a more precise location
};


#endif //VMTEST_TOKEN_H
