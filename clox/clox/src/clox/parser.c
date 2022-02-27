#include "parser.h"

#include <stdbool.h>
#include <assert.h>

#include "stb_ds.h"

#include "commons.h"
#include "token.h"
#include "ast/expr.h"
#include "ast/statement.h"
#include "ast/program.h"

static bool match(struct parser* p, enum token_kind token_kind);
static bool check(const struct parser* p, enum token_kind token_kind);
static int consume(struct parser* p, enum token_kind token_kind, const char* msg);

static struct token advance(struct parser* p);
static struct token peek(const struct parser* p);
static struct token previous(const struct parser* p);
static bool end_of_input(const struct parser* p);

void parser_init(struct parser* p, struct token* tokens) {
    p->tokens = tokens;
    p->current = 0;
    
#ifdef DEBUG_DUMP_TOKENS
    for (long int i = 0; i < arrlen(tokens); i++) {
        token_fprint(stderr, &tokens[i]);
        fputs(" ", stderr);
    }
    fputs("\n", stderr);
#endif
}

struct clox_ast_program* parser_parse(struct parser* p) {
    struct clox_ast_program* prog = clox_ast_program_new();

    while (!end_of_input(p)) {
        // struct clox_ast_statement* stmt = parser_parse_statement(p);
        struct clox_ast_statement* stmt = parser_parse_declaration(p);
        if (stmt == NULL) {
            fprintf(stderr, "error: line %zu: failed to parse statement\n", peek(p).line);
            clox_ast_program_free(prog);
            return NULL;
        }
        clox_ast_program_add_statement(prog, stmt);
    }

    return prog;
}

struct clox_ast_statement* parser_parse_declaration(struct parser* p) {
    // TODO add error handling and error sincronization
    if (match(p, TOKEN_KIND_VAR)) {
        return parser_parse_var_declaration_statement(p);
    }
    return parser_parse_statement(p);
}


struct clox_ast_expr* parser_parse_expr(struct parser* p) {
    return parser_parse_expr_equality(p);
}

// equality -> comparison ( ("!=" | "==") comparison )* ;
struct clox_ast_expr* parser_parse_expr_equality(struct parser* p) {
    struct clox_ast_expr* expr = parser_parse_expr_comparison(p);
    if (expr == NULL) {
        return NULL;
    }

    while (match(p, TOKEN_KIND_BANG_EQUAL) || match(p, TOKEN_KIND_EQUAL_EQUAL)) {
        struct token operator = previous(p);

        struct clox_ast_expr* right = parser_parse_expr_comparison(p);
        if (right == NULL) {
            //TODO free expr recursively
            char op[4] = {0};
            memcpy(op, operator.lexeme.ptr, MIN(operator.lexeme.len, ARRAY_SIZE(op)));
            fprintf(stderr, "error: line %zu: invalid right hand side expression from binary operator '%s'\n", peek(p).line, op);
            return NULL;
        }

        expr = clox_ast_expr_binary_new(expr, operator, right);
    }

    return expr;
}

// comparison -> term ( (">" | ">=" | "<" | "<=") term )* ;
struct clox_ast_expr* parser_parse_expr_comparison(struct parser* p) {
    struct clox_ast_expr* expr = parser_parse_expr_term(p);
    if (expr == NULL) {
        return NULL;
    }

    while (match(p, TOKEN_KIND_GREATER) || match(p, TOKEN_KIND_GREATER_EQUAL) || match(p, TOKEN_KIND_LESS) || match(p, TOKEN_KIND_LESS_EQUAL)) {
        struct token operator = previous(p);

        struct clox_ast_expr* right = parser_parse_expr_term(p);
        if (right == NULL) {
            //TODO free expr recursively
            char op[4] = {0};
            memcpy(op, operator.lexeme.ptr, MIN(operator.lexeme.len, ARRAY_SIZE(op)));
            fprintf(stderr, "error: line %zu: invalid right hand side expression from binary operator '%s'\n", peek(p).line, op);
            return NULL;
        }

        expr = clox_ast_expr_binary_new(expr, operator, right);
    }

    return expr;
}

// term -> factor ( ("-" | "+") factor )*
struct clox_ast_expr* parser_parse_expr_term(struct parser* p) {
    struct clox_ast_expr* expr = parser_parse_expr_factor(p);
    if (expr == NULL) {
        return NULL;
    }

    while (match(p, TOKEN_KIND_MINUS) || match(p, TOKEN_KIND_PLUS)) {
        struct token operator = previous(p);

        struct clox_ast_expr* right = parser_parse_expr_factor(p);
        if (right == NULL) {
            //TODO free expr recursively
            char op[4] = {0};
            memcpy(op, operator.lexeme.ptr, MIN(operator.lexeme.len, ARRAY_SIZE(op)));
            fprintf(stderr, "error: line %zu: invalid right hand side expression from binary operator '%s'\n", peek(p).line, op);
            return NULL;
        }

        expr = clox_ast_expr_binary_new(expr, operator, right);
    }

    return expr;
}

// factor -> unary ( ("/" | "*") unary )*
struct clox_ast_expr* parser_parse_expr_factor(struct parser* p) {
    struct clox_ast_expr* expr = parser_parse_expr_unary(p);
    if (expr == NULL) {
        return NULL;
    }

