#ifndef CLOX_STR_H
#define CLOX_STR_H

#include <stddef.h>
#include <stdbool.h>

struct str {
    char* ptr;
    size_t len;
    size_t cap;
};

struct str str_empty(void);

bool str_equals(struct str a, struct str b);
struct str str_concat(struct str a, struct str b);

#endif
