#include "scanner.h"

#include <stdbool.h>
#include <ctype.h>

#include "stb_ds.h"

#include "commons.h"
#include "strview.h"
#include "token.h"

static void scanner_init(struct scanner* s, struct strview src);
static void scanner_scan_token(struct scanner* s);
static char scanner_advance(struct scanner* s);
static bool scanner_eof(const struct scanner* s);
static void scanner_add_token(struct scanner* s, enum token_kind kind);
static void scanner_add_token_number(struct scanner* s, struct strview lexeme, double val);
static bool scanner_match(struct scanner* s, char expected);
static void scanner_scan_line_comment(struct scanner* s);
static void scanner_scan_block_comment(struct scanner* s);
static void scanner_scan_string(struct scanner* s);
static void scanner_scan_number(struct scanner* s);
static void scanner_scan_identifier(struct scanner* s);
static enum token_kind scanner_keyword_or_identifier(struct strview lexeme);
static char scanner_peek(const struct scanner* s);
static char scanner_peek_next(const struct scanner* s);
static bool is_alpha(char c);

static void scanner_init(struct scanner* s, struct strview src) {
    scanner_free(s);
    s->input = src;
    s->start = s->current = 0;
    s->line = 1;
    s->tokens = NULL;
}

void scanner_free(struct scanner* s) {
    if (s->tokens != NULL) {
        arrfree(s->tokens);
    }
}

int scanner_scan_all(struct scanner* s, struct strview src) {
    scanner_init(s, src);
    while (!scanner_eof(s)) {
        s->start = s->current;
        scanner_scan_token(s);
    }
    arrpush(s->tokens, token_new_eof(s->line));
    return 0;
}

int scanner_scan_all_from_cstr(struct scanner* s, const char* src, size_t src_len) {
    return scanner_scan_all(s, strview_from_cstr(src, src_len));
}

static void scanner_scan_token(struct scanner* s) {
    char c = scanner_advance(s);
    switch (c) {
        case '(':
            scanner_add_token(s, TOKEN_KIND_LEFT_PAREN);
            break;

        case ')':
            scanner_add_token(s, TOKEN_KIND_RIGHT_PAREN);
            break;

        case '{':
            scanner_add_token(s, TOKEN_KIND_LEFT_BRACE);
            break;

        case '}':
            scanner_add_token(s, TOKEN_KIND_RIGHT_BRACE);
            break;

        case ',':
            scanner_add_token(s, TOKEN_KIND_COMMA);
            break;

        case '.':
            scanner_add_token(s, TOKEN_KIND_DOT);
            break;

        case '-':
            scanner_add_token(s, TOKEN_KIND_MINUS);
            break;

        case '+':
            scanner_add_token(s, TOKEN_KIND_PLUS);
            break;

        case ';':
            scanner_add_token(s, TOKEN_KIND_SEMICOLON);
            break;

        case '*':
            scanner_add_token(s, TOKEN_KIND_STAR);
            break;

        case '!':
            scanner_add_token(s, scanner_match(s, '=') ? TOKEN_KIND_BANG_EQUAL    : TOKEN_KIND_BANG);
            break;

        case '=':
            scanner_add_token(s, scanner_match(s, '=') ? TOKEN_KIND_EQUAL_EQUAL   : TOKEN_KIND_EQUAL);
            break;

        case '<':
            scanner_add_token(s, scanner_match(s, '=') ? TOKEN_KIND_LESS_EQUAL    : TOKEN_KIND_LESS);
            break;

        case '>':
            scanner_add_token(s, scanner_match(s, '=') ? TOKEN_KIND_GREATER_EQUAL : TOKEN_KIND_GREATER);
            break;

        case '/': {
            // double slashes `//` starts a line comment.
            if (scanner_match(s, '/')) {
                scanner_scan_line_comment(s);
            // '/*' starts a block comment which ends with '*/'
            } else if (scanner_match(s, '*')) {
                scanner_scan_block_comment(s);
            } else {
                scanner_add_token(s, TOKEN_KIND_SLASH);
            }
        } break;

        case ' ':
        case '\r':
        case '\t':
            /* noop. skip them */
            break;

        case '\n':
            s->line++;
            break;

        case '"':
            scanner_scan_string(s);
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            scanner_scan_number(s);
            break;

        // the erroneous character is still consumed by the earlier call to advance().
        // That’s important so that we don’t get stuck in an infinite loop.
        default: {
            if (is_alpha(c)) {
                scanner_scan_identifier(s);
            } else {
                fprintf(stderr, "error: unexpected character '%c' at line %zu\n", c, s->line);
            }
        }
    }
}

static void scanner_scan_line_comment(struct scanner* s) {
    while (scanner_peek(s) != '\n' && !scanner_eof(s)) {
        scanner_advance(s);
    }
}

static void scanner_scan_block_comment(struct scanner* s) {
    for (int depth = 1; depth > 0 && !scanner_eof(s); scanner_advance(s)) {
        char c = scanner_peek(s);
        char n = scanner_peek_next(s);
        
        if (c == '*' && n == '/') {
            depth--;
        } else if (c == '/' && n == '*') {
            depth++;
        } else if (c == '\n') {
            s->line++;
        }
    }
    if (scanner_eof(s)) {
        fprintf(stderr, "error: unterminated block comment. expecting token '*/'\n");
        return;
    }
    scanner_advance(s); // last '*'
    scanner_advance(s); // last '/'
}

