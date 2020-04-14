#include "Token.h"
#include <Lexer.h>
#include <gtest/gtest.h>

namespace aeeh {

  // TODO make the tests a lot more convenient to write 
  // TODO add lot more tests.

TEST(Lexer, number) {
  EXPECT_EQ(frontend::lex("2").at(0).type, ast::TokenType::NUMBER);
}

}
