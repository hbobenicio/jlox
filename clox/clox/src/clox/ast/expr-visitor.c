#include "expr-visitor.h"

#include <assert.h>

#include "expr.h"

int clox_ast_expr_accept(struct clox_ast_expr* expr, const struct clox_ast_expr_visitor* visitor, void* userctx) {
    // Don't call other accepts inside an accept function. just visitors.
    // let visitors call the accept back to this module, which improves traversal flexibility
    switch (expr->kind) {
    case CLOX_AST_EXPR_KIND_BINARY:
        if (visitor->visit_binary) {
            return visitor->visit_binary(expr, userctx);
        }
        return 0;

    case CLOX_AST_EXPR_KIND_GROUPING:
        if (visitor->visit_grouping) {
            return visitor->visit_grouping(expr, userctx);
        }
        return 0;

    case CLOX_AST_EXPR_KIND_LITERAL:
        if (visitor->visit_literal) {
            return visitor->visit_literal(expr, userctx);
        }
        return 0;

    case CLOX_AST_EXPR_KIND_UNARY:
        if (visitor->visit_unary) {
            return visitor->visit_unary(expr, userctx);
        }
        return 0;

    case CLOX_AST_EXPR_KIND_VAR:
        if (visitor->visit_var) {
            return visitor->visit_var(expr, userctx);
        }
        return 0;

    case CLOX_AST_EXPR_KIND_ASSIGN:
        if (visitor->visit_assign) {
            return visitor->visit_assign(expr, userctx);
        }
        return 0;
    }

    assert(false && "unsupported clox_ast_expr_kind");
    return 1;
}
