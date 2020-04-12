cmake_minimum_required(VERSION 3.14)
project(VmTest)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
# Export compile commands so clangd (auto-completion and such) works correctly.

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

include_directories("include/CppCmb")
include_directories("include/cxxopts")
include_directories("include/NamedType")

# gtest is not just a header that needs to be included.
add_subdirectory(include/googletest)

add_subdirectory(src)
add_subdirectory(test)

