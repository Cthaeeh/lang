//
// Created by kai on 4/19/19.
//
#include "Lexer.h"

const std::map<std::string, Token::Type > Lexer::keywords_ =  {{"if",Token::IF},
                                                               {"else",Token::ELSE},
                                                               {"false",Token::FALSE},
                                                               {"true",Token::TRUE},
                                                               {"for",Token::FOR},
                                                               {"return",Token::RETURN},
                                                               {"while",Token::WHILE}};

Lexer::Lexer(const std::string &source)
:source_(source),
 tokens_(std::make_shared<std::vector<Token>>())
{

}

TokensConstPtr Lexer::lex()
{
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    //tokens_->push_back(Token(Token::EoF,"",line));
    return tokens_;
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(Token::LEFT_PAREN); break;
        case ')': addToken(Token::RIGHT_PAREN); break;
        case '{': addToken(Token::LEFT_BRACE); break;
        case '}': addToken(Token::RIGHT_BRACE); break;
        case ',': addToken(Token::COMMA); break;
        case '.': addToken(Token::DOT); break;
        case '-': addToken(Token::MINUS); break;
        case '+': addToken(Token::PLUS); break;
        case ';': addToken(Token::SEMICOLON); break;
        case '*': addToken(Token::STAR); break;
        case '/': addToken(Token::SLASH); break;

        case ' ':
            //Ignore whitespace;
            break;

        case '\n':
            line++;
            break;

        case '!': addToken(match('=') ? Token::BANG_EQUAL : Token::BANG); break;
        case '=': addToken(match('=') ? Token::EQUAL_EQUAL : Token::EQUAL); break;
        case '<': addToken(match('=') ? Token::LESS_EQUAL : Token::LESS); break;
        case '>': addToken(match('=') ? Token::GREATER_EQUAL : Token::GREATER); break;


        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c))  {
                identifier();
            } else {
                throw std::runtime_error(
                        std::string("Unexpected character '") + c + "'");   
                // TODO serious error propagation, dont just kill the app.
            }
    }

}

bool Lexer::isAtEnd() {
    return current >= source_.length();
}

char Lexer::advance()
{
    current++;
    return source_.at(current -1);  //TODO unicode support !!!
}

void Lexer::addToken(Token::Type type)
{
    auto text = source_.substr(start, (current - start));
    tokens_->push_back(Token(type, text, line));
}

bool Lexer::match(char expected)
{
    if (isAtEnd()) return false;
    if (source_.at(current) != expected) return false;
    current++;
    return true;
}

bool Lexer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

void Lexer::number()
{
    while(isDigit(peek())) advance();
    int val = std::stoi(source_.substr(start, (start - current)));
    addToken(Token::NUMBER);    // TODO add literal.
}

char Lexer::peek()
{
    if (isAtEnd()) return '\0';
    return source_.at(current);
}

void Lexer::identifier()
{
    while(isAlpha(peek())) advance();

    auto text = source_.substr(start, (start - current));

    if (keywords_.find(text) != keywords_.end()) {
        addToken(keywords_.at(text));
    } else {
        addToken(Token::IDENTIFIER);
    }
}

bool Lexer::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z')  ||
           (c >= 'A' && c <= 'Z')  ||
            c == '_';
}

