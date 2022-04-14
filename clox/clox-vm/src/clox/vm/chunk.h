#ifndef CLOX_VM_CHUNK_H
#define CLOX_VM_CHUNK_H

#include "common.h"
#include "value.h"

enum clox_vm_op_code {
    CLOX_VM_OP_CODE_RETURN,
    CLOX_VM_OP_CODE_CONSTANT,
};

struct clox_vm_chunk {
    /**
     * @brief A dynamic array of multiple codes
     */
    uint8_t* codes;
    /**
     * @brief the number of elements in the array we have allocated
     */
    size_t codes_capacity;
    /**
     * @brief how many of those allocated entries are actually in use
     */
    size_t codes_count;
    /**
     * @brief Dynamic array of constant values
     */
    struct clox_vm_value_array constants;
    /**
     * @brief Dynamic array of source code line numbers.
     * 
     * The index in this array is aligned with the index in the codes array.
     * This is way we don't need another capacity/count variables.
     * 
     * In order to get the op code source code line, use its index here.
     */
    size_t* lines;
};

void clox_vm_chunk_init(struct clox_vm_chunk* chunk);
void clox_vm_chunk_free(struct clox_vm_chunk* chunk);
void clox_vm_chunk_write(struct clox_vm_chunk* chunk, uint8_t byte, size_t line);

/**
 * @return the index where the constant was appended 
 */
size_t clox_vm_chunk_add_constant(struct clox_vm_chunk* chunk, clox_vm_value value);

#endif
