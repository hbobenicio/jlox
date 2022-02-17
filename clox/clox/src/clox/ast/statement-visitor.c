#include "statement-visitor.h"

#include "statement.h"

int clox_ast_statement_accept(struct clox_ast_statement* stmt, const struct clox_ast_statement_visitor* visitor, void* userctx) {
    switch (stmt->kind) {
    case CLOX_AST_STATEMENT_KIND_EXPR:
        if (visitor->visit_statement_expr) {
            return visitor->visit_statement_expr(stmt, userctx);
        }
        return 0;

    case CLOX_AST_STATEMENT_KIND_PRINT:
        if (visitor->visit_statement_print) {
            return visitor->visit_statement_print(stmt, userctx);
        }
        return 0;

    case CLOX_AST_STATEMENT_KIND_VAR:
        if (visitor->visit_statement_print) {
            return visitor->visit_statement_var(stmt, userctx);
        }
        return 0;
    }
    
    return 1;
}
