#include <iostream>

#include "Chunk.h"
#include "Debug.h"
#include "VirtualMachine.h"
#include <cppcmb.hpp>
#include "compiler-frontend/Lexer.h"

void testParserCombinator();

namespace pc = cppcmb;

int main() {

    std::cout << "Lexer:" << std::endl;
    Lexer lexer("1+2");
    auto tokens = lexer.lex();
    for (auto t : *tokens) {
        std::cout << t.toString();
    }
    std::cout << std::endl;


    testParserCombinator();

    Chunk testChunk;
    testChunk.appendConstant(1.2, 123);
    testChunk.appendConstant(1.2, 123);
    testChunk.appendSimpleOp(OP_ADD, 123);

    dissassembleChunk(testChunk,"test chunk");

    VirtualMachine vm;
    vm.interpret(testChunk);

    return 0;
}

struct Node{
    int val;
    char op;
    Node* rhs;
    Node* lhs;

    Node(Node* lhs, char op, Node* rhs):
        val(0), lhs(lhs), op(op), rhs(rhs) {}

    Node(int val):
        val(val), lhs(nullptr), op('#'), rhs(nullptr) {

    }

    std::string toString() {
        if (rhs != nullptr && lhs != nullptr) {
            return "lhs" + lhs->toString() + op +  "rhs" + rhs->toString();
        } else {
            return std::to_string(val);
        }
    }
};

template <char Ch>
bool is_same_char(char c) { return c == Ch; }

template <char Ch>
inline constexpr auto match = pc::one[pc::filter(is_same_char<Ch>)];

Node* makeBinary(Node* rhs, char op, Node* lhs) {
    return new Node(lhs,op,rhs);
}

Node* toNum(std::vector<char> const& chs) {
    int n = 0;
    for (auto c : chs) n = n * 10 + (c - '0');
    return new Node(n);
}


cppcmb_decl(expr,   Node*);
cppcmb_decl(num,    Node*);
cppcmb_decl(digit,  char);

cppcmb_def(expr) = pc::pass
        | (expr & match<'+'> & num) [makeBinary]
        | (expr & match<'-'> & num) [makeBinary]
        | num
        %= pc::as_memo_d;

cppcmb_def(num) =
        (+digit) [toNum];

cppcmb_def(digit) = pc::one[pc::filter(isdigit)];

void testParserCombinator() {
    auto parser = pc::parser(expr);

    auto res = parser.parse("1+13");
    if (res.is_success()) {
        std::cout << res.success().value()->toString() << std::endl;
    } else {
        std::cout << "parse fail" << std::endl;
    }
}