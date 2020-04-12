cmake_minimum_required(VERSION 3.14)
project(VmTest)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include_directories("include/CppCmb")
include_directories("include/cxxopts")
include_directories("include/NamedType")

# gtest is not just a header that needs to be included.
add_subdirectory(include/googletest)

add_subdirectory(src)
add_subdirectory(test)

