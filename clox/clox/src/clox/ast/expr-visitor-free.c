#include "expr-visitor-free.h"

#include <stdlib.h>

#include "expr.h"
#include "expr-visitor.h"

static int visit_binary(struct clox_ast_expr* expr, void* userctx);
static int visit_grouping(struct clox_ast_expr* expr, void* userctx);
static int visit_literal(struct clox_ast_expr* expr, void* userctx);
static int visit_unary(struct clox_ast_expr* expr, void* userctx);
static int visit_var(struct clox_ast_expr* expr, void* userctx);

const struct clox_ast_expr_visitor* clox_ast_expr_visitor_free(void) {
    static const struct clox_ast_expr_visitor vtable = {
        .visit_binary = visit_binary,
        .visit_grouping = visit_grouping,
        .visit_literal = visit_literal,
        .visit_unary = visit_unary,
        .visit_var = visit_var,
    };
    return &vtable;
}

// Memory free must be a Postorder traversal
static int visit_binary(struct clox_ast_expr* expr, void* userctx) {
    (void) userctx;
    
    clox_ast_expr_free(expr->value.binary.left);
    clox_ast_expr_free(expr->value.binary.right);
    free(expr);

    return 0;
}

static int visit_grouping(struct clox_ast_expr* expr, void* userctx) {
    (void) userctx;
    
    clox_ast_expr_free(expr->value.grouping.expr);
    free(expr);

    return 0;
}

static int visit_literal(struct clox_ast_expr* expr, void* userctx) {
    (void) userctx;
    
    if (expr->value.literal.kind == CLOX_AST_EXPR_LITERAL_KIND_STRING) {
        str_free(&expr->value.literal.value.string.val);
    }
    free(expr);

    return 0;
}

static int visit_unary(struct clox_ast_expr* expr, void* userctx) {
    (void) userctx;
    
    clox_ast_expr_free(expr->value.unary.right);
    free(expr);

    return 0;
}

static int visit_var(struct clox_ast_expr* expr, void* userctx) {
    (void) userctx;
    
    free(expr);

    return 0;
}
