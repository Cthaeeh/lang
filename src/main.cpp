#include <iostream>

#include <Chunk.h>
#include <Debug.h>
#include "VirtualMachine.h"
#include <Lexer.h>
#include <Parser.h>
#include <CodeGen.h>
#include <BinaryExpr.h>
#include <LiteralExpr.h>
#include <NotSoPrettyPrinter.h>

int main() {

    auto testExpr = BinaryExpr::make(LiteralExpr::make(Token(Token::NUMBER,"1",0)),
                                     Token(Token::STAR,"*",0),
                                     LiteralExpr::make(Token(Token::NUMBER,"2",0)));

    NotSoPrettyPrinter printer1;
    std::cout << "AST:" << std::endl << printer1.print(testExpr) << std::endl;

    std::cout << "Lexer:" << std::endl;
    Lexer lexer("1/2");
    auto tokens = lexer.lex();
    for (auto t : *tokens) {
        std::cout << " " << t.toString();
    }
    std::cout << std::endl;
    std::cout << std::endl;

    Parser parser(tokens);
    auto ast = parser.parse();

    NotSoPrettyPrinter printer;
    std::cout << "AST:" << std::endl << printer.print(ast) << std::endl;

    CodeGen codeGen;
    auto chunk = codeGen.generate(ast);

    dissassembleChunk(chunk,"test chunk");

    VirtualMachine vm;
    vm.interpret(chunk);

    return 0;
}

