#include "scanner.h"

void scanner_init(struct scanner* s) {
    *s = (struct scanner) {
        .start = 0,
        .current = 0,
        .line = 1,
    };
}

struct token* scanner_scan_all(struct scanner* s, struct strview src) {
    (void) s;
    (void) src;
    // TODO
    return NULL;
}
