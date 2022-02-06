#ifndef CLOX_STRVIEW_H
#define CLOX_STRVIEW_H

#include <stddef.h>
#include "str.h"

struct strview {
    const char* ptr;
    size_t len;
};

struct strview strview_empty();
struct strview strview_from_cstr(const char* src, size_t src_len);
struct strview strview_from_str(struct str str);

#endif
