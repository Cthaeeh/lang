#include <array>
#include <cstdio>
#include <cstdlib>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <Lexer.h>
#include <Parser.h>
#include <CodeGenAssembly.h>
#include <fstream>

namespace cg = aeeh::code_gen;

std::string exec(const std::string &command) {
  //std::array<char, 128> buffer;
  //std::string result;
  //std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"),
  //                                              pclose);
  //if (!pipe) {
  //  throw std::runtime_error("popen() failed!");
  //}
  //while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
  //  result += buffer.data();
  //}
  //return result;
  std::system((command + " > tempFile").c_str());
  std::ifstream t("tempFile");
  return std::string((std::istreambuf_iterator<char>(t)),
                      std::istreambuf_iterator<char>());
}

std::string compileAndExecute(const std::string &code)
{
  Lexer lexer(code);
  auto tokens = lexer.lex();

  Parser parser(tokens);
  auto ast = parser.parse();

	auto assembly = cg::generateAssembly(ast);
  std::ofstream out("temp_test.asm");
	out << assembly;
  out.flush();

  std::system("nasm -f elf64 -o temp_test.o temp_test.asm"); 
  std::system("ld temp_test.o -o temp_test"); 

  std::system("./temp_test > tempFile");
  std::ifstream t("tempFile");
  return std::string((std::istreambuf_iterator<char>(t)),
                      std::istreambuf_iterator<char>());

}

TEST(FullAsm, cmdLineExecution) { 
  
  EXPECT_EQ(exec("echo \"Hello\""), std::string("Hello\n")); 

}

TEST(FullAsm, addition) { 
  
  EXPECT_EQ(compileAndExecute("1 + 1"), std::string("2")); 
  EXPECT_EQ(compileAndExecute("2 + 2"), std::string("4")); 
  EXPECT_EQ(compileAndExecute("5000 + 500"), std::string("5500")); 

}


TEST(FullAsm, subtraction) { 
  
  EXPECT_EQ(compileAndExecute("1 - 1"), std::string("0")); 
  EXPECT_EQ(compileAndExecute("4 - 2"), std::string("2")); 
  EXPECT_EQ(compileAndExecute("5000 - 500"), std::string("4500")); 

}

TEST(FullAsm, multiplication) { 
  
  EXPECT_EQ(compileAndExecute("1 * 1"), std::string("1")); 
  EXPECT_EQ(compileAndExecute("2 * 2"), std::string("4")); 
  EXPECT_EQ(compileAndExecute("5 * 5"), std::string("25")); 

}

TEST(FullAsm, division) { 
  
  EXPECT_EQ(compileAndExecute("1 / 1"), std::string("1")); 
  EXPECT_EQ(compileAndExecute("9 / 3"), std::string("3")); 
  EXPECT_EQ(compileAndExecute("100 / 4"), std::string("25")); 

}

