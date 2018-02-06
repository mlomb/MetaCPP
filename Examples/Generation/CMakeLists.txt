cmake_minimum_required (VERSION 2.8)


add_executable(Example-Generation main.cpp)

target_link_libraries(Example-Generation MetaCPP)

include(../../CMake/MetaPrebuild.cmake)
meta_generate(Example-Generation "test.hpp" "Generated.hpp" "Generated.cpp" "")