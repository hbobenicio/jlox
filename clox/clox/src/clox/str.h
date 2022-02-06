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
bool str_is_empty(struct str);

struct str str_dup(struct str str);
void str_free(struct str* str);
bool str_equals(struct str a, struct str b);
struct str str_concat(struct str a, struct str b);

#endif