    while (match(p, TOKEN_KIND_SLASH) || match(p, TOKEN_KIND_STAR)) {
        struct token operator = previous(p);

        struct clox_ast_expr* right = parser_parse_expr_unary(p);
        if (right == NULL) {
            //TODO free expr recursively
            char op[4] = {0};
            memcpy(op, operator.lexeme.ptr, MIN(operator.lexeme.len, ARRAY_SIZE(op)));
            fprintf(stderr, "error: line %zu: invalid right hand side expression from binary operator '%s'\n", peek(p).line, op);
            return NULL;
        }

        expr = clox_ast_expr_binary_new(expr, operator, right);
    }

    return expr;
}

// unary -> ("!" | "-") unary | primary
struct clox_ast_expr* parser_parse_expr_unary(struct parser* p) {
    if (match(p, TOKEN_KIND_BANG) || match(p, TOKEN_KIND_MINUS)) {
        struct token operator = previous(p);

        struct clox_ast_expr* right = parser_parse_expr_unary(p);
        if (right == NULL) {
            //TODO free expr recursively
            char op[4] = {0};
            memcpy(op, operator.lexeme.ptr, MIN(operator.lexeme.len, ARRAY_SIZE(op)));
            fprintf(stderr, "error: line %zu: invalid right hand side expression from binary operator '%s'\n", peek(p).line, op);
            return NULL;
        }

        return clox_ast_expr_unary_new(operator, right);
    }

    return parser_parse_expr_primary(p);
}

// primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
struct clox_ast_expr* parser_parse_expr_primary(struct parser* p) {
    if (match(p, TOKEN_KIND_NUMBER)) {
        struct token token = previous(p);
        return clox_ast_expr_literal_number_new(token.value.number.val);
    }
    if (match(p, TOKEN_KIND_STRING)) {
        struct token token = previous(p);
        return clox_ast_expr_literal_string_new(token.value.string.val);
    }
    if (match(p, TOKEN_KIND_TRUE)) {
        return clox_ast_expr_literal_bool_new(true);
    }
    if (match(p, TOKEN_KIND_FALSE)) {
        return clox_ast_expr_literal_bool_new(false);
    }
    if (match(p, TOKEN_KIND_NIL)) {
        return clox_ast_expr_literal_nil_new();
    }
    if (match(p, TOKEN_KIND_LEFT_PAREN)) {
        struct clox_ast_expr* expr = parser_parse_expr(p);
        if (expr == NULL) {
            return NULL;
        }
        if (consume(p, TOKEN_KIND_RIGHT_PAREN, "expect ')' after expression") != 0) {
            // TODO free expr recursively
            return NULL;
        }
        return clox_ast_expr_grouping_new(expr);
    }
    if (match(p, TOKEN_KIND_IDENTIFIER)) {
        return clox_ast_expr_var_new(previous(p));
    }
    struct token current_token = peek(p);
    fprintf(stderr, "error: line %zu: expecting a primary expression (a literal or an opening parentesis '('), got '%s'\n", current_token.line, token_to_cstr(&current_token));
    return NULL;
}

struct clox_ast_statement* parser_parse_statement(struct parser* p) {
    if (match(p, TOKEN_KIND_PRINT)) {
        return parser_parse_print_statement(p);
    }
    return parser_parse_expr_statement(p);
}

struct clox_ast_statement* parser_parse_print_statement(struct parser* p) {
    struct clox_ast_expr* expr = parser_parse_expr(p);
    if (expr == NULL) {
        fprintf(stderr, "error: failed to parse print statement expression\n");
        return NULL;
    }
    consume(p, TOKEN_KIND_SEMICOLON, "error: expecting ';' after print expression operand");
    return clox_ast_statement_new_print(expr);
}

struct clox_ast_statement* parser_parse_expr_statement(struct parser* p) {
    struct clox_ast_expr* expr = parser_parse_expr(p);
    if (expr == NULL) {
        fprintf(stderr, "error: failed to parse expression statement\n");
        return NULL;
    }
    consume(p, TOKEN_KIND_SEMICOLON, "error: expecting ';' after expression");
    return clox_ast_statement_new_expr(expr);
}

struct clox_ast_statement* parser_parse_var_declaration_statement(struct parser* p) {
    consume(p, TOKEN_KIND_IDENTIFIER, "error: expecting variable name");
    struct token var_name = previous(p);

    struct clox_ast_expr* initializer = NULL;
    if (match(p, TOKEN_KIND_EQUAL)) {
        initializer = parser_parse_expr(p);
    }

    consume(p, TOKEN_KIND_SEMICOLON, "error: expecting ';' after variable declaration");
    return clox_ast_statement_new_var(var_name, initializer);
}

static int consume(struct parser* p, enum token_kind token_kind, const char* msg) {
    if (check(p, token_kind)) {
        advance(p);
        return 0;
    }

    struct token current_token = peek(p);
    fprintf(stderr, "error: line %zu: %s: expected token %s, got %s\n",
        current_token.line,
        msg,
        token_kind_to_cstr(token_kind),
        token_to_cstr(&current_token)
    );

    return 1;
}

static bool match(struct parser* p, enum token_kind token_kind) {
    if (check(p, token_kind)) {
        advance(p);
        return true;
    }
    return false;
}

static bool check(const struct parser* p, enum token_kind token_kind) {
    if (end_of_input(p)) {
        return false;
    }
    return peek(p).kind == token_kind;
}

static struct token advance(struct parser* p) {
    if (!end_of_input(p)) {
        p->current++;
    }
    return previous(p);
}

static struct token peek(const struct parser* p) {
    return p->tokens[p->current];
}

static struct token previous(const struct parser* p) {
    assert(p->current > 0);
    return p->tokens[p->current - 1];
}

static bool end_of_input(const struct parser* p) {
    return peek(p).kind == TOKEN_KIND_EOF;
    // return p->current >= arrlen(p->tokens);
}
