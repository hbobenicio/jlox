#include "expr.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// Visitors for freeing expressions
// (using this recursively slow approach until we get our Ast/Expr Allocator)
static void expr_free_visit_binary(struct expr* expr, void* userctx);
static void expr_free_visit_grouping(struct expr* expr, void* userctx);
static void expr_free_visit_literal(struct expr* expr, void* userctx);
static void expr_free_visit_unary(struct expr* expr, void* userctx);

static const struct expr_visitor expr_free_visitor = {
    .visit_binary = expr_free_visit_binary,
    .visit_grouping = expr_free_visit_grouping,
    .visit_literal = expr_free_visit_literal,
    .visit_unary = expr_free_visit_unary,
};

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

    memcpy(cstr, sv.ptr, sv.len);
    
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
    // Don't call other accepts inside an accept function. just visitors.
    // let visitors call the accept back to this module, which improves traversal flexibility
    switch (expr->kind) {
    case EXPR_KIND_BINARY:
        visitor->visit_binary(expr, userctx);
        break;

    case EXPR_KIND_GROUPING:
        visitor->visit_grouping(expr, userctx);
        break;

    case EXPR_KIND_LITERAL:
        visitor->visit_literal(expr, userctx);
        break;

    case EXPR_KIND_UNARY:
        visitor->visit_unary(expr, userctx);
        break;
    
    default:
        assert(false && "unsupported expr_kind variant. did you add a new variant for it recently?");
        break;
    }
}

// This is implemented basically as a post-order visitor.
// Maybe we eventually end up with multiple post-order implementations.
// Could we abstract those implementations as one and offer an extension point useful enough? (func ptr?)
void expr_free(struct expr* expr) {
    expr_accept(expr, &expr_free_visitor, NULL);
}

// Memory free must be a Postorder traversal
static void expr_free_visit_binary(struct expr* expr, void* userctx) {
    (void) userctx;
    expr_free(expr->value.binary.left);
    expr_free(expr->value.binary.right);
    free(expr);
}

static void expr_free_visit_grouping(struct expr* expr, void* userctx) {
    (void) userctx;
    expr_free(expr->value.grouping.expr);
    free(expr);
}

static void expr_free_visit_literal(struct expr* expr, void* userctx) {
    (void) userctx;
    if (expr->value.literal.kind == EXPR_LITERAL_KIND_STRING) {
        str_free(&expr->value.literal.value.string.val);
    }
    free(expr);
}

static void expr_free_visit_unary(struct expr* expr, void* userctx) {
    (void) userctx;
    expr_free(expr->value.unary.right);
    free(expr);
}
