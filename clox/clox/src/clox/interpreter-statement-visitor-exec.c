#include "interpreter-statement-visitor-exec.h"

#include "ast/statement.h"
#include "ast/statement-visitor.h"
#include "interpreter.h"
#include "env.h"

static int exec_statement_expr(struct clox_ast_statement* stmt, void* userctx);
static int exec_statement_print(struct clox_ast_statement* stmt, void* userctx);
static int exec_statement_var(struct clox_ast_statement* stmt, void* userctx);

const struct clox_ast_statement_visitor* clox_interpreter_statement_visitor_exec(void) {
    static const struct clox_ast_statement_visitor vtable = {
        .visit_statement_expr = exec_statement_expr,
        .visit_statement_print = exec_statement_print,
        .visit_statement_var = exec_statement_var,
    };
    return &vtable;
}

static int exec_statement_expr(struct clox_ast_statement* stmt, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_statement_expr* expr_stmt = &stmt->as.expr_statement;

    // How to handle errors here?
    (void) clox_interpreter_eval(interpreter, expr_stmt->expr);
    
    return 0;
}

static int exec_statement_print(struct clox_ast_statement* stmt, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_statement_print* print_stmt = &stmt->as.print_statement;

    // How to handle errors here?
    //NOTE val is a borrow which is owned by the interpreter
    struct clox_value val = clox_interpreter_eval(interpreter, print_stmt->expr);

    // Executing the print action
    clox_value_fprintln(stdout, val);

    return 0;
}

static int exec_statement_var(struct clox_ast_statement* stmt, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_statement_var* var_stmt = &stmt->as.var_statement;

    struct clox_value var_value = clox_value_nil();
    if (var_stmt->initializer) {
        var_value = clox_value_dup(clox_interpreter_eval(interpreter, var_stmt->initializer));
    }

    struct strview var_name = var_stmt->name.lexeme;

    clox_env_define(&interpreter->env, var_name, var_value);

    return 0;
}
