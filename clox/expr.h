#ifndef CLOX_EXPR_H
#define CLOX_EXPR_H

#include "token.h"
#include "str.h"

struct expr {
    enum expr_kind {
        EXPR_BINARY,
        EXPR_GROUPING,
        EXPR_LITERAL,
        EXPR_UNARY,
    } kind;

    union {
        struct expr_binary {
            struct expr* left;
            struct token operator;
            struct expr* right;
        } binary;

        struct expr_grouping {
            struct expr* expr;
        } grouping;

        struct expr_literal {
            enum expr_literal_kind {
                EXPR_LITERAL_NUMBER,
                EXPR_LITERAL_STRING,
            } kind;

            union {
                struct expr_literal_number {
                    double val;
                } number;

                struct expr_literal_string {
                    // TODO this could be improved maybe... it's ok for now
                    struct str val;
                } string;
            } value;
        } literal;

        struct expr_unary {
            struct token operator;
            struct expr* right;
        } unary;

    } value;
};

#endif
