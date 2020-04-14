#include "VirtualMachine.h"
#include <Chunk.h>
#include <CodeGen.h>
#include <CodeGenAssembly.h>
#include <Debug.h>
#include <Lexer.h>
#include <NotSoPrettyPrinter.h>
#include <Parser.h>
#include <fstream>
#include <include/cxxopts.hpp>
#include <iostream>
#include <string>

namespace cg = aeeh::code_gen;

int main(int argc, char **argv) {
  // TODO move command line parsing to method.
  cxxopts::Options options("test", "A brief description");

  options.add_options()("a,assembly", "Output assembly instead of using the vm",
                        cxxopts::value<bool>()->default_value("true"));
  auto result = options.parse(argc, argv);

  auto outputAssembly = result["assembly"].as<bool>();

  // TODO get ur output from file /cmd line.
  auto tokens = aeeh::frontend::lex("2 + 2");

  auto ast = aeeh::frontend::parse(tokens);

  if (!ast.has_value()) {
    std::cout << "No AST generated" << std::endl;
    return 1;
  }

  std::cout << "AST:" << std::endl << aeeh::ast::print(ast.value()) << std::endl;

  if (outputAssembly) {
    auto assembly = cg::generateAssembly(ast.value());
    std::ofstream out("output.asm");
    out << assembly;
  } else {
    auto chunk = aeeh::backend::generate(ast.value());

    dissassembleChunk(chunk, "test chunk");

    VirtualMachine vm;
    vm.interpret(chunk);
  }
  return 0;
}
