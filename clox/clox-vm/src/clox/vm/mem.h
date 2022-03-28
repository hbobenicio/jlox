#ifndef CLOX_VM_MEMORY_H
#define CLOX_VM_MEMORY_H

#include "common.h"

#define CLOX_VM_MEM_GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define CLOX_VM_MEM_GROW_ARRAY(type, ptr, old_count, new_count) \
    clox_vm_mem_reallocate(ptr, sizeof(type) * (old_count), sizeof(type) * (new_count))

#define CLOX_VM_MEM_FREE_ARRAY(type, ptr, old_count) \
    clox_vm_mem_reallocate(ptr, sizeof(type) * (old_count), 0)

/**
 * @brief the single function we’ll use for all dynamic memory management in clox—allocating memory,
 * freeing it, and changing the size of an existing allocation.
 * 
 * The two size arguments passed to clox_vm_mem_reallocate() control which operation to perform:
 * 
 * oldSize  newSize                     Operation
 * 0        Non‑zero                    Allocate new block.
 * Non‑zero 0                           Free allocation.
 * Non‑zero Smaller than oldSize        Shrink existing allocation.
 * Non‑zero Larger than oldSize         Grow existing allocation.
 * 
 * @param ptr 
 * @param old_size 
 * @param new_size 
 * @return void* 
 */
void* clox_vm_mem_reallocate(void* ptr, size_t old_size, size_t new_size);

#endif
