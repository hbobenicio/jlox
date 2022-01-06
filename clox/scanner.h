#ifndef CLOX_SCANNER_H
#define CLOX_SCANNER_H

#include <stddef.h>

#include "strview.h"

struct token;
struct scanner {
    struct strview input;
    size_t start;
    size_t current;
    size_t line;
    struct token* tokens;
};

int scanner_scan_all(struct scanner* s, struct strview src);
int scanner_scan_all_from_cstr(struct scanner* s, const char* src, size_t src_len);

#endif
