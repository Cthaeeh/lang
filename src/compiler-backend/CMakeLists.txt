
add_library(compiler-backend STATIC
            CodeGen.cpp CodeGen.h)

target_link_libraries(compiler-backend compiler-ast)
target_link_libraries(compiler-frontend compiler-bytecode)

target_include_directories(compiler-backend PRIVATE ../compiler-ast)
target_include_directories(compiler-backend PRIVATE ../compiler-ast/auto-generated)
