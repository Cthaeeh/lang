file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cpp)

add_executable(Test ${TEST_SOURCES})

target_link_libraries(Test compiler-frontend)
target_link_libraries(Test compiler-backend)
target_link_libraries(Test compiler-ast)
target_link_libraries(Test compiler-bytecode)
target_link_libraries(Test gtest)

target_include_directories(Test PRIVATE ../src/compiler-ast/)
target_include_directories(Test PRIVATE ../src/compiler-ast/auto-generated)
target_include_directories(Test PRIVATE ../src/compiler-bytecode/)
target_include_directories(Test PRIVATE ../src/compiler-frontend/)
target_include_directories(Test PRIVATE ../src/compiler-backend/)
