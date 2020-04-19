//
// Created by kai on 4/19/19.
//

#ifndef VMTEST_LEXER_H
#define VMTEST_LEXER_H

#include <string>
#include <vector>
#include <Token.h>  
#include <optional>
#include <functional>

namespace aeeh{
namespace frontend{
namespace detail{

// One way to make this mostly functional is to introduce this Idea of a
// ScanProgess. It is an optional token (maybe we were not able to scan
// anything) and a thing pointing to the end of the token.
struct ScanProgress {
  std::optional<ast::Token> token;
  std::string_view::const_iterator endOfToken;
};

// Now we can say what a ScanFunction is. StringView -> ScanProgress
using ScanFunction = std::function<ScanProgress(std::string_view)>;

// This is abit hacky I think, but since we dont really have chaining optionals
// it makes the code more readable
using CharPredicate = std::function<bool(std::optional<char>)>;

auto isDigit(std::optional<char> c) -> bool;

auto isChar(char toCheck) -> CharPredicate; 

auto isAlpha(std::optional<char> c) -> bool; 

auto peek(std::string_view code) -> std::optional<char>;

auto scanNumber(std::string_view code) -> ScanProgress;

auto scanIdentifier(std::string_view code) -> ScanProgress;

auto scanSingleCharacterToken(ast::TokenType type) -> ScanFunction;

auto scanDoubleCharacterToken(ast::TokenType type) -> ScanFunction;

auto inline skipChar(std::string_view code) -> ScanProgress;

auto scanToken(std::string_view code) -> ScanProgress;

auto lex(std::string_view code) -> std::vector<ast::Token>;

}

auto lex(std::string_view code) -> std::vector<ast::Token>;

}
}

#endif //VMTEST_LEXER_H
