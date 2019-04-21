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
cppcmb_decl(mul,     ExprPtr);
cppcmb_decl(primary, ExprPtr);

cppcmb_def(expr) = pc::pass
                   | (expr & match<Token::PLUS> & mul) [BinaryExpr::make]
                   | (expr & match<Token::MINUS> & mul) [BinaryExpr::make]
                   | mul
                   %= pc::as_memo_d;

cppcmb_def(mul) = pc::pass
                   | (mul & match<Token::STAR> & primary) [BinaryExpr::make]
                   | (mul & match<Token::SLASH> & primary) [BinaryExpr::make]
                   | primary
                   %= pc::as_memo_d;

cppcmb_def(primary) = pc::pass
                   | (match<Token::NUMBER>) [LiteralExpr::make]
                   | (match<Token::LEFT_PAREN> & expr & match<Token::RIGHT_PAREN>) [pc::select<1>]
                   %= pc::as_memo_d;

ExprPtr Parser::parse(){
    auto parser = pc::parser(expr);

    auto res = parser.parse(*tokens_);
    if (res.is_success()) {
        std::cout << "Parse success" << std::endl;
        return res.success().value();
    } else {
        throw std::runtime_error("Parsing failed");
    }
}
