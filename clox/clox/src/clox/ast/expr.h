#ifndef CLOX_EXPR_H
#define CLOX_EXPR_H

#include <stdbool.h>
#include <clox/token.h>
#include <clox/str.h>
#include <clox/strview.h>
#include "tree-traversal-order.h"

enum clox_ast_expr_literal_kind {
    CLOX_AST_EXPR_LITERAL_KIND_NUMBER,
    CLOX_AST_EXPR_LITERAL_KIND_STRING,
    CLOX_AST_EXPR_LITERAL_KIND_BOOL,
    CLOX_AST_EXPR_LITERAL_KIND_NIL,
};

struct clox_ast_expr_literal_number {
    double val;
};

struct clox_ast_expr_literal_string {
    // TODO this could be improved maybe... it's ok for now
    struct str val;
};

struct clox_ast_expr_literal_bool {
    bool val;
};

enum clox_ast_expr_kind {
    CLOX_AST_EXPR_KIND_BINARY,
    CLOX_AST_EXPR_KIND_GROUPING,
    CLOX_AST_EXPR_KIND_LITERAL,
    CLOX_AST_EXPR_KIND_UNARY,
};

struct clox_ast_expr_binary {
    struct clox_ast_expr* left;
    struct token operator;
    struct clox_ast_expr* right;
};

struct clox_ast_expr_grouping {
    struct clox_ast_expr* expr;
};

struct clox_ast_expr_literal {
    enum clox_ast_expr_literal_kind kind;
    union {
        struct clox_ast_expr_literal_number number;
        struct clox_ast_expr_literal_string string;
        struct clox_ast_expr_literal_bool boolean;
    } value;
};

struct clox_ast_expr_unary {
    struct token operator;
    struct clox_ast_expr* right;
};

struct clox_ast_expr {
    enum clox_ast_expr_kind kind;
    union {
        struct clox_ast_expr_binary binary;
        struct clox_ast_expr_grouping grouping;
        struct clox_ast_expr_literal literal;
        struct clox_ast_expr_unary unary;
    } value;
};

struct clox_ast_expr* expr_binary_new(struct clox_ast_expr* left, struct token operator, struct clox_ast_expr* right);
struct clox_ast_expr* expr_unary_new(struct token operator, struct clox_ast_expr* right);
struct clox_ast_expr* expr_literal_bool_new(bool val);
struct clox_ast_expr* expr_literal_nil_new(void);
struct clox_ast_expr* expr_literal_string_new(struct strview sv);
struct clox_ast_expr* expr_literal_number_new(double val);
struct clox_ast_expr* expr_grouping_new(struct clox_ast_expr* expr);
struct clox_ast_expr  expr_literal_number_create(double val);
struct clox_ast_expr  expr_grouping_create(struct clox_ast_expr* expr);

void expr_free(struct clox_ast_expr* expr);

//TODO improve this by replacing the void return type to int, for error handling
struct expr_visitor {
    void (*visit_binary)(struct clox_ast_expr* expr, void* userctx);
    void (*visit_grouping)(struct clox_ast_expr* expr, void* userctx);
    void (*visit_literal)(struct clox_ast_expr* expr, void* userctx);
    void (*visit_unary)(struct clox_ast_expr* expr, void* userctx);
};

void expr_accept(struct clox_ast_expr* expr, const struct expr_visitor* visitor, void* userctx);

#endif
