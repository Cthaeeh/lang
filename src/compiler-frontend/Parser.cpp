//
// Created by kai on 4/19/19.
//

#include "Parser.h"
#include "Expr.h"
#include "NotSoPrettyPrinter.h"
#include "Token.h"
#include <cppcmb.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <optional>
#include <utility>
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

auto parseGroup(const Range &tokens) -> ParseProgress {
  if (tokens.empty()) {
    return noProgress(tokens);
  } else {
    auto [expr, pos] = parse({tokens.begin + 1, tokens.end}, 0);
    if(pos == tokens.end || (*pos).type != ast::TokenType::RIGHT_PAREN) {
      // TODO do useful error reporting.
      std::cout << "Missing closing ')'" << std::endl;
      return noProgress(tokens);
    } else {
      return {expr, pos + 1};
    }
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
      parse({tokens.begin + 1, tokens.end}, 0); // WHAT PRECEDENCE ???
  if (rhs.has_value()) {
    return {ast::makeBinaryExpr(lhs, *tokens.begin, rhs.value()), pos};
  } else {
    return noProgress(tokens);
  }
}

auto parseBinaryExpr(Precedence precedence) -> InfixParselet {

  return [precedence = precedence](const ast::Expr &lhs, const Range &tokens) ->ParseProgress {

    if (tokens.empty()) {
      return noProgress(tokens);
    }

    auto [rhs, pos] =
        parse({tokens.begin + 1, tokens.end}, precedence); // WHAT PRECEDENCE ???
    if (rhs.has_value()) {
      return {ast::makeBinaryExpr(lhs, *tokens.begin, rhs.value()), pos};
    } else {
      return noProgress(tokens);
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

auto parse(const Range &tokens, Precedence precedence) -> ParseProgress {
  
  std::cout << "NUMBER OF TOKENS LEFT" << std::distance(tokens.begin, tokens.end) << std::endl;
  if (tokens.empty())
    return noProgress(tokens);

  // TODO use some flat map or something
  static auto prefixParslets =
      std::vector<std::tuple<ast::TokenType, PrefixParselet, Precedence>>{
          {ast::TokenType::LEFT_PAREN, parseGroup, PREFIX},
          {ast::TokenType::NUMBER, parseLiteral, PREFIX},
          {ast::TokenType::MINUS, parsePrefix, PREFIX},
          {ast::TokenType::PLUS, parsePrefix, PREFIX},
          {ast::TokenType::BANG, parsePrefix, PREFIX}};

  static auto infixParslets =
      std::vector<std::tuple<ast::TokenType, InfixParselet, Precedence>>{
          {ast::TokenType::MINUS, parseBinaryExpr(SUM), SUM},
          {ast::TokenType::PLUS, parseBinaryExpr(SUM), SUM},
          {ast::TokenType::STAR, parseBinaryExpr(PRODUCT), PRODUCT},
          {ast::TokenType::SLASH, parseBinaryExpr(PRODUCT), PRODUCT}};

  auto token = *tokens.begin;
  std::cout << "MAIN PARSE FUNCTION TOKEN IS:" << ast::toString(token) << std::endl;

  auto prefix = findParselet(prefixParslets, token.type);

  std::cout << "MAIN PARSE FUNCTION FOUND PREFIX PARSLET:" << (prefix.has_value() ? "yes":"no") << std::endl;

  auto [left, pos] = prefix ? prefix.value()(tokens) : noProgress(tokens);

  std::cout << "By parsing the prefix we made progress:" << std::distance(tokens.begin,
                                                                          pos) << std::endl;

  // Stop if no progress was made
  if (pos == tokens.end) {
    std::cout << "Hit the end, will return " << std::endl;
    return {left,pos};
  }
  std::cout << "MAIN PARSE FUNCTION TOKEN FOR INFIX PARSING IS:" << ast::toString(*pos) << std::endl;

  while (precedence < getPrecedence(infixParslets, (*pos).type)) {
    auto infix = findParselet(infixParslets, (*pos).type);

    std::cout << "MAIN PARSE FUNCTION FOUND INFIX PARSLET:" << (infix.has_value() ? "yes":"no") << std::endl;

    Range r = {pos, tokens.end};

    auto l = left;
    // TODO error what if no infix available ?
    auto [left_2, pos_2] = infix.value()(l.value(), r);

    left = std::move(left_2);
    pos = pos_2;

    if (pos == tokens.end) {
      std::cout << "Hit the end, will return 2" << std::endl;
      return {left,pos};
    }

    std::cout << "By parsing the infix we made progress:" << std::distance(tokens.begin,
                                                                           pos) << std::endl;
  }

  return {left, pos};
}

} // namespace detail

std::optional<ast::Expr> parse(const std::vector<ast::Token> &tokens) {

  return detail::parse({tokens.begin(), tokens.end()}, 0).first;
}

} // namespace frontend
} // namespace aeeh
