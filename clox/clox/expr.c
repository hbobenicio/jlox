#include "expr.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

static void expr_accept_binary(struct expr_binary* expr, const struct expr_visitor* visitor, void* userctx);
static void expr_accept_grouping(struct expr_grouping* expr, const struct expr_visitor* visitor, void* userctx);
static void expr_accept_literal(struct expr_literal* expr, const struct expr_visitor* visitor, void* userctx);
static void expr_accept_unary(struct expr_unary* expr, const struct expr_visitor* visitor, void* userctx);

struct expr* expr_binary_new(struct expr* left, struct token operator, struct expr* right) {
    struct expr* expr = malloc(sizeof(struct expr));
    if (expr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return expr;
    }
    expr->kind = EXPR_KIND_BINARY;
    expr->value.binary = (struct expr_binary) {
        .left = left,
        .operator = operator,
        .right = right,
    };
    return expr;
}

struct expr* expr_unary_new(struct token operator, struct expr* right) {
    struct expr* expr = malloc(sizeof(struct expr));
    if (expr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return expr;
    }
    expr->kind = EXPR_KIND_UNARY;
    expr->value.unary = (struct expr_unary) {
        .operator = operator,
        .right = right,
    };
    return expr;
}

struct expr* expr_literal_bool_new(bool val) {
    struct expr* expr = malloc(sizeof(struct expr));
    if (expr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return expr;
    }
    expr->kind = EXPR_KIND_LITERAL;
    expr->value.literal = (struct expr_literal) {
        .kind = EXPR_LITERAL_KIND_BOOL,
        .value.boolean = (struct expr_literal_bool) {
            .val = val,
        },
    };
    return expr;
}

struct expr* expr_literal_nil_new(void) {
    struct expr* expr = malloc(sizeof(struct expr));
    if (expr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return expr;
    }
    *expr = (struct expr) {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = (struct expr_literal) {
            .kind = EXPR_LITERAL_KIND_NIL,
            .value = {0},
        },
    };
    return expr;
}

struct expr* expr_literal_string_new(struct strview sv) {
    struct expr* expr = malloc(sizeof(struct expr));
    if (expr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return expr;
    }
    size_t cstr_len = sv.len;
    size_t cstr_cap = sv.len + 1;
    char* cstr = calloc(cstr_cap, sizeof(char));
    if (cstr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        free(expr);
        return NULL;
    }
    *expr = (struct expr) {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = (struct expr_literal) {
            .kind = EXPR_LITERAL_KIND_STRING,
            .value.string = (struct expr_literal_string) {
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

struct expr* expr_literal_number_new(double val) {
    struct expr* expr = malloc(sizeof(struct expr));
    if (expr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return expr;
    }
    *expr = (struct expr) {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = (struct expr_literal) {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = (struct expr_literal_number) {
                .val = val,
            },
        },
    };
    return expr;
}

struct expr* expr_grouping_new(struct expr* expr) {
    struct expr* e = malloc(sizeof(struct expr));
    if (e == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return e;
    }
    *e = (struct expr) {
        .kind = EXPR_KIND_GROUPING,
        .value.grouping = (struct expr_grouping) {
            .expr = expr,
        },
    };
    return e;
}

struct expr expr_literal_number_create(double val) {
    return (struct expr) {
        .kind = EXPR_KIND_LITERAL,
        .value.literal = (struct expr_literal) {
            .kind = EXPR_LITERAL_KIND_NUMBER,
            .value.number = (struct expr_literal_number) {
                .val = val,
            },
        },
    };
}

struct expr expr_grouping_create(struct expr* expr) {
    return (struct expr) {
        .kind = EXPR_KIND_GROUPING,
        .value.grouping = (struct expr_grouping) {
            .expr = expr,
        },
    };
}

void expr_accept(struct expr* expr, const struct expr_visitor* visitor, void* userctx) {
    switch (expr->kind) {
    case EXPR_KIND_BINARY:
        expr_accept_binary(&expr->value.binary, visitor, userctx);
        break;

    case EXPR_KIND_GROUPING:
        expr_accept_grouping(&expr->value.grouping, visitor, userctx);
        break;

    case EXPR_KIND_LITERAL:
        expr_accept_literal(&expr->value.literal, visitor, userctx);
        break;

    case EXPR_KIND_UNARY:
        expr_accept_unary(&expr->value.unary, visitor, userctx);
        break;
    
    default:
        assert(false && "unsupported expr_kind variant. did you add a new variant for it recently?");
        break;
    }
}

static void expr_accept_binary(struct expr_binary* expr, const struct expr_visitor* visitor, void* userctx) {
    visitor->visit_binary(expr, userctx);
}

static void expr_accept_grouping(struct expr_grouping* expr, const struct expr_visitor* visitor, void* userctx) {
    visitor->visit_grouping(expr, userctx);
}

static void expr_accept_literal(struct expr_literal* expr, const struct expr_visitor* visitor, void* userctx) {
    visitor->visit_literal(expr, userctx);
}

static void expr_accept_unary(struct expr_unary* expr, const struct expr_visitor* visitor, void* userctx) {
    visitor->visit_unary(expr, userctx);
}
