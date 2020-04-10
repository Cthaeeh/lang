cmake_minimum_required(VERSION 3.14)
project(VmTest)
set(CMAKE_CXX_STANDARD 17)

include_directories("include/CppCmb")
include_directories("include/cxxopts")
add_subdirectory(src)
