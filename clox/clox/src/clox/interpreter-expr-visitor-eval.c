#include "interpreter-expr-visitor-eval.h"

#include <assert.h>

#include "ast/expr.h"
#include "ast/expr-visitor.h"
#include "value.h"
#include "interpreter.h"

static void eval_visit_expr_binary(struct clox_ast_expr* expr, void* userctx);
static void eval_visit_expr_grouping(struct clox_ast_expr* expr, void* userctx);
static void eval_visit_expr_literal(struct clox_ast_expr* expr, void* userctx);
static void eval_visit_expr_unary(struct clox_ast_expr* expr, void* userctx);
static void eval_visit_expr_var(struct clox_ast_expr* expr, void* userctx);

const struct clox_ast_expr_visitor* clox_interpreter_expr_visitor_eval(void) {
    static const struct clox_ast_expr_visitor vtable = {
        .visit_binary = eval_visit_expr_binary,
        .visit_grouping = eval_visit_expr_grouping,
        .visit_literal = eval_visit_expr_literal,
        .visit_unary = eval_visit_expr_unary,
        .visit_var = eval_visit_expr_var,
    };
    return &vtable;
}

static void eval_visit_expr_binary(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_binary* expr_bin = &expr->value.binary;
    
    //FIXME these could also fail. Improve error handling. Avoid evaluating right hand side?
    //NOTE this is a borrowed value
    struct clox_value left = clox_interpreter_eval(interpreter, expr_bin->left);
    if (left.kind == CLOX_VALUE_KIND_STRING) {
        left.as.string = str_dup(left.as.string);
    }

    //NOTE this is a borrowed value
    struct clox_value right = clox_interpreter_eval(interpreter, expr_bin->right);
    if (right.kind == CLOX_VALUE_KIND_STRING) {
        right.as.string = str_dup(right.as.string);
    }
    
    switch(expr_bin->operator.kind) {
    case TOKEN_KIND_PLUS:
        if (left.kind == CLOX_VALUE_KIND_NUMBER && right.kind == CLOX_VALUE_KIND_NUMBER) {
            struct clox_value val = clox_value_number(left.as.number + right.as.number);
            clox_interpreter_set_value(interpreter, val);
        } else if (left.kind == CLOX_VALUE_KIND_STRING && right.kind == CLOX_VALUE_KIND_STRING) {
            // this allocates a new string (we own this str)
            struct str concatenation = str_concat(left.as.string, right.as.string);

            // its str is a borrow from the above concatenation
            struct clox_value val = clox_value_string_str_borrow(concatenation);

            clox_interpreter_set_value(interpreter, val);
        } else {
            //TODO improve error handling
            assert(false && "plus(+) operator is only valid for operands of the same type (number or string)");
        }
        break;

    case TOKEN_KIND_MINUS:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_number(left.as.number - right.as.number));
        break;

    case TOKEN_KIND_STAR:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_number(left.as.number * right.as.number));
        break;

    case TOKEN_KIND_SLASH:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_number(left.as.number / right.as.number));
        break;

    case TOKEN_KIND_GREATER:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number > right.as.number));
        break;

    case TOKEN_KIND_GREATER_EQUAL:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number >= right.as.number));
        break;

    case TOKEN_KIND_LESS:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number < right.as.number));
        break;

    case TOKEN_KIND_LESS_EQUAL:
        //TODO improve error handling
        assert(left.kind == CLOX_VALUE_KIND_NUMBER);
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number <= right.as.number));
        break;

    case TOKEN_KIND_BANG_EQUAL:
        clox_interpreter_set_value(interpreter, clox_value_bool(!clox_value_is_equal(left, right)));
        break;

    case TOKEN_KIND_EQUAL_EQUAL:
        clox_interpreter_set_value(interpreter, clox_value_bool(clox_value_is_equal(left, right)));
        break;

    default:
        fputs("error: unknown binary operator: ", stderr);
        token_fprint(stderr, &expr_bin->operator);
        fputs("\n", stderr);
        assert(false && "unknown binary operator");
    }

    clox_value_free(&right);
    clox_value_free(&left);
}

static void eval_visit_expr_grouping(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;

    struct clox_value val = clox_interpreter_eval(interpreter, expr->value.grouping.expr);
    clox_interpreter_set_value(interpreter, val);
}

static void eval_visit_expr_literal(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_literal* expr_lit = &expr->value.literal;

    switch (expr_lit->kind) {
    case CLOX_AST_EXPR_LITERAL_KIND_NUMBER:
        clox_interpreter_set_value(interpreter, clox_value_number(expr_lit->value.number.val));
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_STRING:
        //NOTE this allocates a new string
        clox_interpreter_set_value(interpreter, clox_value_string_str_dup(expr_lit->value.string.val));
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_BOOL:
        clox_interpreter_set_value(interpreter, clox_value_bool(expr_lit->value.boolean.val));
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_NIL:
        clox_interpreter_set_value(interpreter, clox_value_nil());
        break;
    }
}

static void eval_visit_expr_unary(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_unary* expr_un = &expr->value.unary;

    // NOTE if we must do something after clox_interpreter_set_value, we must str_dup this if it's a string
    struct clox_value right = clox_interpreter_eval(interpreter, expr_un->right);

    switch (expr_un->operator.kind) {
    case TOKEN_KIND_BANG:
        //TODO improve error handling
        assert(right.kind == CLOX_VALUE_KIND_BOOL);
        clox_interpreter_set_value(interpreter, clox_value_bool(!clox_value_is_truthy(right)));
        break;

    case TOKEN_KIND_MINUS:
        //TODO improve error handling
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        clox_interpreter_set_value(interpreter, clox_value_number(-right.as.number));
        break;

    default:
        fputs("error: unknown unary operator: ", stderr);
        token_fprint(stderr, &expr_un->operator);
        fputs("\n", stderr);
        assert(false && "unknown binary operator");
    }
}

static void eval_visit_expr_var(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_var* expr_var = &expr->value.var;

    struct strview var_name = expr_var->name.lexeme;
    struct clox_value var_value;

    if (clox_env_get(&interpreter->env, var_name, &var_value) != 0) {
        fprintf(stderr, "error: runtime error\n");
        //TODO abort further execution (error handling)
    }

    clox_interpreter_set_value(interpreter, clox_value_dup(var_value));
}
