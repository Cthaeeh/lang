//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_TOKEN_H
#define VMTEST_TOKEN_H

#include <string>

namespace aeeh {
namespace ast {

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  IF,
  ELSE,
  TRUE,
  FALSE,
  FOR,
  RETURN,
  WHILE,

  EoF
};

struct Token {

Token(TokenType type,
      const std::string &lexeme,
      int line);

TokenType type;
std::string lexeme;
int line;  // TODO make this a Location

};

std::string toString(const Token &token);

} // namespace ast
} // namespace aeeh

#endif // VMTEST_TOKEN_H
