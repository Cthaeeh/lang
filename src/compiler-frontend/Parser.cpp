//
// Created by kai on 4/19/19.
//

#include "Parser.h"
#include <BinaryExpr.h>
#include <LiteralExpr.h>
#include <cppcmb.hpp>
#include <Token.h>
#include <iostream>

namespace pc = cppcmb;

Parser::Parser(const TokensConstPtr tokens) : tokens_(tokens)
{

}

template <Token::Type type>
bool is_same_token_type(Token t) { return t.type() == type; }

template <Token::Type type>
inline constexpr auto match = pc::one[pc::filter(is_same_token_type<type>)];

cppcmb_decl(expr,    ExprPtr);
cppcmb_decl(primary, ExprPtr);

cppcmb_def(expr) = pc::pass
                   | (expr & match<Token::PLUS> & primary) [BinaryExpr::make]
                   | (expr & match<Token::MINUS> & primary) [BinaryExpr::make]
                   | primary
                   %= pc::as_memo_d;

cppcmb_def(primary) =
        (match<Token::NUMBER>) [LiteralExpr::make];

ExprPtr Parser::parse(){
    auto parser = pc::parser(expr);

    auto res = parser.parse(*tokens_);
    if (res.is_success()) {
        return res.success().value();
    } else {
        throw std::runtime_error("Parsing failed");
    }
}
