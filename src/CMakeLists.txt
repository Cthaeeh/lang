add_subdirectory(compiler-frontend)
add_subdirectory(compiler-backend)
add_subdirectory(compiler-ast)
add_subdirectory(compiler-bytecode)

add_executable(VmTest main.cpp VirtualMachine.cpp VirtualMachine.h)

target_link_libraries(VmTest compiler-frontend)
target_link_libraries(VmTest compiler-backend)
target_link_libraries(VmTest compiler-ast)
target_link_libraries(VmTest compiler-bytecode)

target_include_directories(VmTest PRIVATE compiler-ast/)
target_include_directories(VmTest PRIVATE compiler-ast/auto-generated)
target_include_directories(VmTest PRIVATE compiler-bytecode/)
target_include_directories(VmTest PRIVATE compiler-frontend/)
target_include_directories(VmTest PRIVATE compiler-backend/)
