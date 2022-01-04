#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//POSIX
#include <sys/mman.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "commons.h"
#include "token.h"
#include "scanner.h"

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
        script_run(script_path, script_path_len);
        return EXIT_SUCCESS;
    }
    repl_start();
    return EXIT_SUCCESS;
}

int script_run(const char* script_path, size_t script_path_len) {
    (void) script_path_len;
    
    struct strview script_contents = {0};
    if (file_read_contents(script_path, &script_contents) != 0) {
        fprintf(stderr, "error: failed to run script %s\n", script_path);
        return 1;
    }

    struct scanner scanner;
    scanner_init(&scanner);

    struct token* tokens = scanner_scan_all(&scanner, script_contents);
    (void) tokens;

    munmap(script_contents.ptr, script_contents.len);
    return 0;
}

void repl_start(void) {
    struct scanner scanner;
    scanner_init(&scanner);

    for (;;) {
        printf("> ");

        char line[1024] = {0};
        const size_t line_cap = ARRAY_SIZE(line);
        fgets(line, line_cap, stdin);
        const size_t line_len = strnlen(line, line_cap);

        struct token* tokens = scanner_scan_all(&scanner, (struct strview){ .ptr = line, .len = line_len });
        (void) tokens;
    }
}
