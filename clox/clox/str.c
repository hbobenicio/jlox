#include "str.h"

#include <string.h>

bool str_equals(struct str a, struct str b) {
    if (a.len != b.len) {
        return false;
    }
    return memcmp(a.ptr, b.ptr, a.len) == 0;
}
