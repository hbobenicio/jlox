#include "strview.h"

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
