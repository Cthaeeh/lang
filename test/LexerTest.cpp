#include "Token.h"
#include <Lexer.h>
#include <algorithm>
#include <gtest/gtest.h>
#include <string>

namespace aeeh {

// TODO make the tests a lot more convenient to write
// TODO add lot more tests.

void typesEqual(const std::vector<ast::Token> &tokens,
                const std::vector<ast::TokenType> &types) {

  // Abuse as std::equal as zip.
  std::equal(tokens.begin(), tokens.end(), types.begin(), types.end(),
             [](const auto &token, const auto &type) {
               EXPECT_EQ(token.type, type)
                   << "Token '" << token.lexeme << "' has wrong type. (" << std::to_string(static_cast<int>(type))
                   << ")" << std::endl;
               return true;
             });
}

TEST(Lexer, number) {
  typesEqual(frontend::lex("2"), {ast::TokenType::NUMBER});
  typesEqual(frontend::lex("4222"), {ast::TokenType::NUMBER});
}

TEST(Lexer, math) {
  typesEqual(
      frontend::lex("1 + 1 * 1 / 1 - 1"),
      {ast::TokenType::NUMBER, ast::TokenType::PLUS, ast::TokenType::NUMBER,
       ast::TokenType::STAR, ast::TokenType::NUMBER, ast::TokenType::SLASH,
       ast::TokenType::NUMBER, ast::TokenType::MINUS, ast::TokenType::NUMBER});
}

TEST(Lexer, identifier) {
  typesEqual(frontend::lex("whatIsUp"), {ast::TokenType::IDENTIFIER});
  typesEqual(frontend::lex("yo"), {ast::TokenType::IDENTIFIER});
}

TEST(Lexer, functionsDefinitions) {
  typesEqual(frontend::lex("fn whatIsUp(x, y) { return x + y }"),
             {ast::TokenType::FUNCTION_DEF, ast::TokenType::IDENTIFIER,
              ast::TokenType::LEFT_PAREN, ast::TokenType::IDENTIFIER,
              ast::TokenType::COMMA, ast::TokenType::IDENTIFIER,
              ast::TokenType::RIGHT_PAREN, ast::TokenType::LEFT_BRACE,
              ast::TokenType::RETURN, ast::TokenType::IDENTIFIER,
              ast::TokenType::PLUS, ast::TokenType::IDENTIFIER,
              ast::TokenType::RIGHT_BRACE});

  typesEqual(frontend::lex("whatIsUp(2,4)"),
             {ast::TokenType::IDENTIFIER, ast::TokenType::LEFT_PAREN,
              ast::TokenType::NUMBER, ast::TokenType::COMMA,
              ast::TokenType::NUMBER, ast::TokenType::RIGHT_PAREN});
}

TEST(Lexer, functionsCalls) {
  typesEqual(frontend::lex("whatIsUp()"),
             {ast::TokenType::IDENTIFIER, ast::TokenType::LEFT_PAREN,
              ast::TokenType::RIGHT_PAREN});

  typesEqual(frontend::lex("whatIsUp(2,4)"),
             {ast::TokenType::IDENTIFIER, ast::TokenType::LEFT_PAREN,
              ast::TokenType::NUMBER, ast::TokenType::COMMA,
              ast::TokenType::NUMBER, ast::TokenType::RIGHT_PAREN});
}

// TODO add more tests.
} // namespace aeeh
