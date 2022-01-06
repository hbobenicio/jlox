#ifndef CLOX_STR_H
#define CLOX_STR_H

#include <stddef.h>

struct str {
    char* ptr;
    size_t len;
    size_t cap;
};

#endif
