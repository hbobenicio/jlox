#include "token.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct token token_new_eof(size_t line) {
    return (struct token) {
        .kind = TOKEN_KIND_EOF,
        .lexeme = strview_empty(),
        .line = line,
        .value = {0},
    };
}

struct token token_new_number(size_t line, struct strview lexeme, union token_value value) {
    return (struct token) {
        .kind = TOKEN_KIND_NUMBER,
        .lexeme = lexeme,
        .line = line,
        .value = value,
    };
}

size_t token_len(const struct token* t) {
    return t->lexeme.len;
}

const char* token_kind_to_cstr(const struct token* token) {
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

void token_fprint(FILE* file, const struct token* token) {
    switch(token->kind) {
    case TOKEN_KIND_NUMBER: {
        double val = token->value.number.val;
        fprintf(file, "<%s(%lf)>", token_kind_to_cstr(token), val);
    } break;
    case TOKEN_KIND_STRING: {
        struct strview sv = token->value.string.val;
        char* cstr = calloc(sv.len + 1, sizeof(char));
        memcpy(cstr, sv.ptr, sv.len);
        fprintf(file, "<%s(%s)>", token_kind_to_cstr(token), cstr);
        free(cstr);
    } break;
    default:
        fprintf(file, "%s", token_kind_to_cstr(token));
    }
}
