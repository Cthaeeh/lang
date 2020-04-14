//
// Created by kai on 4/19/19.
//

#include "Parser.h"
#include "Expr.h"
#include "Token.h"
#include <cppcmb.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include <vector>

namespace aeeh {

namespace frontend {
namespace detail {

using TokenIterator = std::vector<ast::Token>::const_iterator;

struct Range {

  const TokenIterator begin;
  const TokenIterator end;

  bool empty() const { return begin == end; };
};

using Precedence = size_t;

// TODO do this differently.
const auto SUM = size_t{3};
const auto PRODUCT = size_t{4};
const auto PREFIX = size_t{6};

using ParseProgress = std::pair<std::optional<ast::Expr>, TokenIterator>;

using PrefixParselet = std::function<ParseProgress(const Range &)>;

using InfixParselet =
    std::function<ParseProgress(const ast::Expr &, const Range &)>;

auto parse(const Range &tokens, Precedence precedence) -> ParseProgress;

auto inline noProgress(const Range &tokens) -> ParseProgress {
  return {std::nullopt, tokens.begin};
}

auto parseLiteral(const Range &tokens) -> ParseProgress {
  if (tokens.empty()) {
    return noProgress(tokens);
  } else {
    return {ast::makeLiteralExpr(*tokens.begin), tokens.begin + 1};
  }
}

auto parsePrefix(const Range &tokens) -> ParseProgress {
  if (tokens.empty()) {
    return noProgress(tokens);
  }

  auto [expr, pos] = parse({tokens.begin + 1, tokens.end}, PREFIX);

  if (expr.has_value()) {
    return {ast::makeUnaryExpr(*tokens.begin, expr.value()), pos};
  } else {
    return noProgress(tokens); // TODO error reporting,
  }
}

auto parseInfix(const ast::Expr &lhs, const Range &tokens) -> ParseProgress {
  if (tokens.empty()) {
    return noProgress(tokens);
  }

  auto [rhs, pos] =
      parse({tokens.begin + 1, tokens.end}, 23); // WHAT PRECEDENCE ???
  if (rhs.has_value()) {
    return {ast::makeBinaryExpr(lhs, *tokens.begin, rhs.value()), pos};
  } else {
    return noProgress(tokens);
  }
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

auto parse(const Range &tokens, Precedence precedence) -> ParseProgress {

  if (tokens.empty())
    return noProgress(tokens);

  // TODO use some flat map or something
  static auto prefixParslets =
      std::vector<std::tuple<ast::TokenType, PrefixParselet, Precedence>>{
          {ast::TokenType::NUMBER, parseLiteral, PREFIX},
          {ast::TokenType::MINUS, parsePrefix, PREFIX},
          {ast::TokenType::PLUS, parsePrefix, PREFIX},
          {ast::TokenType::BANG, parsePrefix, PREFIX}};

  static auto infixParslets =
      std::vector<std::tuple<ast::TokenType, InfixParselet, Precedence>>{
          {ast::TokenType::MINUS, parseInfix, SUM},
          {ast::TokenType::PLUS, parseInfix, SUM},
          {ast::TokenType::STAR, parseInfix, PRODUCT},
          {ast::TokenType::SLASH, parseInfix, PRODUCT}};

  auto token = *tokens.begin;

  auto prefix = findParselet(prefixParslets, token.type);

  auto [left, pos] = prefix ? prefix.value()(tokens) : noProgress(tokens);

  // Stop if no progress was made

  while (precedence < getPrecedence(infixParslets, (*pos).type)) {
    auto infix = findParselet(infixParslets, token.type);

    Range r = {pos, tokens.end};

    auto l = left;
    // TODO error what if no infix available ?
    auto [left, pos] = infix.value()(l.value(), r);
  }

  return {left, pos};
}

} // namespace detail

std::optional<ast::Expr> parse(const std::vector<ast::Token> &tokens) {

  return detail::parse({tokens.begin(), tokens.end()}, 0).first;
}

} // namespace frontend
} // namespace aeeh
