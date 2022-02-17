#ifndef CLOX_AST_STATEMENT_H
#define CLOX_AST_STATEMENT_H

#include <clox/token.h>

struct clox_ast_expr;

enum clox_ast_statement_kind {
    /**
     * @brief Expression Statement
     */
    CLOX_AST_STATEMENT_KIND_EXPR,
    /**
     * @brief Print Statement
     */
    CLOX_AST_STATEMENT_KIND_PRINT,
    /**
     * @brief Variable Declaration Statement
     */
    CLOX_AST_STATEMENT_KIND_VAR,
};

struct clox_ast_statement_expr {
    struct clox_ast_expr* expr;
};

struct clox_ast_statement_print {
    struct clox_ast_expr* expr;
};

struct clox_ast_statement_var {
    struct token name;
    struct clox_ast_expr* initializer;
};

struct clox_ast_statement {
    enum clox_ast_statement_kind kind;
    union {
        struct clox_ast_statement_expr expr_statement;
        struct clox_ast_statement_print print_statement;
        struct clox_ast_statement_var var_statement;
    } as;
};

struct clox_ast_statement* clox_ast_statement_new_expr(struct clox_ast_expr* expr);
struct clox_ast_statement* clox_ast_statement_new_print(struct clox_ast_expr* expr);
struct clox_ast_statement* clox_ast_statement_new_var(struct token name, struct clox_ast_expr* initializer);

void clox_ast_statement_free(struct clox_ast_statement* stmt);
void clox_ast_statement_expr_free(struct clox_ast_statement_expr* expr_stmt);
void clox_ast_statement_print_free(struct clox_ast_statement_print* print_stmt);
void clox_ast_statement_var_free(struct clox_ast_statement_var* var_stmt);

#endif
