cmake_minimum_required(VERSION 3.1)
project(MetaCPP)

set(MetaCPP_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MetaCPP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE MetaCPP_SRC ${MetaCPP_INC_DIR}/MetaCPP/*.hpp
        ${MetaCPP_SRC_DIR}/MetaCPP/*.hpp
        ${MetaCPP_SRC_DIR}/MetaCPP/*.cpp)

add_library(MetaCPP ${MetaCPP_SRC})

target_include_directories(MetaCPP PUBLIC ${MetaCPP_INC_DIR} ../External/rapidjson/include)