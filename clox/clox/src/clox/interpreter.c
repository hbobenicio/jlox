#include "interpreter.h"

#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "stb_ds.h"
#include "ast/expr.h"
#include "ast/statement.h"
#include "ast/statement-visitor.h"
#include "ast/program.h"

// Interpreter's evaluation expression visitor implementation
static void eval_visit_expr_binary(struct clox_ast_expr* expr, void* userctx);
static void eval_visit_expr_grouping(struct clox_ast_expr* expr, void* userctx);
static void eval_visit_expr_literal(struct clox_ast_expr* expr, void* userctx);
static void eval_visit_expr_unary(struct clox_ast_expr* expr, void* userctx);

// Interpreter's exec statement visitor implementation
static int exec_visit_statement_expr(struct clox_ast_statement* stmt, void* userctx);
static int exec_visit_statement_print(struct clox_ast_statement* stmt, void* userctx);

// Auxiliary functions
static void interpreter_set_value(struct clox_interpreter* interpreter, struct clox_value val);
static bool is_truthy(struct clox_value value);
static bool is_equal(struct clox_value left, struct clox_value right);

// Expr visitor vtable
static const struct expr_visitor eval_expr_visitor = {
    .visit_binary = eval_visit_expr_binary,
    .visit_grouping = eval_visit_expr_grouping,
    .visit_literal = eval_visit_expr_literal,
    .visit_unary = eval_visit_expr_unary,
};

// Statement visitor vtable
static const struct clox_ast_statement_visitor eval_statement_visitor = {
    .visit_statement_expr = exec_visit_statement_expr,
    .visit_statement_print = exec_visit_statement_print,
};

void clox_interpreter_init(struct clox_interpreter* interpreter) {
    interpreter->value = clox_value_nil();
}

void clox_interpreter_free(struct clox_interpreter* interpreter) {
    // this will free the value if it's a str
    interpreter_set_value(interpreter, clox_value_nil());
}

struct clox_value clox_interpreter_eval(struct clox_interpreter* interpreter, struct clox_ast_expr* expr) {
    expr_accept(expr, &eval_expr_visitor, interpreter);
    return interpreter->value;
}

static void eval_visit_expr_binary(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_binary* expr_bin = &expr->value.binary;
    
    //FIXME these could also fail. Improve error handling. Avoid evaluating right hand side?
    //FIXME we now own this value. we must free it after use
    struct clox_value left = clox_interpreter_eval(interpreter, expr_bin->left);
    if (left.kind == CLOX_VALUE_KIND_STRING) {
        left.as.string = str_dup(left.as.string);
    }

    //FIXME we now own this value. we must free it after use
    struct clox_value right = clox_interpreter_eval(interpreter, expr_bin->right);
    if (right.kind == CLOX_VALUE_KIND_STRING) {
        right.as.string = str_dup(right.as.string);
    }
    
    switch(expr_bin->operator.kind) {
    case TOKEN_KIND_PLUS:
        if (left.kind == CLOX_VALUE_KIND_NUMBER && right.kind == CLOX_VALUE_KIND_NUMBER) {
            struct clox_value val = clox_value_number(left.as.number + right.as.number);
            interpreter_set_value(interpreter, val);
        } else if (left.kind == CLOX_VALUE_KIND_STRING && right.kind == CLOX_VALUE_KIND_STRING) {
            // this allocates a new string (we own this str)
            struct str concatenation = str_concat(left.as.string, right.as.string);

            // its str is a borrow from the above concatenation
            struct clox_value val = clox_value_string_str_borrow(concatenation);

            interpreter_set_value(interpreter, val);
        } else {
            //TODO improve error handling
            assert(false && "plus(+) operator is only valid for operands of the same type (number or string)");
        }
        break;

    case TOKEN_KIND_MINUS:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_number(left.as.number - right.as.number));
        break;

    case TOKEN_KIND_STAR:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_number(left.as.number * right.as.number));
        break;

    case TOKEN_KIND_SLASH:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_number(left.as.number / right.as.number));
        break;

    case TOKEN_KIND_GREATER:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_bool(left.as.number > right.as.number));
        break;

    case TOKEN_KIND_GREATER_EQUAL:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_bool(left.as.number >= right.as.number));
        break;

    case TOKEN_KIND_LESS:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_bool(left.as.number < right.as.number));
        break;

    case TOKEN_KIND_LESS_EQUAL:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_bool(left.as.number <= right.as.number));
        break;

    case TOKEN_KIND_BANG_EQUAL:
        interpreter_set_value(interpreter, clox_value_bool(!is_equal(left, right)));
        break;

    case TOKEN_KIND_EQUAL_EQUAL:
        interpreter_set_value(interpreter, clox_value_bool(is_equal(left, right)));
        break;

    default:
        assert(false && "unsupported binary operator. did you add a new binary operator recently?");
        break;
    }

    clox_value_free(&right);
    clox_value_free(&left);
}

