#ifndef CLOX_VM_VALUE_H
#define CLOX_VM_VALUE_H

#include "common.h"

typedef double clox_vm_value;

struct clox_vm_value_array {
    clox_vm_value* values;
    size_t capacity;
    size_t count;
};

void clox_vm_value_array_init(struct clox_vm_value_array* va);
void clox_vm_value_array_write(struct clox_vm_value_array* va, clox_vm_value value);
void clox_vm_value_array_free(struct clox_vm_value_array* va);
void clox_vm_value_print(clox_vm_value value);

#endif
