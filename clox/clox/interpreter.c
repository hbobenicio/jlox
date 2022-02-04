#include "interpreter.h"

#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "expr.h"

static void eval_visit_binary(struct expr* expr, void* userctx);
static void eval_visit_grouping(struct expr* expr, void* userctx);
static void eval_visit_literal(struct expr* expr, void* userctx);
static void eval_visit_unary(struct expr* expr, void* userctx);

static bool is_truthy(struct clox_value value);
static bool is_equal(struct clox_value left, struct clox_value right);

static const struct expr_visitor eval_visitor = {
    .visit_binary = eval_visit_binary,
    .visit_grouping = eval_visit_grouping,
    .visit_literal = eval_visit_literal,
    .visit_unary = eval_visit_unary,
};

struct clox_value clox_interpreter_eval(struct clox_interpreter* interpreter, struct expr* expr) {
    expr_accept(expr, &eval_visitor, interpreter);
    return interpreter->value;
}

static void eval_visit_binary(struct expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct expr_binary* expr_bin = &expr->value.binary;
    
    struct clox_value left = clox_interpreter_eval(interpreter, expr_bin->left);
    struct clox_value right = clox_interpreter_eval(interpreter, expr_bin->right);
    
    switch(expr_bin->operator.kind) {
        case TOKEN_KIND_PLUS:
            if (left.kind == CLOX_VALUE_KIND_NUMBER && right.kind == CLOX_VALUE_KIND_NUMBER) {
                interpreter->value = clox_value_number(left.as.number + right.as.number);
            } else if (left.kind == CLOX_VALUE_KIND_STRING && right.kind == CLOX_VALUE_KIND_STRING) {
                //FIXME care about leaks from here... str_concat allocates a new buffer
                interpreter->value = clox_value_string(str_concat(left.as.string, right.as.string));
            } else {
                //TODO improve error handling
                assert(false && "plus(+) operator is only valid for operands of the same type (number or string)");
            }
            break;

        case TOKEN_KIND_MINUS:
            //TODO improve error handling
            assert(left.kind == CLOX_VALUE_KIND_NUMBER);
            assert(right.kind == CLOX_VALUE_KIND_NUMBER);
            interpreter->value = clox_value_number(left.as.number - right.as.number);
            break;

        case TOKEN_KIND_STAR:
            //TODO improve error handling
            assert(left.kind == CLOX_VALUE_KIND_NUMBER);
            assert(right.kind == CLOX_VALUE_KIND_NUMBER);
            interpreter->value = clox_value_number(left.as.number * right.as.number);
            break;

        case TOKEN_KIND_SLASH:
            //TODO improve error handling
            assert(left.kind == CLOX_VALUE_KIND_NUMBER);
            assert(right.kind == CLOX_VALUE_KIND_NUMBER);
            interpreter->value = clox_value_number(left.as.number / right.as.number);
            break;

        case TOKEN_KIND_GREATER:
            //TODO improve error handling
            assert(left.kind == CLOX_VALUE_KIND_NUMBER);
            assert(right.kind == CLOX_VALUE_KIND_NUMBER);
            interpreter->value = clox_value_bool(left.as.number > right.as.number);
            break;

        case TOKEN_KIND_GREATER_EQUAL:
            //TODO improve error handling
            assert(left.kind == CLOX_VALUE_KIND_NUMBER);
            assert(right.kind == CLOX_VALUE_KIND_NUMBER);
            interpreter->value = clox_value_bool(left.as.number >= right.as.number);
            break;

        case TOKEN_KIND_LESS:
            //TODO improve error handling
            assert(left.kind == CLOX_VALUE_KIND_NUMBER);
            assert(right.kind == CLOX_VALUE_KIND_NUMBER);
            interpreter->value = clox_value_bool(left.as.number < right.as.number);
            break;

        case TOKEN_KIND_LESS_EQUAL:
            //TODO improve error handling
            assert(left.kind == CLOX_VALUE_KIND_NUMBER);
            assert(right.kind == CLOX_VALUE_KIND_NUMBER);
            interpreter->value = clox_value_bool(left.as.number <= right.as.number);
            break;

        case TOKEN_KIND_BANG_EQUAL:
            interpreter->value = clox_value_bool(!is_equal(left, right));
            break;

        case TOKEN_KIND_EQUAL_EQUAL:
            interpreter->value = clox_value_bool(is_equal(left, right));
            break;

        default:
            assert(false && "unsupported binary operator. did you add a new binary operator recently?");
            break;
    }
}

static void eval_visit_grouping(struct expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;

    interpreter->value = clox_interpreter_eval(interpreter, expr->value.grouping.expr);
}

static void eval_visit_literal(struct expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct expr_literal* expr_lit = &expr->value.literal;

    switch (expr_lit->kind) {
    case EXPR_LITERAL_KIND_NUMBER:
        interpreter->value = clox_value_number(expr_lit->value.number.val);
        break;

    case EXPR_LITERAL_KIND_STRING:
        //FIXME it should allocate a new string, instead of borrowing it.
        //      values must have independent lifecycle from the AST
        interpreter->value = clox_value_string(expr_lit->value.string.val);
        break;

    case EXPR_LITERAL_KIND_BOOL:
        interpreter->value = clox_value_bool(expr_lit->value.boolean.val);
        break;

    case EXPR_LITERAL_KIND_NIL:
        interpreter->value = clox_value_nil();
        break;

    default:
        assert(false && "unsupported expr_lit variant kind. did you add a new variant to it recently?");
        break;
    }
}

static void eval_visit_unary(struct expr* expr, void* userctx) {
    struct clox_interpreter* interpreter = userctx;
    struct expr_unary* expr_un = &expr->value.unary;

    struct clox_value right = clox_interpreter_eval(interpreter, expr_un->right);

    switch (expr_un->operator.kind) {
    case TOKEN_KIND_BANG:
        //TODO improve error handling
        assert(right.kind == CLOX_VALUE_KIND_BOOL);
        interpreter->value = clox_value_bool(!is_truthy(right));
        break;

    case TOKEN_KIND_MINUS:
        //TODO improve error handling
        assert(right.kind == CLOX_VALUE_KIND_NUMBER);
        interpreter->value = clox_value_number(-right.as.number);
        break;
    
    default:
        assert(false && "unsupported unary operator. did you added a new unary operator recently?");
        break;
    }
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
