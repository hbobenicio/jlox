cmake_minimum_required(VERSION 3.21)

project(clox
    VERSION 0.0.1
    DESCRIPTION "Lox programming language written in C"
    LANGUAGES C
)

option(CLOX_DISABLE_CUSTOM_FLAGS "Disable the custom compile/link flags used by default" OFF)

if(NOT ${CLOX_DISABLE_CUSTOM_FLAGS})
    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -Wall -Wextra -Wpedantic -Werror=vla -O0 -ggdb -fsanitize=address,undefined -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -Wextra -Wpedantic -Werror=vla -O3 -march=native")
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
    "${PROJECT_SOURCE_DIR}/clox/commons.c"
    "${PROJECT_SOURCE_DIR}/clox/strview.c"
    "${PROJECT_SOURCE_DIR}/clox/str.c"
    "${PROJECT_SOURCE_DIR}/clox/token.c"
    "${PROJECT_SOURCE_DIR}/clox/scanner.c"
    "${PROJECT_SOURCE_DIR}/clox/expr.c"
    "${PROJECT_SOURCE_DIR}/clox/ast-printer.c"
    "${PROJECT_SOURCE_DIR}/clox/ast-rpn-printer.c"
    "${PROJECT_SOURCE_DIR}/clox/parser.c"
    "${PROJECT_SOURCE_DIR}/clox/interpreter.c"
)

# The value 17 from this property required cmake 3.21 version
# PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/natc-commons/include/natc/commons/strview.h"
set_target_properties(clox PROPERTIES C_STANDARD 17)

# PRIVATE
#     "${PROJECT_SOURCE_DIR}/natc-commons/src"
target_include_directories(clox PUBLIC "${PROJECT_SOURCE_DIR}/clox")

##########################
# main (repl) executable #
##########################

add_executable(clox-cli "${PROJECT_SOURCE_DIR}/clox-cli/main.c")
set_target_properties(clox-cli PROPERTIES
    C_STANDARD 17
    OUTPUT_NAME "clox"
)
target_include_directories(clox-cli
    PRIVATE
        # clox::INTERFACE_INCLUDE_DIRECTORIES
        "${PROJECT_SOURCE_DIR}"
)
target_link_libraries(clox-cli clox)

#########
# Tests #
#########

include(CTest)

add_executable(ast-printer.unit "${PROJECT_SOURCE_DIR}/clox/ast-printer.unit.c")
target_include_directories(ast-printer.unit PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(ast-printer.unit clox)
add_test(NAME ast-printer.unit COMMAND "${CMAKE_CURRENT_BINARY_DIR}/ast-printer.unit")

add_executable(ast-rpn-printer.unit "${PROJECT_SOURCE_DIR}/clox/ast-rpn-printer.unit.c")
target_include_directories(ast-rpn-printer.unit PRIVATE "${PROJECT_SOURCE_DIR}")
target_link_libraries(ast-rpn-printer.unit clox)
add_test(NAME ast-rpn-printer.unit COMMAND "${CMAKE_CURRENT_BINARY_DIR}/ast-rpn-printer.unit")
