#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include <clox/stb_ds.h>

#include <clox/token.h>
#include "expr.h"
#include "ast-printer.h"

int main() {
    // TODO can we improve this with an AST allocator?
    // TODO we can create some constructors to ease this construction, right? :)
    struct clox_ast_expr lit_45_67 = clox_ast_expr_literal_number_create(45.67);
    struct clox_ast_expr lit_123 = clox_ast_expr_literal_number_create(123.0);
    struct clox_ast_expr expr_left = {
        .kind = CLOX_AST_EXPR_KIND_UNARY,
        .value.unary = (struct clox_ast_expr_unary) {
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
    struct clox_ast_expr expr_right = clox_ast_expr_grouping_create(&lit_45_67);
    struct clox_ast_expr expr = {
        .kind = CLOX_AST_EXPR_KIND_BINARY,
        .value.binary = (struct clox_ast_expr_binary) {
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

    ast_printer_println(&expr);
}
