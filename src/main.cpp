#include <iostream>

#include "Chunk.h"
#include "Debug.h"
#include "VirtualMachine.h"
#include "compiler-frontend/Lexer.h"
#include "compiler-frontend/Parser.h"
#include <BinaryExpr.h>

void testParserCombinator();


int main() {

    std::cout << "Lexer:" << std::endl;
    Lexer lexer("1+2");
    auto tokens = lexer.lex();
    for (auto t : *tokens) {
        std::cout << " " << t.toString();
    }
    std::cout << std::endl;

    Parser parser(tokens);
    auto ast = parser.parse();

    // TODO GodeGen for creating a chunk

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


