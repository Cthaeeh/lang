//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_PARSER_H
#define VMTEST_PARSER_H

#include <Expr.h>
#include <optional>
#include <vector>
#include "Token.h"

namespace aeeh{
namespace frontend{

  std::optional<ast::Expr> parse(const std::vector<ast::Token> &tokens);

}
}

#endif //VMTEST_PARSER_H
