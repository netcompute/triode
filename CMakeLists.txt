cmake_minimum_required(VERSION 3.12)
project(triode)

set(CMAKE_CXX_STANDARD 14)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

flex_target(lexer src/token.l "${CMAKE_CURRENT_BINARY_DIR}/token.cc")
bison_target(parser src/parser.y "${CMAKE_CURRENT_BINARY_DIR}/parser.cc")

include_directories(src /usr/local/include)

llvm_map_components_to_libnames(llvm_libs support core irreader)

add_executable(triode src/main.cpp src/node.h
        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS} src/node.cpp src/Table.h src/ir/generator.cpp src/ir/generator.h)

target_link_libraries(triode ${llvm_libs})