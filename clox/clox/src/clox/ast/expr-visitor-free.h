#ifndef CLOX_AST_EXPR_VISITOR_FREE_H
#define CLOX_AST_EXPR_VISITOR_FREE_H

struct clox_ast_expr;
struct clox_ast_expr_visitor;

// Visitors for freeing expressions
// (using this recursively slow approach until we get our Ast/Expr Allocator)

const struct clox_ast_expr_visitor* clox_ast_expr_visitor_free(void);

#endif
