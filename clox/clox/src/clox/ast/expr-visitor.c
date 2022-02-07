#include "expr-visitor.h"

#include <assert.h>

#include "expr.h"

void clox_ast_expr_accept(struct clox_ast_expr* expr, const struct clox_ast_expr_visitor* visitor, void* userctx) {
    // Don't call other accepts inside an accept function. just visitors.
    // let visitors call the accept back to this module, which improves traversal flexibility
    switch (expr->kind) {
    case CLOX_AST_EXPR_KIND_BINARY:
        visitor->visit_binary(expr, userctx);
        break;

    case CLOX_AST_EXPR_KIND_GROUPING:
        visitor->visit_grouping(expr, userctx);
        break;

    case CLOX_AST_EXPR_KIND_LITERAL:
        visitor->visit_literal(expr, userctx);
        break;

    case CLOX_AST_EXPR_KIND_UNARY:
        visitor->visit_unary(expr, userctx);
        break;
    
    default:
        assert(false && "unsupported clox_ast_expr_kind variant. did you add a new variant for it recently?");
        break;
    }
}
