//
// Created by kai on 4/19/19.
//
#include "Lexer.h"
#include "Token.h"
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace aeeh {
namespace frontend {
namespace detail {

auto scanDepending(CharPredicate predicate, ScanFunction ifTrue,
                   ScanFunction ifFalse) -> ScanFunction {

  return [predicate, ifTrue, ifFalse](std::string_view code) {
    return (predicate(peek(code))) ? ifTrue(code) : ifFalse(code);
  };
}

// clang-format off

static const auto dispatchTable = std::vector<std::pair<CharPredicate, ScanFunction>>
 {
  {isDigit, scanNumber},
  {isAlpha, scanIdentifier},
  {isChar('('), scanSingleCharacterToken(ast::TokenType::LEFT_PAREN)},
  {isChar(')'), scanSingleCharacterToken(ast::TokenType::RIGHT_PAREN)},
  {isChar('{'), scanSingleCharacterToken(ast::TokenType::LEFT_BRACE)},
  {isChar('}'), scanSingleCharacterToken(ast::TokenType::RIGHT_BRACE)},
  {isChar(','), scanSingleCharacterToken(ast::TokenType::COMMA)},
  {isChar('.'), scanSingleCharacterToken(ast::TokenType::DOT)},
  {isChar('-'), scanSingleCharacterToken(ast::TokenType::MINUS)},
  {isChar('+'), scanSingleCharacterToken(ast::TokenType::PLUS)},
  {isChar(';'), scanSingleCharacterToken(ast::TokenType::SEMICOLON)},
  {isChar('*'), scanSingleCharacterToken(ast::TokenType::STAR)},
  {isChar('/'), scanSingleCharacterToken(ast::TokenType::SLASH)},
  {isChar(' '), skipChar},
  {isChar('\n'),skipChar}, // TODO include line info again
  {isChar('!'), scanDepending(isChar('='),
                              scanDoubleCharacterToken(ast::TokenType::BANG_EQUAL),
                              scanSingleCharacterToken(ast::TokenType::BANG))},
  {isChar('='), scanDepending(isChar('='),
                              scanDoubleCharacterToken(ast::TokenType::EQUAL_EQUAL),
                              scanSingleCharacterToken(ast::TokenType::EQUAL))},
  {isChar('<'), scanDepending(isChar('='),
                              scanDoubleCharacterToken(ast::TokenType::LESS_EQUAL),
                              scanSingleCharacterToken(ast::TokenType::LESS))},
  {isChar('>'), scanDepending(isChar('='),
                              scanDoubleCharacterToken(ast::TokenType::GREATER_EQUAL),
                              scanSingleCharacterToken(ast::TokenType::GREATER))}
  };

static const std::unordered_map<std::string, ast::TokenType> keywords_ = {
  {"if",      ast::TokenType::IF},       
  {"else",    ast::TokenType::ELSE},
  {"false",   ast::TokenType::FALSE}, 
  {"true",    ast::TokenType::TRUE},
  {"for",     ast::TokenType::FOR},     
  {"return",  ast::TokenType::RETURN},
  {"while",   ast::TokenType::WHILE}};

// clang-format on

auto isDigit(std::optional<char> c) -> bool { return c >= '0' && c <= '9'; }

auto isChar(char toCheck) -> CharPredicate {
  return [toCheck](std::optional<char> c) { return c == toCheck; };
}

auto isAlpha(std::optional<char> c) -> bool {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

/// Look at the second character of the view (If it is there)
auto peek(std::string_view code) -> std::optional<char> {
  if (code.size() < 2) {
    return std::nullopt;
  }
  return code[1];
}

/// Advance as long as digits are hit.
auto scanNumber(std::string_view code) -> ScanProgress {
  if (code.empty()) {
    return NoProgress;
  }

  auto it = std::find_if(code.begin(), code.end(), std::not_fn(isDigit));

  auto text = std::string(code.begin(), it);
  // TODO acually use the value ?
  return {ast::Token(ast::TokenType::NUMBER, text, 0),
          std::distance(code.begin(), it)};
}

/// Advance as long as Letters are hit
auto scanIdentifier(std::string_view code) -> ScanProgress {
  if (code.empty()) {
    return NoProgress;
  }

  auto it = std::find_if(code.begin(), code.end(), std::not_fn(isAlpha));

  auto text = std::string(code.begin(), it);

  if (keywords_.find(text) != keywords_.end()) {
    return {ast::Token(keywords_.at(text), text, 0),
            std::distance(code.begin(), it)};
  } else {
    return {ast::Token(ast::TokenType::IDENTIFIER, text, 0),
            std::distance(code.begin(), it)};
  }
}

auto scanSingleCharacterToken(ast::TokenType type) -> ScanFunction {
  return [type](std::string_view code) -> ScanProgress {
    auto text = std::string(code.begin(), code.begin() + 1);
    return {ast::Token(type, text, 0), 1};
    // TODO get line info back
  };
}

auto scanDoubleCharacterToken(ast::TokenType type) -> ScanFunction {
  return [type](std::string_view code) -> ScanProgress {
    auto text = std::string(code.begin(), code.begin() + 2);
    return {ast::Token(type, text, 0), 2}; // TODO get line info back
  };
}

auto inline skipChar(std::string_view code) -> ScanProgress {
  return {std::nullopt, 1};
}

auto scanToken(std::string_view code) -> ScanProgress {

  if (code.empty()) {
    return {std::nullopt, 1};
  }

  auto c = *code.begin();

  for (const auto &[charPredicate, scanFunction] : dispatchTable) {
    if (charPredicate(c))
      return scanFunction(code);
  }

  std::cout << "No progress made. -> PROBLEM" << std::endl;
  // TODO somehow propagate here that no progress was made, because no character
  // matched.
  return NoProgress;
}

std::vector<ast::Token> lex(std::string_view code) {
  if (code.empty()) {
    return {};
  }

  auto tokens = std::vector<ast::Token>();

  auto currentPos = 0;
  while (currentPos < code.size()) {

    auto scanProgress = scanToken(code.substr(currentPos, code.size()));

    if (scanProgress.token.has_value()) {
      tokens.push_back(scanProgress.token.value());
    }

    currentPos = currentPos + scanProgress.charsEaten;
  }
  return tokens;
}

} // namespace detail

std::vector<ast::Token> lex(std::string_view code) { return detail::lex(code); }

} // namespace frontend
} // namespace aeeh
