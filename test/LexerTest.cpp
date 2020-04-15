#include "Token.h"
#include <Lexer.h>
#include <algorithm>
#include <gtest/gtest.h>

namespace aeeh {

  // TODO make the tests a lot more convenient to write 
  // TODO add lot more tests.

bool typesEqual(const std::vector<ast::Token> &tokens,
                const std::vector<ast::TokenType> &types) 
{
    return std::equal(tokens.begin(), tokens.end(),
                      types.begin(), types.end(),
                      [](const auto &token, const auto &type) 
                      {return token.type == type;});
}

TEST(Lexer, number) {
  EXPECT_TRUE(typesEqual(frontend::lex("2"), {ast::TokenType::NUMBER}));
  EXPECT_TRUE(typesEqual(frontend::lex("4222"), {ast::TokenType::NUMBER}));
}

TEST(Lexer, math) {
  EXPECT_TRUE(typesEqual(frontend::lex("1 + 1 * 1 / 1 - 1"), 
              {ast::TokenType::NUMBER,
               ast::TokenType::PLUS,
               ast::TokenType::NUMBER,
               ast::TokenType::STAR,
               ast::TokenType::NUMBER,
               ast::TokenType::SLASH,
               ast::TokenType::NUMBER,
               ast::TokenType::MINUS,
               ast::TokenType::NUMBER}));
}

TEST(Lexer, identifier) {
  EXPECT_TRUE(typesEqual(frontend::lex("whatIsUp"), {ast::TokenType::IDENTIFIER}));
  EXPECT_TRUE(typesEqual(frontend::lex("yo"), {ast::TokenType::IDENTIFIER}));
}

TEST(Lexer, functions) {
  EXPECT_TRUE(typesEqual(frontend::lex("whatIsUp()"), {ast::TokenType::IDENTIFIER,
                                                       ast::TokenType::LEFT_PAREN,
                                                       ast::TokenType::RIGHT_PAREN}));

  EXPECT_TRUE(typesEqual(frontend::lex("whatIsUp(2,4)"), 
                         {ast::TokenType::IDENTIFIER,
                          ast::TokenType::LEFT_PAREN,
                          ast::TokenType::NUMBER,
                          ast::TokenType::COMMA,
                          ast::TokenType::NUMBER,
                          ast::TokenType::RIGHT_PAREN}));
}

// TODO add more tests.
}
