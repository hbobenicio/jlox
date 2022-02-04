#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct str str_empty(void) {
    return (struct str) {
        .ptr = NULL,
        .cap = 0,
        .len = 0,
    };
}

bool str_equals(struct str a, struct str b) {
    if (a.len != b.len) {
        return false;
    }
    return memcmp(a.ptr, b.ptr, a.len) == 0;
}

struct str str_concat(struct str a, struct str b) {
    struct str str;
    
    str.len = a.len + b.len;
    str.cap = str.len + 1; // '\0'

    str.ptr = calloc(str.cap, sizeof(char));
    if (str.ptr == NULL) {
        fprintf(stderr, "error: out of memory.\n");
        return str_empty();
    }

    memcpy(str.ptr, a.ptr, a.len);
    memcpy(str.ptr + a.len, b.ptr, b.len);

    return str;
}
