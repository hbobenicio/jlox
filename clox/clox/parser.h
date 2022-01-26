#ifndef CLOX_PARSER_H
#define CLOX_PARSER_H

#include <stddef.h>

struct token;

struct parser {
    struct token* tokens;
    size_t current;
};

void parser_init(struct parser* p, struct token* tokens);

struct expr* parser_parse(struct parser* p);
struct expr* parser_parse_expr(struct parser* p);
struct expr* parser_parse_expr_equality(struct parser* p);
struct expr* parser_parse_expr_comparison(struct parser* p);
struct expr* parser_parse_expr_term(struct parser* p);
struct expr* parser_parse_expr_factor(struct parser* p);
struct expr* parser_parse_expr_unary(struct parser* p);
struct expr* parser_parse_expr_primary(struct parser* p);

#endif
