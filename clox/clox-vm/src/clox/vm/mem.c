#include "mem.h"

void* clox_vm_mem_reallocate(void* ptr, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }
    void* result = realloc(ptr, new_size);
    if (result == NULL) {
        exit(EXIT_FAILURE);
    }

    return result;
}
