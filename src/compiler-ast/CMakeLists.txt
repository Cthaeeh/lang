
find_package(PythonInterp 3.5 REQUIRED)

execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/genAst.py --headers
        OUTPUT_VARIABLE OUTPUT_FILES
        RESULT_VARIABLE RETURN_VALUE
)


if (NOT RETURN_VALUE EQUAL 0)
    message(FATAL_ERROR "Failed to get the names of the files the python script will generate")
endif()
message(STATUS "python generated files: ${OUTPUT_FILES}")

add_custom_command(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/genAst.py --out "${CMAKE_CURRENT_SOURCE_DIR}/auto-generated/"
        DEPENDS AstNode.h Expr.h ${CMAKE_CURRENT_SOURCE_DIR}/scripts/genAst.py
        OUTPUT ${OUTPUT_FILES}
        COMMENT "generating AST with pyhton!"
        )

add_library(compiler-ast STATIC
            Token.cpp Token.h
            AstNode.h Expr.h
            NotSoPrettyPrinter.cpp NotSoPrettyPrinter.h
            ${OUTPUT_FILES} )

target_include_directories(compiler-ast PRIVATE auto-generated/)




