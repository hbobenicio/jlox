#include "interpreter.h"

#include "stb_ds.h"
#include "ast/expr.h"
#include "ast/expr-visitor.h"
#include "ast/statement.h"
#include "ast/statement-visitor.h"
#include "ast/program.h"
#include "interpreter-expr-visitor-eval.h"
#include "interpreter-statement-visitor-exec.h"

void clox_interpreter_init(struct clox_interpreter* interpreter) {
    interpreter->value = clox_value_nil();
    clox_env_init(&interpreter->env);
}

void clox_interpreter_free(struct clox_interpreter* interpreter) {
    clox_env_free(&interpreter->env);
    // this will free the value if it's a str
    clox_interpreter_set_value(interpreter, clox_value_nil());
}

struct clox_value clox_interpreter_eval(struct clox_interpreter* interpreter, struct clox_ast_expr* expr) {
    clox_ast_expr_accept(expr, clox_interpreter_expr_visitor_eval(), interpreter);
    return interpreter->value;
}

int clox_interpreter_exec_statement(struct clox_interpreter* interpreter, struct clox_ast_statement* stmt) {
    return clox_ast_statement_accept(stmt, clox_interpreter_statement_visitor_exec(), interpreter);
}

int clox_interpreter_exec_program(struct clox_interpreter* interpreter, struct clox_ast_program* prog) {
    for (long i = 0; i < arrlen(prog->statements); i++) {
        int rc = clox_interpreter_exec_statement(interpreter, prog->statements[i]);
        if (rc != 0) {
            //TODO add line number to the statement and print it here
            fprintf(stderr, "error: %s:%d: runtime error\n", __FILE__, __LINE__);
            return rc;
        }
    }
    return 0;
}

void clox_interpreter_set_value(struct clox_interpreter* interpreter, struct clox_value val) {
    if (interpreter->value.kind == CLOX_VALUE_KIND_STRING) {
        str_free(&interpreter->value.as.string);
    }
    interpreter->value = val;
}
