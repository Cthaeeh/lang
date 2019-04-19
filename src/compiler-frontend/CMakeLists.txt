
add_library(compiler-frontend STATIC
            Lexer.cpp Lexer.h
            Parser.cpp Parser.h)

target_link_libraries(compiler-frontend compiler-ast)