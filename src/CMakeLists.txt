cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(compiler-frontend)
add_subdirectory(compiler-ast)

add_executable(VmTest main.cpp Debug.cpp Debug.h Chunk.cpp Chunk.h VirtualMachine.cpp VirtualMachine.h)

target_link_libraries(VmTest compiler-frontend)