#include "parser.h"

#include <stdbool.h>
#include "token.h"
#include "expr.h"

static bool match(struct parser* p, enum token_kind token_kind);
static struct token previous(struct parser* p);

void parser_init(struct parser* p, struct token* tokens) {
    p->tokens = tokens;
    p->current = 0;
}

struct expr* parser_parse_expr(struct parser* p) {
    return parser_parse_expr_equality(p);
}

// equality -> comparison ( ( "!=" | "==" ) comparison )* ;
struct expr* parser_parse_expr_equality(struct parser* p) {
    struct expr* expr = parser_parse_expr_comparison(p);
    if (expr == NULL) {
        return NULL;
    }

    while (match(p, TOKEN_KIND_BANG_EQUAL) && match(p, TOKEN_KIND_EQUAL_EQUAL)) {
        struct token operator = previous(p);
        struct expr* right = parser_parse_expr_comparison(p);
        if (right == NULL) {
            //TODO free recursively expr
            return NULL;
        }
        expr = expr_binary_new(expr, operator, right);
    }

    return expr;
}

struct expr* parser_parse_expr_comparison(struct parser* p) {
    // TODO
    return NULL;
}

static bool match(struct parser* p, enum token_kind token_kind) {
    // TODO
    return false;
}

static struct token previous(struct parser* p) {
    return p->tokens[p->current - 1];
}
