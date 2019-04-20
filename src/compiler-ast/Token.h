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

    Token (Type type,const std::string &lexeme, int line);

    Token(const Token& token);

    Token();

    Type type() const;

    std::string toString();

private:

    // TODO make this const it must work somehow.
     Type type_;
     std::string lexeme_;
     int line_;     //TODO make this a more precise location
};


#endif //VMTEST_TOKEN_H
