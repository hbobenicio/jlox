#include "str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"

struct str str_empty(void) {
    return (struct str) {
        .ptr = NULL,
        .cap = 0,
        .len = 0,
    };
}

bool str_is_empty(struct str str) {
    return str.ptr == NULL || str.len == 0 || str.cap == 0;
}

struct str str_dup(struct str str) {
    if (str_is_empty(str)) {
        return str_empty();
    }

    char* ptr = calloc(str.cap, sizeof(char));
    CLOX_ERR_PANIC_OOM_IF_NULL(ptr);

    memcpy(ptr, str.ptr, str.len);

    return (struct str) {
        .cap = str.cap,
        .len = str.len,
        .ptr = ptr,
    };
}

void str_free(struct str* str) {
    if (str->ptr) {
        free(str->ptr);
        str->ptr = NULL;
    }
    str->cap = str->len = 0;
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
    CLOX_ERR_PANIC_OOM_IF_NULL(str.ptr);

    memcpy(str.ptr, a.ptr, a.len);
    memcpy(str.ptr + a.len, b.ptr, b.len);

    return str;
}
