#include "value.h"

#include "mem.h"

void clox_vm_value_array_init(struct clox_vm_value_array* va) {
    va->values = NULL;
    va->capacity = va->count = 0;
}

void clox_vm_value_array_write(struct clox_vm_value_array* va, clox_vm_value value) {
    if (va->capacity < va->count + 1) {
        size_t old_capacity = va->capacity;
        va->capacity = CLOX_VM_MEM_GROW_CAPACITY(old_capacity);
        va->values = CLOX_VM_MEM_GROW_ARRAY(clox_vm_value, va->values, old_capacity, va->capacity);
    }
    va->values[va->count++] = value;
}

void clox_vm_value_array_free(struct clox_vm_value_array* va) {
    CLOX_VM_MEM_FREE_ARRAY(clox_vm_value, va->values, va->capacity);
    clox_vm_value_array_init(va);
}

void clox_vm_value_print(clox_vm_value value) {
    printf("%g", value);
}
