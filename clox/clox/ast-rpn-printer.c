#include "ast-rpn-printer.h"

#include "expr.h"
#include <stdbool.h>
#include <assert.h>

struct ast_rpn_printer {
    FILE* file;
};

static void ast_rpn_printer_visit_binary(struct expr* expr, void* userctx);
static void ast_rpn_printer_visit_grouping(struct expr* expr, void* userctx);
static void ast_rpn_printer_visit_literal(struct expr* expr, void* userctx);
static void ast_rpn_printer_visit_unary(struct expr* expr, void* userctx);

static const struct expr_visitor ast_rpn_printer_expr_visitor = {
    .visit_binary = ast_rpn_printer_visit_binary,
    .visit_grouping = ast_rpn_printer_visit_grouping,
    .visit_literal = ast_rpn_printer_visit_literal,
    .visit_unary = ast_rpn_printer_visit_unary,
};

void ast_rpn_printer_println(struct expr* expr) {
    ast_rpn_printer_fprintln(stdout, expr);
}

void ast_rpn_printer_fprintln(FILE* file, struct expr* expr) {
    struct ast_rpn_printer ast_rpn_printer = {
        .file = file,
    };

    expr_accept(expr, &ast_rpn_printer_expr_visitor, &ast_rpn_printer);

    fputs("\n", file);
}

static void ast_rpn_printer_visit_binary(struct expr* expr, void* userctx) {
    struct ast_rpn_printer* ast_rpn_printer = userctx;
    struct expr_binary* expr_bin = &expr->value.binary;

    expr_accept(expr_bin->left, &ast_rpn_printer_expr_visitor, userctx);
    fprintf(ast_rpn_printer->file, " ");

    expr_accept(expr_bin->right, &ast_rpn_printer_expr_visitor, userctx);
    fprintf(ast_rpn_printer->file, " ");

    char op = expr_bin->operator.lexeme.ptr[0];
    fprintf(ast_rpn_printer->file, "%c", op);
}

static void ast_rpn_printer_visit_grouping(struct expr* expr, void* userctx) {
    expr_accept(expr->value.grouping.expr, &ast_rpn_printer_expr_visitor, userctx);
}

static void ast_rpn_printer_visit_literal(struct expr* expr, void* userctx) {
    struct ast_rpn_printer* ast_rpn_printer = userctx;
    struct expr_literal* expr_lit = &expr->value.literal;
    
    switch (expr_lit->kind) {
    case EXPR_LITERAL_KIND_NUMBER:
        fprintf(ast_rpn_printer->file, "%lf", expr_lit->value.number.val);
        break;

    case EXPR_LITERAL_KIND_STRING:
        fprintf(ast_rpn_printer->file, "%s", expr_lit->value.string.val.ptr);
        break;

    case EXPR_LITERAL_KIND_BOOL:
        fprintf(ast_rpn_printer->file, "%s", (expr_lit->value.boolean.val) ? "true" : "false");
        break;

    case EXPR_LITERAL_KIND_NIL:
        fputs("nil", ast_rpn_printer->file);
        break;

    default:
        assert(false && "unsupported expr_lit variant kind. did you added a new variant to it recently?");
        break;
    }
}

static void ast_rpn_printer_visit_unary(struct expr* expr, void* userctx) {
    struct ast_rpn_printer* ast_rpn_printer = userctx;
    struct expr_unary* expr_un = &expr->value.unary;
    
    expr_accept(expr_un->right, &ast_rpn_printer_expr_visitor, userctx);

    char op = expr_un->operator.lexeme.ptr[0];
    fprintf(ast_rpn_printer->file, " %c", op);
}
