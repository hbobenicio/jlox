#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "clox/stb_ds.h"

#include "clox/token.h"
#include "clox/expr.h"
#include "clox/ast-printer.h"

int main() {
    // TODO can we improve this with an AST allocator?
    // TODO we can create some constructors to ease this construction, right? :)
    struct expr lit_45_67 = {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = (struct expr_literal) {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = (struct expr_literal_number) {
                .val = 45.67,
            },
        },
    };
    struct expr lit_123 = {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = (struct expr_literal) {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = (struct expr_literal_number) {
                .val = 123.0,
            },
        },
    };
    struct expr expr_left = {
        .kind = EXPR_KIND_UNARY,
        .value.unary = (struct expr_unary) {
            .operator = (struct token) {
                .kind = TOKEN_KIND_MINUS,
                .line = 1,
                .lexeme = (struct strview) {
                    .ptr = "-",
                    .len = 1,
                },
                .value = {0},
            },
            .right = &lit_123,
        },
    };
    struct expr expr_right = {
        .kind = EXPR_KIND_GROUPING,
        .value.grouping = (struct expr_grouping) {
            .expr = &lit_45_67,
        },
    };
    struct expr expr = {
        .kind = EXPR_KIND_BINARY,
        .value.binary = (struct expr_binary) {
            .left = &expr_left,
            .operator = (struct token) {
                .kind = TOKEN_KIND_STAR,
                .line = 1,
                .lexeme = (struct strview) {
                    .ptr = "*",
                    .len = 1,
                },
                .value = {0},
            },
            .right = &expr_right,
        },
    };

    struct ast_printer ast_printer = {
        .file = stdout,
    };

    ast_printer_print(&ast_printer, &expr);
    puts("");
}
