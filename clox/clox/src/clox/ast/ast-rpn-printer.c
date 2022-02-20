#include "ast-rpn-printer.h"

#include <stdbool.h>
#include <assert.h>

#include "expr.h"
#include "expr-visitor.h"

struct ast_rpn_printer {
    FILE* file;
};

static int rpn_print_binary(struct clox_ast_expr* expr, void* userctx);
static int rpn_print_grouping(struct clox_ast_expr* expr, void* userctx);
static int rpn_print_literal(struct clox_ast_expr* expr, void* userctx);
static int rpn_print_unary(struct clox_ast_expr* expr, void* userctx);
static int rpn_print_var(struct clox_ast_expr* expr, void* userctx);

static const struct clox_ast_expr_visitor ast_rpn_printer_expr_visitor = {
    .visit_binary = rpn_print_binary,
    .visit_grouping = rpn_print_grouping,
    .visit_literal = rpn_print_literal,
    .visit_unary = rpn_print_unary,
    .visit_var = rpn_print_var
};

void ast_rpn_printer_println(struct clox_ast_expr* expr) {
    ast_rpn_printer_fprintln(stdout, expr);
}

void ast_rpn_printer_fprintln(FILE* file, struct clox_ast_expr* expr) {
    struct ast_rpn_printer ast_rpn_printer = {
        .file = file,
    };

    if (clox_ast_expr_accept(expr, &ast_rpn_printer_expr_visitor, &ast_rpn_printer) != 0) {
        fputs("error: failed printing expression\n", stderr);
        return;
    }

    fputs("\n", file);
}

static int rpn_print_binary(struct clox_ast_expr* expr, void* userctx) {
    struct ast_rpn_printer* ast_rpn_printer = userctx;
    struct clox_ast_expr_binary* expr_bin = &expr->value.binary;

    if (clox_ast_expr_accept(expr_bin->left, &ast_rpn_printer_expr_visitor, userctx) != 0) {
        return 1;
    }
    fprintf(ast_rpn_printer->file, " ");

    if (clox_ast_expr_accept(expr_bin->right, &ast_rpn_printer_expr_visitor, userctx) != 0) {
        return 1;
    }
    fprintf(ast_rpn_printer->file, " ");

    char op = expr_bin->operator.lexeme.ptr[0];
    fprintf(ast_rpn_printer->file, "%c", op);

    return 0;
}

static int rpn_print_grouping(struct clox_ast_expr* expr, void* userctx) {
    return clox_ast_expr_accept(expr->value.grouping.expr, &ast_rpn_printer_expr_visitor, userctx);
}

static int rpn_print_literal(struct clox_ast_expr* expr, void* userctx) {
    struct ast_rpn_printer* ast_rpn_printer = userctx;
    struct clox_ast_expr_literal* expr_lit = &expr->value.literal;
    
    switch (expr_lit->kind) {
    case CLOX_AST_EXPR_LITERAL_KIND_NUMBER:
        fprintf(ast_rpn_printer->file, "%lf", expr_lit->value.number.val);
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_STRING:
        fprintf(ast_rpn_printer->file, "%s", expr_lit->value.string.val.ptr);
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_BOOL:
        fprintf(ast_rpn_printer->file, "%s", (expr_lit->value.boolean.val) ? "true" : "false");
        break;

    case CLOX_AST_EXPR_LITERAL_KIND_NIL:
        fputs("nil", ast_rpn_printer->file);
        break;
    }

    return 0;
}

static int rpn_print_unary(struct clox_ast_expr* expr, void* userctx) {
    struct ast_rpn_printer* ast_rpn_printer = userctx;
    struct clox_ast_expr_unary* expr_un = &expr->value.unary;
    
    if (clox_ast_expr_accept(expr_un->right, &ast_rpn_printer_expr_visitor, userctx) != 0) {
        return 1;
    }

    char op = expr_un->operator.lexeme.ptr[0];
    fprintf(ast_rpn_printer->file, " %c", op);

    return 0;
}

static int rpn_print_var(struct clox_ast_expr* expr, void* userctx) {
    struct ast_rpn_printer* ast_rpn_printer = userctx;
    struct clox_ast_expr_var* expr_var = &expr->value.var;

    struct strview var_name = expr_var->name.lexeme;
    strview_fprint(var_name, ast_rpn_printer->file);

    return 0;
}
