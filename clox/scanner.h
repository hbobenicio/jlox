#ifndef CLOX_SCANNER_H
#define CLOX_SCANNER_H

#include <stddef.h>

#include "strview.h"

struct scanner {
    size_t start;
    size_t current;
    size_t line;
};

void scanner_init(struct scanner* s);
struct token* scanner_scan_all(struct scanner* s, struct strview src);

#endif
