#ifndef CLOX_STR_H
#define CLOX_STR_H

#include <stddef.h>
#include <stdbool.h>

struct str {
    char* ptr;
    size_t len;
    size_t cap;
};

bool str_equals(struct str a, struct str b);

#endif
