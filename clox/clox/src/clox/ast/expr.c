#include "expr.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <clox/commons.h>
#include "expr-visitor.h"
#include "expr-visitor-free.h"

struct clox_ast_expr* clox_ast_expr_binary_new(struct clox_ast_expr* left, struct token operator, struct clox_ast_expr* right) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    expr->kind = CLOX_AST_EXPR_KIND_BINARY;
    expr->value.binary = (struct clox_ast_expr_binary) {
        .left = left,
        .operator = operator,
        .right = right,
    };
    return expr;
}

struct clox_ast_expr* clox_ast_expr_unary_new(struct token operator, struct clox_ast_expr* right) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    expr->kind = CLOX_AST_EXPR_KIND_UNARY;
    expr->value.unary = (struct clox_ast_expr_unary) {
        .operator = operator,
        .right = right,
    };
    return expr;
}

struct clox_ast_expr* clox_ast_expr_var_new(struct token name) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    *expr = (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_VAR,
        .value.var = (struct clox_ast_expr_var) {
            .name = name,
        },
    };
    return expr;
}

struct clox_ast_expr* clox_ast_expr_assign_new(struct token name, struct clox_ast_expr* value) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    *expr = (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_ASSIGN,
        .value.assign = (struct clox_ast_expr_assign) {
            .name = name,
            .value = value,
        },
    };
    return expr;
}


struct clox_ast_expr* clox_ast_expr_literal_bool_new(bool val) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    expr->kind = CLOX_AST_EXPR_KIND_LITERAL;
    expr->value.literal = (struct clox_ast_expr_literal) {
        .kind = CLOX_AST_EXPR_LITERAL_KIND_BOOL,
        .value.boolean = (struct clox_ast_expr_literal_bool) {
            .val = val,
        },
    };
    return expr;
}

struct clox_ast_expr* clox_ast_expr_literal_nil_new(void) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    *expr = (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_LITERAL,
        .value.literal = (struct clox_ast_expr_literal) {
            .kind = CLOX_AST_EXPR_LITERAL_KIND_NIL,
            .value = {0},
        },
    };
    return expr;
}

struct clox_ast_expr* clox_ast_expr_literal_string_new(struct strview sv) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    size_t cstr_len = sv.len;
    size_t cstr_cap = sv.len + 1;

    char* cstr = calloc(cstr_cap, sizeof(char));
    CLOX_ERR_PANIC_OOM_IF_NULL(cstr);

    memcpy(cstr, sv.ptr, sv.len);
    
    *expr = (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_LITERAL,
        .value.literal = (struct clox_ast_expr_literal) {
            .kind = CLOX_AST_EXPR_LITERAL_KIND_STRING,
            .value.string = (struct clox_ast_expr_literal_string) {
                .val = (struct str) {
                    .cap = cstr_cap,
                    .len = cstr_len,
                    .ptr = cstr,
                },
            },
        },
    };
    return expr;
}

struct clox_ast_expr* clox_ast_expr_literal_number_new(double val) {
    struct clox_ast_expr* expr = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(expr);

    *expr = (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_LITERAL,
        .value.literal = (struct clox_ast_expr_literal) {
            .kind = CLOX_AST_EXPR_LITERAL_KIND_NUMBER,
            .value.number = (struct clox_ast_expr_literal_number) {
                .val = val,
            },
        },
    };
    return expr;
}

struct clox_ast_expr* clox_ast_expr_grouping_new(struct clox_ast_expr* expr) {
    struct clox_ast_expr* e = malloc(sizeof(struct clox_ast_expr));
    CLOX_ERR_PANIC_OOM_IF_NULL(e);

    *e = (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_GROUPING,
        .value.grouping = (struct clox_ast_expr_grouping) {
            .expr = expr,
        },
    };
    return e;
}

struct clox_ast_expr clox_ast_expr_literal_number_create(double val) {
    return (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_LITERAL,
        .value.literal = (struct clox_ast_expr_literal) {
            .kind = CLOX_AST_EXPR_LITERAL_KIND_NUMBER,
            .value.number = (struct clox_ast_expr_literal_number) {
                .val = val,
            },
        },
    };
}

struct clox_ast_expr clox_ast_expr_grouping_create(struct clox_ast_expr* expr) {
    return (struct clox_ast_expr) {
        .kind = CLOX_AST_EXPR_KIND_GROUPING,
        .value.grouping = (struct clox_ast_expr_grouping) {
            .expr = expr,
        },
    };
}

// This is implemented basically as a post-order visitor.
// Maybe we eventually end up with multiple post-order implementations.
// Could we abstract those implementations as one and offer an extension point useful enough? (func ptr?)
void clox_ast_expr_free(struct clox_ast_expr* expr) {
    clox_ast_expr_accept(expr, clox_ast_expr_visitor_free(), NULL);
}
