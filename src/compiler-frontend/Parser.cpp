//
// Created by kai on 4/19/19.
//

#include "Parser.h"
#include "Expr.h"
#include "Token.h"
#include <functional>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

namespace aeeh {
namespace frontend {
namespace detail {

using TokenView = std::basic_string_view<ast::Token>;

struct ParseProgress {
  std::optional<ast::Expr> expr;
  long tokensEaten;
};

static auto NoProgress = ParseProgress{std::nullopt, 0};

using Precedence = size_t;

// TODO do this differently.
const auto SUM = Precedence{3};
const auto PRODUCT = Precedence{4};
const auto PREFIX = Precedence{6};

using PrefixParselet = std::function<ParseProgress(TokenView)>;

using InfixParselet =
    std::function<ParseProgress(const ast::Expr &, TokenView)>;

auto parse(TokenView, Precedence precedence = 0) -> ParseProgress;
auto parseGroup(TokenView tokens) -> ParseProgress;
auto parseLiteral(TokenView tokens) -> ParseProgress;
auto parsePrefix(TokenView tokens) -> ParseProgress;
auto parseBinaryExpr(Precedence precedence) -> InfixParselet;

// clang-format off
static auto prefixParslets = std::vector<std::tuple<ast::TokenType, PrefixParselet, Precedence>>{
  {ast::TokenType::LEFT_PAREN, parseGroup,               PREFIX},
  {ast::TokenType::NUMBER,     parseLiteral,             PREFIX},
  {ast::TokenType::MINUS,      parsePrefix,              PREFIX},
  {ast::TokenType::PLUS,       parsePrefix,              PREFIX},
  {ast::TokenType::BANG,       parsePrefix,              PREFIX}};

static auto infixParslets = std::vector<std::tuple<ast::TokenType, InfixParselet, Precedence>>{
  {ast::TokenType::MINUS,      parseBinaryExpr(SUM),     SUM},
  {ast::TokenType::PLUS,       parseBinaryExpr(SUM),     SUM},
  {ast::TokenType::STAR,       parseBinaryExpr(PRODUCT), PRODUCT},
  {ast::TokenType::SLASH,      parseBinaryExpr(PRODUCT), PRODUCT}};
// clang-format on

auto parseLiteral(TokenView tokens) -> ParseProgress {
  if (tokens.empty()) {
    return NoProgress;
  } else {
    return {ast::makeLiteralExpr(*tokens.begin()), 1};
  }
}

auto parseGroup(TokenView tokens) -> ParseProgress {
  if (tokens.empty()) {
    return NoProgress;
  }

  auto [expr, exprLength] = parse(tokens.substr(1));

  if (exprLength + 1 >= tokens.size() ||
      tokens.at(exprLength + 1).type != ast::TokenType::RIGHT_PAREN) {
    std::cout << "Expected ')' at end of grouping" << std::endl;
    // TODO do useful error reporting.
    return NoProgress;
  } else {
    return {expr, exprLength + 2};
  }
}

auto parsePrefix(TokenView tokens) -> ParseProgress {
  if (tokens.empty()) {
    return NoProgress;
  }

  auto [expr, exprLength] = parse(tokens.substr(1), PREFIX);

  if (expr.has_value()) {
    return {ast::makeUnaryExpr(tokens.at(0), expr.value()), exprLength + 1};
  } else {
    return NoProgress; // TODO error reporting,
  }
}

auto parseBinaryExpr(Precedence precedence) -> InfixParselet {
  return [precedence = precedence](const ast::Expr &lhs,
                                   TokenView tokens) -> ParseProgress {
    if (tokens.empty()) {
      return NoProgress;
    }

    auto [rhs, exprLength] = parse(tokens.substr(1), precedence);
    if (rhs.has_value()) {
      return {ast::makeBinaryExpr(lhs, tokens.at(0), rhs.value()),
              exprLength + 1};
    } else {
      return NoProgress;
    }
  };
}

// TODO add postfix expressions ?
template <typename T>
std::optional<T>
findParselet(std::vector<std::tuple<ast::TokenType, T, Precedence>> vec,
             ast::TokenType type) {
  for (const auto &[tokenType, parselet, _] : vec) {
    if (tokenType == type) {
      return parselet;
    }
  }
  return std::nullopt;
}

template <typename T>
Precedence
getPrecedence(std::vector<std::tuple<ast::TokenType, T, Precedence>> vec,
              ast::TokenType type) {
  for (const auto &[tokenType, _, precedence] : vec) {
    if (tokenType == type) {
      return precedence;
    }
  }
  return 0;
}

// TODO check if this can be simplified.
auto parse(TokenView tokens, Precedence precedence) -> ParseProgress {
  if (tokens.empty()) return NoProgress;

  auto token = tokens.at(0);

  auto prefixParselet = findParselet(prefixParslets, token.type);

  if (!prefixParselet.has_value()) {
      std::cout << "Found no prefix parselet for token '" + token.lexeme + "'";
  }

  auto [lhs, progress] = prefixParselet.value()(tokens); 

  while (progress < tokens.size() && 
         precedence < getPrecedence(infixParslets, tokens.at(progress).type)) {
    if (auto infixParslet = findParselet(infixParslets, tokens.at(progress).type)) {

      auto [subLhs, subProgress] = infixParslet.value()(lhs.value(), tokens.substr(progress));

      lhs = std::move(subLhs);
      progress += subProgress;

    } else {
      // TODO real error reporting.
      std::cout << "Found no infix parselet for token '" + tokens.at(progress).lexeme + "'";
      break;
    }
  }
  return {lhs, progress};
}

} // namespace detail

std::optional<ast::Expr> parse(const std::vector<ast::Token> &tokens) {
  return detail::parse(detail::TokenView(&(*tokens.begin()), tokens.size()), 0)
      .expr;
}

} // namespace frontend
} // namespace aeeh
