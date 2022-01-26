#ifndef CLOX_TOKEN_H
#define CLOX_TOKEN_H

#include <stdio.h>

#include "strview.h"

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

struct token_value_number {
    double val;
};

struct token_value_string {
    struct strview val;
};

union token_value {
    struct token_value_number number;
    struct token_value_string string;
};

struct token {
    enum token_kind kind;
    size_t line;
    struct strview lexeme;
    union token_value value;
};

struct token token_new_eof(size_t line);
struct token token_new_number(size_t line, struct strview lexeme, union token_value value);

size_t token_len(const struct token* t);
const char* token_to_cstr(const struct token* token);
const char* token_kind_to_cstr(enum token_kind kind);
void token_fprint(FILE* file, const struct token* token);

#endif
