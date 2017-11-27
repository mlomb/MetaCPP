cmake_minimum_required (VERSION 2.8)
project(MetaCPP-CLI)

set (CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../CMake")

find_package(LLVM REQUIRED)
find_Package(Clang REQUIRED)

# LLVM
link_directories(${LLVM_LIB_DIR})
add_definitions(${LLVM_COMPILE_FLAGS})

add_subdirectory(../MetaCPP MetaCPP)

set(MetaCPPCLI_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE MetaCPPCLI_SRC ${MetaCPPCLI_SRC_DIR}/*.hpp
								 ${MetaCPPCLI_SRC_DIR}/*.cpp)

add_executable(MetaCPPCLI ${MetaCPPCLI_SRC})

target_link_libraries(MetaCPPCLI MetaCPP)

# LLVM and Clang
target_link_libraries(MetaCPPCLI PUBLIC ${CLANG_LIBS} ${LLVM_LIBS_CORE} ${LLVM_LDFLAGS})
target_include_directories(MetaCPPCLI PRIVATE ${LLVM_INCLUDE_DIR} ${CLANG_INCLUDE_DIRS})
if(WIN32)
	# Annoying lib in Windows
	target_link_libraries(MetaCPPCLI version.lib)
endif()