static void scanner_scan_string(struct scanner* s) {
    size_t starting_line = s->line;

    while (scanner_peek(s) != '"' && !scanner_eof(s)) {
        if (scanner_peek(s) == '\n') {
            s->line++;
        }
        scanner_advance(s);
    }
    if (scanner_eof(s)) {
        fprintf(stderr, "error: line %zu: unterminated string. expecting token '\"'\n", starting_line);
        return;
    }

    // the closing "
    scanner_advance(s);

    //TODO strview_slice
    // This lexeme includes the quotes, like `"foo"`
    struct strview lexeme = {
        .ptr = s->input.ptr + s->start,
        .len = s->current - s->start,
    };

    // Trim the surrounding quotes.
    // If the lexeme is `"foo"`, this literal value must be just `foo`
    struct strview val = {
        .ptr = lexeme.ptr + 1, // skip the inicial quote
        .len = lexeme.len - 2, // counting down both quotes
    };

    // addToken(STRING, literalValue);
    struct token token = {
        .kind = TOKEN_KIND_STRING,
        .lexeme = lexeme,
        .line = s->line,
        .value = (union token_value) {
            .string = (struct token_value_string) {
                .val = val,
            },
        },
    };
    arrpush(s->tokens, token);
}

static void scanner_scan_number(struct scanner* s) {
    // peek returns '\0' if eof has been reached. isdigit will fail then.
    while (isdigit(scanner_peek(s))) {
        scanner_advance(s);
    }

    // check if it is a float
    if (scanner_peek(s) == '.' && isdigit(scanner_peek_next(s))) {
        // consume the '.'
        scanner_advance(s);

        while (isdigit(scanner_peek(s))) {
            scanner_advance(s);
        }
    }

    //TODO strview_slice
    struct strview lexeme = {
        .ptr = s->input.ptr + s->start,
        .len = s->current - s->start,
    };

    // convert the lexeme into a double value for the token
    //TODO convert this to strtod and properly check for errors!
    double val = atof(lexeme.ptr);

    scanner_add_token_number(s, lexeme, val);
}

static void scanner_scan_identifier(struct scanner* s) {
    while (isalnum(scanner_peek(s))) {
        scanner_advance(s);
    }

    //TODO strview_slice
    struct strview lexeme = {
        .ptr = s->input.ptr + s->start,
        .len = s->current - s->start,
    };

    // If the identifier's lexeme happens to be a reserved keyword, prioritize it
    // TokenType type = keywords.getOrDefault(lexeme, IDENTIFIER);
    // addToken(type);
    enum token_kind kind = scanner_keyword_or_identifier(lexeme);
    scanner_add_token(s, kind);
}

static enum token_kind scanner_keyword_or_identifier(struct strview lexeme) {

    #define IS_KEYWORD(kw) \
        (lexeme.len + 1 == sizeof(kw) && memcmp(lexeme.ptr, kw, lexeme.len) == 0)

    //TODO this could be improved with a hashtable or a regex
    if (IS_KEYWORD("and"))    return TOKEN_KIND_AND;
    if (IS_KEYWORD("class"))  return TOKEN_KIND_CLASS;
    if (IS_KEYWORD("else"))   return TOKEN_KIND_ELSE;
    if (IS_KEYWORD("false"))  return TOKEN_KIND_FALSE;
    if (IS_KEYWORD("for"))    return TOKEN_KIND_FOR;
    if (IS_KEYWORD("fun"))    return TOKEN_KIND_FUN;
    if (IS_KEYWORD("if"))     return TOKEN_KIND_IF;
    if (IS_KEYWORD("nil"))    return TOKEN_KIND_NIL;
    if (IS_KEYWORD("or"))     return TOKEN_KIND_OR;
    if (IS_KEYWORD("print"))  return TOKEN_KIND_PRINT;
    if (IS_KEYWORD("return")) return TOKEN_KIND_RETURN;
    if (IS_KEYWORD("super"))  return TOKEN_KIND_SUPER;
    if (IS_KEYWORD("this"))   return TOKEN_KIND_THIS;
    if (IS_KEYWORD("true"))   return TOKEN_KIND_TRUE;
    if (IS_KEYWORD("var"))    return TOKEN_KIND_VAR;
    if (IS_KEYWORD("while"))  return TOKEN_KIND_WHILE;

    #undef IS_KEYWORD

    return TOKEN_KIND_IDENTIFIER;
}

static bool is_alpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

static void scanner_add_token(struct scanner* s, enum token_kind kind) {
    struct token token = {
        .kind = kind,
        .line = s->line,
        // TODO this could improve into a call to strview_slice
        .lexeme = (struct strview) {
            .ptr = s->input.ptr + s->start,
            .len = s->current - s->start,
        },
        .value = {0},
    };
    arrpush(s->tokens, token);
}

static void scanner_add_token_number(struct scanner* s, struct strview lexeme, double val) {
    struct token token = token_new_number(s->line, lexeme, (union token_value) {
        .number = (struct token_value_number) {
            .val = val,
        },
    });
    arrpush(s->tokens, token);
}

static bool scanner_match(struct scanner* s, char expected) {
    if (scanner_peek(s) == expected) {
        s->current++;
        return true;
    }
    return false;
}

static char scanner_peek(const struct scanner* s) {
    if (scanner_eof(s)) {
        return '\0';
    }
    return s->input.ptr[s->current];
}

static char scanner_peek_next(const struct scanner* s) {
    if (s->current + 1 >= s->input.len) {
        return '\0';
    }
    return s->input.ptr[s->current + 1];
}

static char scanner_advance(struct scanner* s) {
    return s->input.ptr[s->current++];
}

static bool scanner_eof(const struct scanner* s) {
    return s->current >= s->input.len;
}
