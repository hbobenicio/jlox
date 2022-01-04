#ifndef CLOX_STRVIEW_H
#define CLOX_STRVIEW_H

#include <stddef.h>

struct strview {
    char* ptr;
    size_t len;
};

#endif
