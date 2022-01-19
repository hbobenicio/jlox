#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//POSIX
#include <sys/mman.h>

#define STB_DS_IMPLEMENTATION
#include "clox/stb_ds.h"

#include "ansi.h"
#include "clox/str.h"
#include "clox/commons.h"
#include "clox/token.h"
#include "clox/scanner.h"

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
        fprintf(stderr, "error: failed to run script %s\n", script_path);
        return 1;
    }

    struct scanner scanner = {0};
    scanner_scan_all(&scanner, strview_from_str(script_contents));

    munmap(script_contents.ptr, script_contents.len);
    return 0;
}

void repl_start(void) {
    struct scanner scanner = {0};

    char line[1024] = {0};
    const size_t line_cap = ARRAY_SIZE(line);

    puts(CLOX_ANSI_BHMAG "=== Clox REPL - Press CTRL+C to exit ===" CLOX_ANSI_RESET);
    while (1) {
        memset(line, 0, line_cap);

        printf(CLOX_ANSI_HWHT "> " CLOX_ANSI_RESET);

        fgets(line, line_cap, stdin);
        const size_t line_len = strnlen(line, line_cap);

        scanner_scan_all_from_cstr(&scanner, line, line_len);
        for (long i = 0; i < arrlen(scanner.tokens); i++) {
            token_fprint(stdout, &scanner.tokens[i]);
            putchar(' ');
        }
        puts("");
    }
}
