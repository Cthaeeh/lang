#include <iostream>
#include <Chunk.h>
#include <Debug.h>
#include "VirtualMachine.h"
#include <Lexer.h>
#include <Parser.h>
#include <CodeGen.h>
#include <CodeGenAssembly.h>
#include <BinaryExpr.h>
#include <LiteralExpr.h>
#include <NotSoPrettyPrinter.h>
#include <include/cxxopts.hpp>

namespace cg = aeeh::code_gen;

int main(int argc, char** argv) 
{
	// TODO move command line parsing to method.
	cxxopts::Options options("test", "A brief description");
	
	options.add_options()
		("a,assembly", "Output assembly instead of using the vm",
		 cxxopts::value<bool>()->default_value("true")
		)
	;
	auto result = options.parse(argc, argv);

	auto outputAssembly = result["assembly"].as<bool>();

	// TODO make this functional -> why does this need an object ???
    Lexer lexer("-2 + 23");
    auto tokens = lexer.lex();

	// TODO make this functional -> why does this need an object ???
    Parser parser(tokens);
    auto ast = parser.parse();

    NotSoPrettyPrinter printer;
    std::cout << "AST:" << std::endl << printer.print(ast) << std::endl;

	if (outputAssembly) {
		auto assembly = cg::generateAssembly(ast);
	} else {
        CodeGen codeGen;
        auto chunk = codeGen.generate(ast);

        dissassembleChunk(chunk,"test chunk");

        VirtualMachine vm;
        vm.interpret(chunk);
	}

    return 0;
}

