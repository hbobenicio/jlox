#include "chunk.h"
#include "mem.h"

void clox_vm_chunk_init(struct clox_vm_chunk* chunk) {
    chunk->codes = NULL;
    chunk->codes_capacity = chunk->codes_count = 0;
    clox_vm_value_array_init(&chunk->constants);
}

void clox_vm_chunk_free(struct clox_vm_chunk* chunk) {
    CLOX_VM_MEM_FREE_ARRAY(uint8_t, chunk->codes, chunk->codes_capacity);
    clox_vm_value_array_free(&chunk->constants);
    clox_vm_chunk_init(chunk);
}

void clox_vm_chunk_write(struct clox_vm_chunk* chunk, uint8_t byte) {
    if (chunk->codes_count + 1 > chunk->codes_capacity) {
        size_t old_capacity = chunk->codes_capacity;
        chunk->codes_capacity = CLOX_VM_MEM_GROW_CAPACITY(old_capacity);
        chunk->codes = CLOX_VM_MEM_GROW_ARRAY(uint8_t, chunk->codes, old_capacity, chunk->codes_capacity);
    }
    chunk->codes[chunk->codes_count] = byte;
    chunk->codes_count++;
}

size_t clox_vm_chunk_add_constant(struct clox_vm_chunk* chunk, clox_vm_value value) {
    clox_vm_value_array_write(&chunk->constants, value);
    return chunk->constants.count - 1;
}
