cmake_minimum_required(VERSION 3.21)

project(clox
    VERSION 0.0.1
    DESCRIPTION "Lox programming language written in C"
    LANGUAGES C
)

option(CLOX_DISABLE_CUSTOM_FLAGS "Disable the custom compile/link flags used by default" OFF)

if(NOT ${CLOX_DISABLE_CUSTOM_FLAGS})
    # -rdynamic
    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -Wall -Wextra -Wpedantic -Werror=vla -std=c11 -O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer")
    # set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -Wall -Wextra -Wpedantic -Werror=vla -std=c11 -O0 -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra -Wpedantic -Werror=vla -std=c11 -O3 -march=native")
endif()

###############
# Conan Setup #
###############

# include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
# conan_basic_setup(TARGETS)
# find_package(googletest CONFIG)

################
# clox library #
################

add_library(clox
    "${PROJECT_SOURCE_DIR}/clox/src/clox/commons.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/strview.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/str.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/token.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/scanner.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/expr.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/expr-visitor.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/expr-visitor-free.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/statement.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/statement-visitor.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/program.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/ast-printer.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/ast-rpn-printer.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/parser.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/value.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/env.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/interpreter.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/interpreter-expr-visitor-eval.c"
    "${PROJECT_SOURCE_DIR}/clox/src/clox/interpreter-statement-visitor-exec.c"
)

# The value 17 from this property required cmake 3.21 version
# PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/natc-commons/include/natc/commons/strview.h"
set_target_properties(clox PROPERTIES C_STANDARD 17)

target_include_directories(clox
    PRIVATE "${PROJECT_SOURCE_DIR}/clox/src"
    PUBLIC  "${PROJECT_SOURCE_DIR}/clox/include"
)

#######################
# clox cli executable #
#######################

add_executable(clox-cli "${PROJECT_SOURCE_DIR}/clox-cli/main.c")
set_target_properties(clox-cli PROPERTIES
    C_STANDARD 17
    OUTPUT_NAME "clox"
)
target_include_directories(clox-cli
    PRIVATE
        clox::INTERFACE_INCLUDE_DIRECTORIES
        #FIXME this is just temporary. export everything necessary in the include dir
        "${PROJECT_SOURCE_DIR}/clox/src"
)
target_link_libraries(clox-cli clox)

######################
# clox vm executable #
######################

add_executable(clox-vm
    "${PROJECT_SOURCE_DIR}/clox-vm/src/clox/vm/mem.c"
    "${PROJECT_SOURCE_DIR}/clox-vm/src/clox/vm/chunk.c"
    "${PROJECT_SOURCE_DIR}/clox-vm/src/clox/vm/dbg.c"
    "${PROJECT_SOURCE_DIR}/clox-vm/src/clox/vm/main.c"
)
set_target_properties(clox-vm PROPERTIES
    C_STANDARD 17
    OUTPUT_NAME "clox-vm"
)
target_include_directories(clox-vm
    PRIVATE
        clox::INTERFACE_INCLUDE_DIRECTORIES
        "${PROJECT_SOURCE_DIR}/clox/src"
)
target_link_libraries(clox-vm clox)

#########
# Tests #
#########

include(CTest)

add_executable(ast-printer.unit "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/ast-printer.unit.c")
target_include_directories(ast-printer.unit PRIVATE "${PROJECT_SOURCE_DIR}/clox/src")
target_link_libraries(ast-printer.unit clox)
add_test(NAME ast-printer.unit COMMAND "${CMAKE_CURRENT_BINARY_DIR}/ast-printer.unit")

add_executable(ast-rpn-printer.unit "${PROJECT_SOURCE_DIR}/clox/src/clox/ast/ast-rpn-printer.unit.c")
target_include_directories(ast-rpn-printer.unit PRIVATE "${PROJECT_SOURCE_DIR}/clox/src")
target_link_libraries(ast-rpn-printer.unit clox)
add_test(NAME ast-rpn-printer.unit COMMAND "${CMAKE_CURRENT_BINARY_DIR}/ast-rpn-printer.unit")
