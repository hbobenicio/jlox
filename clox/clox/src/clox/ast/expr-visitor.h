#ifndef CLOX_AST_EXPR_VISITOR_H
#define CLOX_AST_EXPR_VISITOR_H

struct clox_ast_expr;

struct clox_ast_expr_visitor {
    int (*visit_binary)(struct clox_ast_expr* expr, void* userctx);
    int (*visit_grouping)(struct clox_ast_expr* expr, void* userctx);
    int (*visit_literal)(struct clox_ast_expr* expr, void* userctx);
    int (*visit_unary)(struct clox_ast_expr* expr, void* userctx);
    int (*visit_var)(struct clox_ast_expr* expr, void* userctx);
    int (*visit_assign)(struct clox_ast_expr* expr, void* userctx);
};

int clox_ast_expr_accept(struct clox_ast_expr* expr, const struct clox_ast_expr_visitor* visitor, void* userctx);

#endif
