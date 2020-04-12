//
// Created by kai on 4/19/19.
//
#include "Lexer.h"
#include "Token.h"
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace aeeh {
namespace frontend {
namespace detail {

struct StringView {
  StringView(std::string::const_iterator begin,
             std::string::const_iterator end)
    :begin(begin),
     end(end){}

  const std::string::const_iterator begin;
  const std::string::const_iterator end;
};

// One way to make this mostly functional is to introduce this Idea of a ScanProgess.
// It is an optional token (maybe we were not able to scan anything) and a thing pointing
// to the end of the token. 
using ScanProgress = std::pair<std::optional<ast::Token>, std::string::const_iterator>; 

// Now we can say what a ScanFunction is. StringView -> ScanProgress
using ScanFunction = std::function<ScanProgress(const StringView &)>;

// This is abit hacky I think, but since we dont really have chaining optionals it makes 
// the code more readable
using CharPredicate = std::function<bool(std::optional<char>)>;


auto isDigit(std::optional<char> c) -> bool { return c >= '0' && c <= '9'; }

auto isChar(char toCheck) -> CharPredicate {
  return [toCheck](std::optional<char> c) {
    return c == toCheck;
  };
}

auto isAlpha(std::optional<char> c) -> bool {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

/// Look at the second character of the view (If it is there)
auto peek(const StringView &code) -> std::optional<char>{
  auto it = code.begin;
  it++;
  if (it != code.end)
    return *it;
  else
    return std::nullopt;
}

/// Advance as long as digits are hit.
auto scanNumber(const StringView &code) -> ScanProgress {
  if (code.begin == code.end) {
    return {std::nullopt, code.begin};
  }

  auto it = code.begin;
  while (isDigit(*it) && it != code.end) {
    it++;
  }
  auto text = std::string(code.begin, it);
  // TODO acually use the value ?
  return std::make_pair(ast::Token(ast::TokenType::NUMBER, text, 0), it);
}

/// Advance as long as Letters are hit
auto scanIdentifier(const StringView &code) -> ScanProgress {
  if (code.begin == code.end) {
    return {std::nullopt, code.begin};
  }

  auto it = code.begin;
  while (isAlpha(*it) && it != code.end) {
    it++;
  }

  auto text = std::string(code.begin, it);

  static const std::map<std::string, ast::TokenType> keywords_ = {
    {"if", ast::TokenType::IF},       {"else", ast::TokenType::ELSE},
    {"false", ast::TokenType::FALSE}, {"true", ast::TokenType::TRUE},
    {"for", ast::TokenType::FOR},     {"return", ast::TokenType::RETURN},
    {"while", ast::TokenType::WHILE}};

  // TODO use optional map acess.
  if (keywords_.find(text) != keywords_.end()) {
    return std::make_pair(ast::Token(keywords_.at(text), text, 0), it);
  } else {
    return std::make_pair(ast::Token(ast::TokenType::IDENTIFIER, text, 0), it);
  }
}

auto scanSingleCharacterToken(ast::TokenType type) -> ScanFunction {
  return [type](const StringView &code) {
    auto text = std::string(code.begin, code.begin + 1);
    return std::make_pair(ast::Token(type, text, 0),
                          code.begin + 1); // TODO get line info back
  };
}

auto scanDoubleCharacterToken(ast::TokenType type) -> ScanFunction  {
  return [type](const StringView &code) {
    auto text = std::string(code.begin, code.begin + 2);
    return std::make_pair(ast::Token(type, text, 0),
                          code.begin + 2); // TODO get line info back
  };
}

auto skipChar(const StringView &code) {
  return std::make_pair(std::nullopt, code.begin + 1);
}

auto scanToken(const StringView &code) -> ScanProgress 
{

  // TODO I am sure this can be written more nicely.
  if (code.begin == code.end) {
    return {std::nullopt, code.begin};
  }

  auto c = *code.begin;

  static const auto myMap = std::vector<std::pair<CharPredicate, ScanFunction>>

  {{isChar('('),scanSingleCharacterToken(ast::TokenType::LEFT_PAREN)},
   {isChar(')'),scanSingleCharacterToken(ast::TokenType::RIGHT_PAREN)},
   {isChar('{'),scanSingleCharacterToken(ast::TokenType::LEFT_BRACE)},
   {isChar('}'),scanSingleCharacterToken(ast::TokenType::RIGHT_BRACE)},
   {isChar(','),scanSingleCharacterToken(ast::TokenType::COMMA)},
   {isChar('.'),scanSingleCharacterToken(ast::TokenType::DOT)},
   {isChar('-'),scanSingleCharacterToken(ast::TokenType::MINUS)},
   {isChar('+'),scanSingleCharacterToken(ast::TokenType::PLUS)},
   {isChar(';'),scanSingleCharacterToken(ast::TokenType::SEMICOLON)},
   {isChar('*'),scanSingleCharacterToken(ast::TokenType::STAR)},
   {isChar('/'),scanSingleCharacterToken(ast::TokenType::SLASH)},
   {isChar(' '),skipChar},
   {isChar('\n'),skipChar},     // TODO include line infor again

   {isChar('!'), [](const StringView &code) {return ((isChar('=')(peek(code))) ? scanDoubleCharacterToken(ast::TokenType::BANG_EQUAL)(code) : scanSingleCharacterToken(ast::TokenType::BANG)(code));}}, 
   
   {isChar('='), [](const StringView &code) {return  ((isChar('=')(peek(code))) ? scanDoubleCharacterToken(ast::TokenType::EQUAL_EQUAL)(code) : scanSingleCharacterToken(ast::TokenType::EQUAL)(code));}}, 
   
   {isChar('<'), [](const StringView &code) {return  ((isChar('=')(peek(code))) ? scanDoubleCharacterToken(ast::TokenType::LESS_EQUAL)(code) : scanSingleCharacterToken(ast::TokenType::LESS)(code));}}, 
   
   {isChar('>'), [](const StringView &code) {return  ((isChar('=')(peek(code))) ? scanDoubleCharacterToken(ast::TokenType::GREATER_EQUAL)(code) : scanSingleCharacterToken(ast::TokenType::GREATER)(code));}},

   {isDigit, scanNumber},
   {isAlpha, scanIdentifier}};

  for (const auto &[charPredicate, scanFunction] : myMap) {
    if (charPredicate(c))
      return scanFunction(code);
  }
  // TODO somehow propagate here that no progress was made, because no character matched.
  return {std::nullopt,code.begin};
}

std::vector<ast::Token> lex(const std::string &source) {
  auto tokens = std::vector<ast::Token>();

  if (source.empty()) {
    return tokens;
  }

  auto pos = source.begin();
  while (pos != source.end()) {
    auto [token, newPos] = scanToken(StringView(pos, source.end()));

    if (token.has_value()) {
      tokens.push_back(token.value());
    } 

    pos = newPos;
  }
  return tokens;
}

} // namespace detail

std::vector<ast::Token> lex(const std::string &source) {
  return detail::lex(source);
}

} // namespace frontend
} // namespace aeeh
