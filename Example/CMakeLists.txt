cmake_minimum_required (VERSION 2.8)
project(MetaCPP-Example)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(../MetaCPP-CLI MetaCPP-CLI)

add_executable(Example main.cpp objects.hpp)

target_link_libraries(Example MetaCPP)

# MetaCPP automatic reflection
include(../CMake/MetaPrebuild.cmake)
meta_generate(Example "objects.hpp" "Generated.hpp" "Generated.cpp" "")
