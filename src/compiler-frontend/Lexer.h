//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_LEXER_H
#define VMTEST_LEXER_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <Token.h>  //TODO fix this so that i dont have the write relative path like that.

/**
 * Ultra basic lexer
 * TODO unicode support
 * TODO support literals
 */

typedef std::shared_ptr<std::vector<Token>> TokensPtr;
typedef std::shared_ptr<const std::vector<Token>> TokensConstPtr;
class Lexer {

public:
    Lexer(const std::string &source);

    TokensConstPtr lex();

private:

    void scanToken();

    bool isAtEnd();
    char advance();
    void addToken(Token::Type type);
    bool match(char expected);
    static bool isDigit(char c);
    void number();
    char peek();
    void identifier();
    static bool isAlpha(char c);

    const std::string source_;
    const TokensPtr tokens_;
    int start = 0;
    int current = 0;
    int line = 0;

    static const std::map<std::string, Token::Type > keywords_;

};


#endif //VMTEST_LEXER_H
