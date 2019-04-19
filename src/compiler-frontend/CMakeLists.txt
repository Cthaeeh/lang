
add_library(compiler-frontend STATIC
            Lexer.cpp Lexer.h)

target_link_libraries(compiler-frontend compiler-ast)