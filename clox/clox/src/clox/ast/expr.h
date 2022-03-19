#ifndef CLOX_EXPR_H
#define CLOX_EXPR_H

#include <stdbool.h>
#include <clox/token.h>
#include <clox/str.h>
#include <clox/strview.h>

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
    CLOX_AST_EXPR_KIND_VAR,
    CLOX_AST_EXPR_KIND_ASSIGN,
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

struct clox_ast_expr_var {
    struct token name;
};

struct clox_ast_expr_assign {
    struct token name;
    struct clox_ast_expr* value;
};

struct clox_ast_expr {
    enum clox_ast_expr_kind kind;
    union {
        struct clox_ast_expr_binary binary;
        struct clox_ast_expr_grouping grouping;
        struct clox_ast_expr_literal literal;
        struct clox_ast_expr_unary unary;
        struct clox_ast_expr_var var;
        struct clox_ast_expr_assign assign;
    } value;
};

struct clox_ast_expr* clox_ast_expr_binary_new(struct clox_ast_expr* left, struct token operator, struct clox_ast_expr* right);
struct clox_ast_expr* clox_ast_expr_unary_new(struct token operator, struct clox_ast_expr* right);
struct clox_ast_expr* clox_ast_expr_var_new(struct token name);
struct clox_ast_expr* clox_ast_expr_assign_new(struct token name, struct clox_ast_expr* value);
struct clox_ast_expr* clox_ast_expr_literal_bool_new(bool val);
struct clox_ast_expr* clox_ast_expr_literal_nil_new(void);
struct clox_ast_expr* clox_ast_expr_literal_string_new(struct strview sv);
struct clox_ast_expr* clox_ast_expr_literal_number_new(double val);
struct clox_ast_expr* clox_ast_expr_grouping_new(struct clox_ast_expr* expr);
struct clox_ast_expr  clox_ast_expr_literal_number_create(double val);
struct clox_ast_expr  clox_ast_expr_grouping_create(struct clox_ast_expr* expr);

void clox_ast_expr_free(struct clox_ast_expr* expr);

#endif