static void eval_visit_expr_grouping(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;

    struct clox_value val = clox_interpreter_eval(interpreter, expr->value.grouping.expr);
    interpreter_set_value(interpreter, val);
}

static void eval_visit_expr_literal(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_literal* expr_lit = &expr->value.literal;

    switch (expr_lit->kind) {
    case CLOX_AST_EXPR_LITERAL_KIND_NUMBER:
        interpreter_set_value(interpreter, clox_value_number(expr_lit->value.number.val));
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_STRING:
        //NOTE this allocates a new string
        interpreter_set_value(interpreter, clox_value_string_str_dup(expr_lit->value.string.val));
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_BOOL:
        interpreter_set_value(interpreter, clox_value_bool(expr_lit->value.boolean.val));
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_NIL:
        interpreter_set_value(interpreter, clox_value_nil());
        break;

    default:
        assert(false && "unsupported expr_lit variant kind. did you add a new variant to it recently?");
        break;
    }
}

static void eval_visit_expr_unary(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_unary* expr_un = &expr->value.unary;

    // NOTE if we must do something after interpreter_set_value, we must str_dup this if it's a string
    struct clox_value right = clox_interpreter_eval(interpreter, expr_un->right);

    switch (expr_un->operator.kind) {
    case TOKEN_KIND_BANG:
        //TODO improve error handling
        assert(right.kind == CLOX_VALUE_KIND_BOOL);
        interpreter_set_value(interpreter, clox_value_bool(!is_truthy(right)));
        break;

    case TOKEN_KIND_MINUS:
        //TODO improve error handling
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter_set_value(interpreter, clox_value_number(-right.as.number));
        break;
    
    default:
        assert(false && "unsupported unary operator. did you added a new unary operator recently?");
        break;
    }
}

int clox_interpreter_exec_statement(struct clox_interpreter* interpreter, struct clox_ast_statement* stmt) {
    return clox_ast_statement_accept(stmt, &eval_statement_visitor, interpreter);
}

static int exec_visit_statement_expr(struct clox_ast_statement* stmt, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_statement_expr* expr_stmt = &stmt->as.expr_statement;

    // How to handle errors here?
    (void) clox_interpreter_eval(interpreter, expr_stmt->expr);
    return 0;
}

static int exec_visit_statement_print(struct clox_ast_statement* stmt, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_statement_print* print_stmt = &stmt->as.print_statement;

    // How to handle errors here?
    //NOTE val is a borrow which is owned by the interpreter
    struct clox_value val = clox_interpreter_eval(interpreter, print_stmt->expr);

    // Executing the print action
    clox_value_fprintln(stdout, val);

    return 0;
}

int clox_interpreter_exec_program(struct clox_interpreter* interpreter, struct clox_ast_program* prog) {
    for (long i = 0; i < arrlen(prog->statements); i++) {
        int rc = clox_interpreter_exec_statement(interpreter, prog->statements[i]);
        if (rc != 0) {
            //TODO add line number to the statement and print it here
            fprintf(stderr, "error: runtime error\n");
            return rc;
        }
    }
    return 0;
}

static void interpreter_set_value(struct clox_interpreter* interpreter, struct clox_value val) {
    if (interpreter->value.kind == CLOX_VALUE_KIND_STRING) {
        str_free(&interpreter->value.as.string);
    }
    interpreter->value = val;
}

static bool is_truthy(struct clox_value value) {
    switch (value.kind) {
    case CLOX_VALUE_KIND_NIL:
        return false;
        
    case CLOX_VALUE_KIND_BOOL:
        return value.as.boolean;

    default:
        return true;
    }
}

static bool is_equal(struct clox_value left, struct clox_value right) {
    if (left.kind != right.kind) {
        return false;
    }

    switch (left.kind) {
    case CLOX_VALUE_KIND_BOOL:
        return left.as.boolean == right.as.boolean;
        
    case CLOX_VALUE_KIND_NIL:
        return true;

    case CLOX_VALUE_KIND_NUMBER: {
        static const double epsilon = 0.00000001;
        return fabs(left.as.number - right.as.number) <= epsilon;
    }

    case CLOX_VALUE_KIND_STRING:
        return str_equals(left.as.string, right.as.string);

    default:
        assert(false && "unsupported value kind. did you add a new value kind recently?");
    }

    return false;
}
