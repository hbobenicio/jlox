#include "interpreter-expr-visitor-eval.h"

#include <assert.h>

#include "ast/expr.h"
#include "ast/expr-visitor.h"
#include "value.h"
#include "interpreter.h"

static int eval_visit_expr_binary(struct clox_ast_expr* expr, void* userctx);
static int eval_visit_expr_grouping(struct clox_ast_expr* expr, void* userctx);
static int eval_visit_expr_literal(struct clox_ast_expr* expr, void* userctx);
static int eval_visit_expr_unary(struct clox_ast_expr* expr, void* userctx);
static int eval_visit_expr_var(struct clox_ast_expr* expr, void* userctx);

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

static int eval_visit_expr_binary(struct clox_ast_expr* expr, void* userctx) {
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
            fprintf(stderr, "error: line %zu: binary operator plus (a.k.a. '+') is only valid if both operands are numbers or strings. left operand is %s and right operand is %s",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        break;

    case TOKEN_KIND_MINUS:
        if (left.kind != CLOX_VALUE_KIND_NUMBER || right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr, "error: line %zu: binary operator '' requires both operands to be numbers. got left as %s and right as %s\n",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        clox_interpreter_set_value(interpreter, clox_value_number(left.as.number - right.as.number));
        break;

    case TOKEN_KIND_STAR:
        if (left.kind != CLOX_VALUE_KIND_NUMBER || right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr, "error: line %zu: binary operator '' requires both operands to be numbers. got left as %s and right as %s\n",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        clox_interpreter_set_value(interpreter, clox_value_number(left.as.number * right.as.number));
        break;

    case TOKEN_KIND_SLASH:
        if (left.kind != CLOX_VALUE_KIND_NUMBER || right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr, "error: line %zu: binary operator '' requires both operands to be numbers. got left as %s and right as %s\n",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        clox_interpreter_set_value(interpreter, clox_value_number(left.as.number / right.as.number));
        break;

    case TOKEN_KIND_GREATER:
        if (left.kind != CLOX_VALUE_KIND_NUMBER || right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr, "error: line %zu: binary operator '' requires both operands to be numbers. got left as %s and right as %s\n",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number > right.as.number));
        break;

    case TOKEN_KIND_GREATER_EQUAL:
        if (left.kind != CLOX_VALUE_KIND_NUMBER || right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr, "error: line %zu: binary operator '' requires both operands to be numbers. got left as %s and right as %s\n",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number >= right.as.number));
        break;

    case TOKEN_KIND_LESS:
        if (left.kind != CLOX_VALUE_KIND_NUMBER || right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr, "error: line %zu: binary operator '' requires both operands to be numbers. got left as %s and right as %s\n",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number < right.as.number));
        break;

    case TOKEN_KIND_LESS_EQUAL:
        if (left.kind != CLOX_VALUE_KIND_NUMBER || right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr, "error: line %zu: binary operator '' requires both operands to be numbers. got left as %s and right as %s\n",
                expr_bin->operator.line, clox_value_kind_to_cstr(left.kind), clox_value_kind_to_cstr(right.kind));
            goto err_free_right_and_left;
        }
        clox_interpreter_set_value(interpreter, clox_value_bool(left.as.number <= right.as.number));
        break;

    case TOKEN_KIND_BANG_EQUAL:
        clox_interpreter_set_value(interpreter, clox_value_bool(!clox_value_is_equal(left, right)));
        break;

    case TOKEN_KIND_EQUAL_EQUAL:
        clox_interpreter_set_value(interpreter, clox_value_bool(clox_value_is_equal(left, right)));
        break;

    default:
        fprintf(stderr, "error: line %zu: unknown binary operator: ", expr_bin->operator.line);
        token_fprint(stderr, &expr_bin->operator);
        fputs("\n", stderr);
        goto err_free_right_and_left;
    }

    clox_value_free(&right);
    clox_value_free(&left);
    return 0;

err_free_right_and_left:
    clox_value_free(&right);
    clox_value_free(&left);
    interpreter->value = clox_value_nil();
    return 1;
}

static int eval_visit_expr_grouping(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;

    struct clox_value val = clox_interpreter_eval(interpreter, expr->value.grouping.expr);
    clox_interpreter_set_value(interpreter, val);

    return 0;
}

static int eval_visit_expr_literal(struct clox_ast_expr* expr, void* userctx) {
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

    return 0;
}

static int eval_visit_expr_unary(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_unary* expr_un = &expr->value.unary;

    // NOTE this value is borrowed. If we need further recursive evaluation with the interpreter we need to dup this.
    struct clox_value right = clox_interpreter_eval(interpreter, expr_un->right);

    switch (expr_un->operator.kind) {
    case TOKEN_KIND_BANG:
        clox_interpreter_set_value(interpreter, clox_value_bool(!clox_value_is_truthy(right)));
        break;

    case TOKEN_KIND_MINUS:
        if (right.kind != CLOX_VALUE_KIND_NUMBER) {
            fprintf(stderr,"error: line %zu: minus unary operator (a.k.a. '-') can only be applied to numbers. got %s\n",
                expr_un->operator.line, clox_value_kind_to_cstr(right.kind));
            return 1;
        }
        clox_interpreter_set_value(interpreter, clox_value_number(-right.as.number));
        break;

    default:
        fprintf(stderr, "error: line %zu: unknown unary operator: ", expr_un->operator.line);
        token_fprint(stderr, &expr_un->operator);
        fputs("\n", stderr);
        return 1;
    }

    return 0;
}

static int eval_visit_expr_var(struct clox_ast_expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct clox_ast_expr_var* expr_var = &expr->value.var;

    struct strview var_name = expr_var->name.lexeme;
    struct clox_value var_value;

    if (clox_env_get(&interpreter->env, var_name, &var_value) != 0) {
        fputs("error: runtime error: undefined variable '", stderr);
        strview_fprint(var_name, stderr);
        fputs("'\n", stderr);
        return 1;
    }

    clox_interpreter_set_value(interpreter, clox_value_dup(var_value));

    return 0;
}
