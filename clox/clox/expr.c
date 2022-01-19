#include "expr.h"

#include <stdbool.h>
#include <assert.h>

static void expr_accept_binary(struct expr_binary* expr, const struct expr_visitor* visitor, void* userctx);
static void expr_accept_grouping(struct expr_grouping* expr, const struct expr_visitor* visitor, void* userctx);
static void expr_accept_literal(struct expr_literal* expr, const struct expr_visitor* visitor, void* userctx);
static void expr_accept_unary(struct expr_unary* expr, const struct expr_visitor* visitor, void* userctx);

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
