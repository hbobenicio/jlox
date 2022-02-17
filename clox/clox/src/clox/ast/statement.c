#include "statement.h"

#include <stdlib.h>

#include <clox/commons.h>
#include "expr.h"

struct clox_ast_statement* clox_ast_statement_new_expr(struct clox_ast_expr* expr) {
    struct clox_ast_statement* stmt = malloc(sizeof(struct clox_ast_statement));
    CLOX_ERR_PANIC_OOM_IF_NULL(stmt);

    *stmt = (struct clox_ast_statement) {
        .kind = CLOX_AST_STATEMENT_KIND_EXPR,
        .as.expr_statement = (struct clox_ast_statement_expr) {
            .expr = expr,
        },
    };

    return stmt;
}

struct clox_ast_statement* clox_ast_statement_new_print(struct clox_ast_expr* expr) {
    struct clox_ast_statement* stmt = malloc(sizeof(struct clox_ast_statement));
    CLOX_ERR_PANIC_OOM_IF_NULL(stmt);

    *stmt = (struct clox_ast_statement) {
        .kind = CLOX_AST_STATEMENT_KIND_PRINT,
        .as.print_statement = (struct clox_ast_statement_print) {
            .expr = expr,
        },
    };

    return stmt;
}

struct clox_ast_statement* clox_ast_statement_new_var(struct token name, struct clox_ast_expr* initializer) {
    struct clox_ast_statement* stmt = malloc(sizeof(struct clox_ast_statement));
    CLOX_ERR_PANIC_OOM_IF_NULL(stmt);

    *stmt = (struct clox_ast_statement) {
        .kind = CLOX_AST_STATEMENT_KIND_VAR,
        .as.var_statement = (struct clox_ast_statement_var) {
            .name = name,
            .initializer = initializer,
        },
    };

    return stmt;
}

void clox_ast_statement_free(struct clox_ast_statement* stmt) {
    switch (stmt->kind) {
    case CLOX_AST_STATEMENT_KIND_EXPR:
        clox_ast_statement_expr_free(&stmt->as.expr_statement);
        break;

    case CLOX_AST_STATEMENT_KIND_PRINT:
        clox_ast_statement_print_free(&stmt->as.print_statement);

    case CLOX_AST_STATEMENT_KIND_VAR:
        clox_ast_statement_var_free(&stmt->as.var_statement);
    }

    free(stmt);
}

void clox_ast_statement_expr_free(struct clox_ast_statement_expr* expr_stmt) {
    clox_ast_expr_free(expr_stmt->expr);
    expr_stmt->expr = NULL;
}

void clox_ast_statement_print_free(struct clox_ast_statement_print* print_stmt) {
    clox_ast_expr_free(print_stmt->expr);
    print_stmt->expr = NULL;
}

void clox_ast_statement_var_free(struct clox_ast_statement_var* var_stmt) {
    if (var_stmt->initializer) {
        clox_ast_expr_free(var_stmt->initializer);
    }
}
