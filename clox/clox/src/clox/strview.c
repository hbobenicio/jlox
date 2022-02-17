#include "strview.h"

#include <assert.h>

struct strview strview_empty() {
    return (struct strview) {
        .ptr = "",
        .len = 0,
    };
}

struct strview strview_from_cstr(const char* src, size_t src_len) {
    return (struct strview) {
        .ptr = src,
        .len = src_len,
    };
}

struct strview strview_from_str(struct str str) {
    return (struct strview) {
        .ptr = str.ptr,
        .len = str.len,
    };
}

bool strview_is_empty(struct strview sv) {
    // Should I compare ptr to ""?
    return sv.len == 0;
}

void strview_print(struct strview sv) {
    strview_fprint(sv, stdin);
}

void strview_fprint(struct strview sv, FILE* file) {
    assert(fwrite(sv.ptr, sizeof(char), sv.len, file) == sv.len);
}
