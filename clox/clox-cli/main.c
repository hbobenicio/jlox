#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//POSIX
#include <sys/mman.h>

#define STB_DS_IMPLEMENTATION
#include <clox/stb_ds.h>

#include <clox/str.h>
#include <clox/commons.h>
#include <clox/token.h>
#include <clox/scanner.h>
#include <clox/ast/expr.h>
#include <clox/parser.h>
#include <clox/ast/ast-printer.h>
#include <clox/interpreter.h>
#include <clox/value.h>
#include <clox/ast/program.h>

#include "ansi.h"

#ifndef FILE_PATH_MAX_LEN
#define FILE_PATH_MAX_LEN 1024
#endif

int script_run(const char* script_path, size_t script_path_len);
void repl_start(void);

int main(int argc, char* argv[]) {
    if (argc >= 3) {
        fprintf(stderr, "usage: %s [script]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 2) {
        const char* script_path = argv[1];
        size_t script_path_len = strnlen(script_path, FILE_PATH_MAX_LEN);
        if (script_path_len >= FILE_PATH_MAX_LEN) {
            fprintf(stderr, "error: script file path overflow. the path limit is %u.\n", FILE_PATH_MAX_LEN);
            return EXIT_FAILURE;
        }
        if (script_run(script_path, script_path_len) != 0) {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    repl_start();
    return EXIT_SUCCESS;
}

int script_run(const char* script_path, size_t script_path_len) {
    (void) script_path_len;
    
    struct str script_contents = {0};
    if (file_read_contents(script_path, &script_contents) != 0) {
        fprintf(stderr, "error: failed to read file contents: %s\n", script_path);
        return 1;
    }

    struct scanner scanner = {0};
    scanner_scan_all(&scanner, strview_from_str(script_contents));

    struct parser parser;
    parser_init(&parser, scanner.tokens);

    // This AST depends on tokens from scanner. So scanner must outlive the AST.
    // The callee (us) owns this ast then we must free it
    struct clox_ast_program* prog = parser_parse(&parser);
    if (prog == NULL) {
        fprintf(stderr, "error: failed to parse file '%s'\n", script_path);
        scanner_free(&scanner);
        munmap(script_contents.ptr, script_contents.len);
        return 1;
    }

    struct clox_interpreter interpreter;
    clox_interpreter_init(&interpreter);

    // NOTE This value is borrowed from the interpreter internal state.
    // struct clox_value value = clox_interpreter_eval(&interpreter, expr);
    // clox_value_fprintln(stdout, value);
    if (clox_interpreter_exec_program(&interpreter, prog) != 0) {
        fprintf(stderr, "error: runtime error\n");
        clox_interpreter_free(&interpreter);
        clox_ast_program_free(prog);
        scanner_free(&scanner);
        return 1;
    }

    clox_interpreter_free(&interpreter);
    clox_ast_program_free(prog);
    scanner_free(&scanner);

    // NOTE: ATM tokens lexemes use strview, so they depend on the input file buffer.
    //       The expr ast use str, so they dont depend on the input file buffer, but they use copies of tokens...
    munmap(script_contents.ptr, script_contents.len);

    return 0;
}

void repl_start(void) {
    struct scanner scanner = {0};
    struct parser parser;

    char line[1024] = {0};
    const size_t line_cap = ARRAY_SIZE(line);

    puts(CLOX_ANSI_BHMAG "=== Clox REPL - Press CTRL+C or CTRL+D to exit ===" CLOX_ANSI_RESET);
    while (1) {
        memset(line, 0, line_cap);

        printf(CLOX_ANSI_HWHT "> " CLOX_ANSI_RESET);

        fgets(line, line_cap, stdin);
        if (line[0] == '\0') {
            break;
        }

        const size_t line_len = strnlen(line, line_cap);

        // Lexing. Tokens are stored inside the scanner
        scanner_scan_all_from_cstr(&scanner, line, line_len);

        parser_init(&parser, scanner.tokens);

        struct clox_ast_program* prog = parser_parse(&parser);
        if (prog == NULL) {
            fprintf(stderr, "error: parsing failed\n");
            //NOTE we don't need to call scanner_free here, because a new scan already free's it
            continue;
        }

        struct clox_interpreter interpreter;
        clox_interpreter_init(&interpreter);

        // NOTE This value is borrowed from the interpreter internal state.
        // struct clox_value value = clox_interpreter_eval(&interpreter, expr);
        // clox_value_fprintln(stdout, value);
        if (clox_interpreter_exec_program(&interpreter, prog) != 0) {
            fprintf(stderr, "error: runtime error\n");
            clox_interpreter_free(&interpreter);
            clox_ast_program_free(prog);
        }
        clox_interpreter_free(&interpreter);
        clox_ast_program_free(prog);
    }
    scanner_free(&scanner);
}
