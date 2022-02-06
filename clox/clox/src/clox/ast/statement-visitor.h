#ifndef CLOX_AST_STATEMENT_VISITOR_H
#define CLOX_AST_STATEMENT_VISITOR_H

struct clox_ast_statement;

struct clox_ast_statement_visitor {
    int (*visit_statement_expr)(struct clox_ast_statement* stmt, void* userctx);
    int (*visit_statement_print)(struct clox_ast_statement* stmt, void* userctx);
};

int clox_ast_statement_accept(struct clox_ast_statement* stmt, const struct clox_ast_statement_visitor* visitor, void* userctx);

#endif
