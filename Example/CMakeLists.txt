cmake_minimum_required (VERSION 2.8)
project(MetaCPP-Example)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(../MetaCPP-CLI MetaCPP-CLI)

add_executable(Example main.cpp objects.hpp)

target_link_libraries(Example MetaCPP)

# MetaCPP automatic reflection
include(../CMake/MetaPrebuild.cmake)

if(NOT DEFINED META_GENERATE_FLAGS)
    set(META_GENERATE_FLAGS "")
endif()
if(DEFINED CLANG_INCLUDE_DIR)
    set(META_GENERATE_FLAGS "${META_GENERATE_FLAGS} --flag -I${CLANG_INCLUDE_DIR}")
else()
    # Fire all weapons
    set(CLANG_INCLUDE_CANDIDATES "/usr/local/lib/clang/12.0.0/include")
    set(CLANG_INCLUDE_CANDIDATES "${CLANG_INCLUDE_CANDIDATES};/usr/lib/llvm-12/lib/clang/12.0.0/include")
    set(CLANG_INCLUDE_CANDIDATES "${CLANG_INCLUDE_CANDIDATES};/usr/local/lib/clang/11.0.0/include")
    set(CLANG_INCLUDE_CANDIDATES "${CLANG_INCLUDE_CANDIDATES};/usr/lib/llvm-11/lib/clang/11.0.0/include")
    set(CLANG_INCLUDE_CANDIDATES "${CLANG_INCLUDE_CANDIDATES};/usr/local/lib/clang/10.0.0/include")
    set(CLANG_INCLUDE_CANDIDATES "${CLANG_INCLUDE_CANDIDATES};/usr/lib/llvm-10/lib/clang/10.0.0/include")
    foreach(CLANG_INCLUDE_CANDIDATE ${CLANG_INCLUDE_CANDIDATES})
        if(EXISTS ${CLANG_INCLUDE_CANDIDATE})
            set(META_GENERATE_FLAGS "${META_GENERATE_FLAGS} --flag -I${CLANG_INCLUDE_CANDIDATE}")
            break()
        endif()
    endforeach()
endif()

meta_generate(Example "objects.hpp" "Generated.hpp" "Generated.cpp" "${META_GENERATE_FLAGS}")
