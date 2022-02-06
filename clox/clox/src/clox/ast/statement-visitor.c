#include "statement-visitor.h"

#include "statement.h"

int clox_ast_statement_accept(struct clox_ast_statement* stmt, const struct clox_ast_statement_visitor* visitor, void* userctx) {
    switch (stmt->kind) {
    case CLOX_AST_STATEMENT_KIND_EXPR:
        return visitor->visit_statement_expr(stmt, userctx);

    case CLOX_AST_STATEMENT_KIND_PRINT:
        return visitor->visit_statement_print(stmt, userctx);
    }
    return 1;
}
