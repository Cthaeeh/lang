//
// Created by kai on 4/19/19.
//

#include "Parser.h"
#include "Expr.h"
#include <cppcmb.hpp>
#include <iostream>
#include <optional>


namespace aeeh {
namespace frontend {

namespace pc = cppcmb;

template <ast::TokenType type> 
bool is_same_token_type(ast::Token t) {
  return t.type == type;
}

template <ast::TokenType type>
inline constexpr auto match = pc::one[pc::filter(is_same_token_type<type>)];

cppcmb_decl(expr, ast::Expr);
cppcmb_decl(mul, ast::Expr);
cppcmb_decl(unary, ast::Expr);
cppcmb_decl(primary, ast::Expr);

cppcmb_def(expr) = pc::pass 
    | (expr & match<ast::TokenType::PLUS> & mul)[ast::makeBinaryExpr] 
    | (expr & match<ast::TokenType::MINUS> & mul)[ast::makeBinaryExpr] 
    | mul
    %= pc::as_memo_d;

cppcmb_def(mul) = pc::pass 
    | (mul & match<ast::TokenType::STAR> & unary)[ast::makeBinaryExpr] 
    | (mul & match<ast::TokenType::SLASH> & unary)[ast::makeBinaryExpr] 
    | unary
    %= pc::as_memo_d;

cppcmb_def(unary) = pc::pass 
    | (match<ast::TokenType::MINUS> & primary)[ast::makeUnaryExpr] 
    | primary
    %= pc::as_memo_d;

cppcmb_def(primary) = pc::pass 
    | (match<ast::TokenType::NUMBER>)[ast::makeLiteralExpr] 
    | (match<ast::TokenType::LEFT_PAREN> & expr & match<ast::TokenType::RIGHT_PAREN>)[pc::select<1>] 
    %= pc::as_memo_d;

std::optional<ast::Expr> parse(const std::vector<ast::Token> &tokens) {

  auto parser = pc::parser(expr);

  auto res = parser.parse(tokens);
  if (res.is_success()) {
    return res.success().value();
  } else {
    return std::nullopt;
  }
}

//std::optional<ast::Expr> parse(const std::vector<ast::Token> &tokens) {
//  return detail::parse(tokens);
//}

} // namespace frontend
} // namespace aeeh
