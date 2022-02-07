#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include <clox/stb_ds.h>

#include <clox/token.h>
#include "expr.h"
#include "ast-rpn-printer.h"

int main() {
    // TODO can we improve this with an AST allocator?
    // TODO we can create some constructors to ease this construction, right? :)
    struct clox_ast_expr one = clox_ast_expr_literal_number_create(1.0);
    struct clox_ast_expr two = clox_ast_expr_literal_number_create(2.0);
    struct clox_ast_expr three = clox_ast_expr_literal_number_create(3.0);
    struct clox_ast_expr four = clox_ast_expr_literal_number_create(4.0);
    struct clox_ast_expr left_expr = {
        .kind = CLOX_AST_EXPR_KIND_BINARY,
        .value.binary = {
            .left = &one,
            .operator = {
                .kind = TOKEN_KIND_PLUS,
                .line = 1,
                .lexeme = {
                    .ptr = "+",
                    .len = 1,
                },
                .value = {0},
            },
            .right = &two,
        },
    };
    struct clox_ast_expr right_expr = {
        .kind = CLOX_AST_EXPR_KIND_BINARY,
        .value.binary = {
            .left = &four,
            .operator = {
                .kind = TOKEN_KIND_MINUS,
                .line = 1,
                .lexeme = {
                    .ptr = "-",
                    .len = 1,
                },
                .value = {0},
            },
            .right = &three,
        },
    };
    struct clox_ast_expr group_left = clox_ast_expr_grouping_create(&left_expr);
    struct clox_ast_expr group_right = clox_ast_expr_grouping_create(&right_expr);
    struct clox_ast_expr expr = {
        .kind = CLOX_AST_EXPR_KIND_BINARY,
        .value.binary = {
            .left = &group_left,
            .operator = {
                .kind = TOKEN_KIND_STAR,
                .line = 1,
                .lexeme = {
                    .ptr = "*",
                    .len = 1,
                },
                .value = {0},
            },
            .right = &group_right,
        },
    };

    ast_rpn_printer_println(&expr);
}
