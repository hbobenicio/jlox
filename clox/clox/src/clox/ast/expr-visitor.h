#ifndef CLOX_AST_EXPR_VISITOR_H
#define CLOX_AST_EXPR_VISITOR_H

struct clox_ast_expr;

//TODO improve this by replacing the void return type to int, for error handling
struct clox_ast_expr_visitor {
    void (*visit_binary)(struct clox_ast_expr* expr, void* userctx);
    void (*visit_grouping)(struct clox_ast_expr* expr, void* userctx);
    void (*visit_literal)(struct clox_ast_expr* expr, void* userctx);
    void (*visit_unary)(struct clox_ast_expr* expr, void* userctx);
    void (*visit_var)(struct clox_ast_expr* expr, void* userctx);
};

void clox_ast_expr_accept(struct clox_ast_expr* expr, const struct clox_ast_expr_visitor* visitor, void* userctx);

#endif
