#include <Parser.h>
#include <Lexer.h>
#include <algorithm>
#include <gtest/gtest.h>
#include <NotSoPrettyPrinter.h>

namespace aeeh {

  // TODO make the tests a lot more convenient to write 
  // TODO add lot more tests.

inline bool typesEqual(const std::vector<ast::Token> &tokens,
                const std::vector<ast::TokenType> &types) 
{
    return std::equal(tokens.begin(), tokens.end(),
                      types.begin(), types.end(),
                      [](const auto &token, const auto &type) 
                      {return token.type == type;});
}

inline ast::Expr lexParse(const std::string &code) 
{
  return frontend::parse(frontend::lex(code)).value();
}

TEST(Parser, basicMath) {
  EXPECT_EQ(ast::print(lexParse("1 + 1")), "(1 + 1)");
  EXPECT_EQ(ast::print(lexParse("1 * 1")), "(1 * 1)");
  EXPECT_EQ(ast::print(lexParse("1 / 1")), "(1 / 1)");
  EXPECT_EQ(ast::print(lexParse("1 - 1")), "(1 - 1)");

  EXPECT_EQ(ast::print(lexParse("1 + 1 / 1 * 1")), "(1 + ((1 / 1) * 1))");
}

TEST(Parser, precedence) {
  // Multiplication before addition.
  EXPECT_EQ(ast::print(lexParse("1 * 1 + 1")), "((1 * 1) + 1)");
  EXPECT_EQ(ast::print(lexParse("1 + 1 * 1")), "(1 + (1 * 1))");

  EXPECT_EQ(ast::print(lexParse("-1 * 1")), "(-1 * 1)");
}

TEST(Parser, grouping) {
  EXPECT_EQ(ast::print(lexParse("1 * (1 + 1)")), "(1 * (1 + 1))");
  EXPECT_EQ(ast::print(lexParse("(1 + 1) * 1")), "((1 + 1) * 1)");
}

TEST(Parser, fktDef) {
  EXPECT_EQ(ast::print(lexParse("fn f(x, y) { return x + y }")), "fn f (x, y) {\n x + y\n}");
}

}
