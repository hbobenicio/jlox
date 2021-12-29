#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// POSIX
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_PATH_MAX_LEN 1024

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

enum token_kind {
    TOKEN_KIND_EOF,
    // Single-character tokens.
    TOKEN_KIND_LEFT_PAREN,
    TOKEN_KIND_RIGHT_PAREN,
    TOKEN_KIND_LEFT_BRACE,
    TOKEN_KIND_RIGHT_BRACE,
    TOKEN_KIND_COMMA,
    TOKEN_KIND_DOT,
    TOKEN_KIND_MINUS,
    TOKEN_KIND_PLUS,
    TOKEN_KIND_SEMICOLON,
    TOKEN_KIND_SLASH,
    TOKEN_KIND_STAR,
    // One or two character tokens.
    TOKEN_KIND_BANG,
    TOKEN_KIND_BANG_EQUAL,
    TOKEN_KIND_EQUAL,
    TOKEN_KIND_EQUAL_EQUAL,
    TOKEN_KIND_GREATER,
    TOKEN_KIND_GREATER_EQUAL,
    TOKEN_KIND_LESS,
    TOKEN_KIND_LESS_EQUAL,
    // Literals.
    TOKEN_KIND_IDENTIFIER,
    TOKEN_KIND_STRING,
    TOKEN_KIND_NUMBER,
    // Keywords.,
    TOKEN_KIND_AND,
    TOKEN_KIND_CLASS,
    TOKEN_KIND_ELSE,
    TOKEN_KIND_FALSE,
    TOKEN_KIND_FUN,
    TOKEN_KIND_FOR,
    TOKEN_KIND_IF,
    TOKEN_KIND_NIL,
    TOKEN_KIND_OR,
    TOKEN_KIND_PRINT,
    TOKEN_KIND_RETURN,
    TOKEN_KIND_SUPER,
    TOKEN_KIND_THIS,
    TOKEN_KIND_TRUE,
    TOKEN_KIND_VAR,
    TOKEN_KIND_WHILE
};

struct strview {
    char* ptr;
    size_t len;
};

struct srcloc {
    size_t line;
    size_t column;
};

struct token_value_number {
    double val;
};

struct token_value_string {
    char* ptr;
    size_t len;
};

union token_value {
    struct token_value_number number;
    struct token_value_string string;
};

struct token {
    enum token_kind kind;
    struct srcloc loc;
    struct strview lexeme;
    union token_value value;
};

struct cursor {
    size_t start;
    size_t current;
};

struct scanner {
    struct cursor cursor;
    struct srcloc loc;
};

size_t token_len(const struct token* t) {
    return t->lexeme.len;
}

const char* token_kind_string(struct token* token) {
    switch (token->kind) {
    case TOKEN_KIND_EOF: return "TOKEN_KIND_EOF";
    case TOKEN_KIND_LEFT_PAREN: return "TOKEN_KIND_LEFT_PAREN";
    case TOKEN_KIND_RIGHT_PAREN: return "TOKEN_KIND_RIGHT_PAREN";
    case TOKEN_KIND_LEFT_BRACE: return "TOKEN_KIND_LEFT_BRACE";
    case TOKEN_KIND_RIGHT_BRACE: return "TOKEN_KIND_RIGHT_BRACE";
    case TOKEN_KIND_COMMA: return "TOKEN_KIND_COMMA";
    case TOKEN_KIND_DOT: return "TOKEN_KIND_DOT";
    case TOKEN_KIND_MINUS: return "TOKEN_KIND_MINUS";
    case TOKEN_KIND_PLUS: return "TOKEN_KIND_PLUS";
    case TOKEN_KIND_SEMICOLON: return "TOKEN_KIND_SEMICOLON";
    case TOKEN_KIND_SLASH: return "TOKEN_KIND_SLASH";
    case TOKEN_KIND_STAR: return "TOKEN_KIND_STAR";
    case TOKEN_KIND_BANG: return "TOKEN_KIND_BANG";
    case TOKEN_KIND_BANG_EQUAL: return "TOKEN_KIND_BANG_EQUAL";
    case TOKEN_KIND_EQUAL: return "TOKEN_KIND_EQUAL";
    case TOKEN_KIND_EQUAL_EQUAL: return "TOKEN_KIND_EQUAL_EQUAL";
    case TOKEN_KIND_GREATER: return "TOKEN_KIND_GREATER";
    case TOKEN_KIND_GREATER_EQUAL: return "TOKEN_KIND_GREATER_EQUAL";
    case TOKEN_KIND_LESS: return "TOKEN_KIND_LESS";
    case TOKEN_KIND_LESS_EQUAL: return "TOKEN_KIND_LESS_EQUAL";
    case TOKEN_KIND_IDENTIFIER: return "TOKEN_KIND_IDENTIFIER";
    case TOKEN_KIND_STRING: return "TOKEN_KIND_STRING";
    case TOKEN_KIND_NUMBER: return "TOKEN_KIND_NUMBER";
    case TOKEN_KIND_AND: return "TOKEN_KIND_AND";
    case TOKEN_KIND_CLASS: return "TOKEN_KIND_CLASS";
    case TOKEN_KIND_ELSE: return "TOKEN_KIND_ELSE";
    case TOKEN_KIND_FALSE: return "TOKEN_KIND_FALSE";
    case TOKEN_KIND_FUN: return "TOKEN_KIND_FUN";
    case TOKEN_KIND_FOR: return "TOKEN_KIND_FOR";
    case TOKEN_KIND_IF: return "TOKEN_KIND_IF";
    case TOKEN_KIND_NIL: return "TOKEN_KIND_NIL";
    case TOKEN_KIND_OR: return "TOKEN_KIND_OR";
    case TOKEN_KIND_PRINT: return "TOKEN_KIND_PRINT";
    case TOKEN_KIND_RETURN: return "TOKEN_KIND_RETURN";
    case TOKEN_KIND_SUPER: return "TOKEN_KIND_SUPER";
    case TOKEN_KIND_THIS: return "TOKEN_KIND_THIS";
    case TOKEN_KIND_TRUE: return "TOKEN_KIND_TRUE";
    case TOKEN_KIND_VAR: return "TOKEN_KIND_VAR";
    case TOKEN_KIND_WHILE: return "TOKEN_KIND_WHILE";
    default:
        assert(0 && "unhandled token kind. did you included a new one recently?");
        return NULL;
    }
}

void token_fprint(FILE* file, struct token* token) {
    switch(token->kind) {
    case TOKEN_KIND_NUMBER: {
        double val = token->value.number.val;
        fprintf(file, "%lf", val);
    } break;
    default:

    }
}

void scanner_init(struct scanner* s) {
    *s = (struct scanner) {
        .cursor = (struct cursor) {
            .start = 0,
            .current = 0,
        },
        .loc = (struct srcloc) {
            .line = 1,
            .column = 1,
        },
    };
}

struct token* scanner_scan_all(struct scanner* s, struct strview src) {
    (void) s;
    (void) src;
    return NULL;
}

int file_read_contents(const char* file_path, struct strview* contents) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "error: failed to open file %s for reading\n", file_path);
        return 1;
    }

    struct stat file_stats;
    if (fstat(fd, &file_stats) == -1) {
        fprintf(stderr, "error: failed to get file size\n");
        close(fd);
        return 1;
    }
    contents->len = file_stats.st_size;

    contents->ptr = mmap(NULL, contents->len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (contents->ptr == MAP_FAILED) {
        fprintf(stderr, "error: failed to read file contents\n");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
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

