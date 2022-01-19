#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "clox/stb_ds.h"

#include "clox/token.h"
#include "clox/expr.h"
#include "clox/ast-rpn-printer.h"

int main() {
    // TODO can we improve this with an AST allocator?
    // TODO we can create some constructors to ease this construction, right? :)
    struct expr one = {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = {
                .val = 1.0,
            },
        },
    };
    struct expr two = {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = {
                .val = 2.0,
            },
        },
    };
    struct expr three = {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = {
                .val = 3.0,
            },
        },
    };
    struct expr four = {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = {
                .val = 4.0,
            },
        },
    };
    struct expr left_expr = {
        .kind = EXPR_KIND_BINARY,
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
    struct expr right_expr = {
        .kind = EXPR_KIND_BINARY,
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
    struct expr group_left = {
        .kind = EXPR_KIND_GROUPING,
        .value.grouping = {
            .expr = &left_expr,
        },
    };
    struct expr group_right = {
        .kind = EXPR_KIND_GROUPING,
        .value.grouping = {
            .expr = &right_expr,
        },
    };
    struct expr expr = {
        .kind = EXPR_KIND_BINARY,
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
