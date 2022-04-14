#include "dbg.h"

#include "chunk.h"

static size_t simple_instruction(const char* name, size_t offset);
static size_t constant_instruction(const char* name, struct clox_vm_chunk* chunk, size_t offset);

void clox_vm_dbg_chunk_disassemble(struct clox_vm_chunk* chunk, const char* name) {
    fprintf(stderr, "== %s ==\n", name);

    for (size_t offset = 0; offset < chunk->codes_count; ) {
        offset = clox_vm_dbg_chunk_disassemble_instruction(chunk, offset);
    }
}

size_t clox_vm_dbg_chunk_disassemble_instruction(struct clox_vm_chunk* chunk, size_t offset) {
    fprintf(stderr, "%04zu ", offset);

    uint8_t instruction = chunk->codes[offset];
    switch (instruction)
    {
    case CLOX_VM_OP_CODE_RETURN:
        return simple_instruction("OP_RETURN", offset);

    case CLOX_VM_OP_CODE_CONSTANT:
        return constant_instruction("OP_CONSTANT", chunk, offset);

    default:
        fprintf(stderr, "Unknown opcode %hhu\n", instruction);
        return offset + 1;
    }
}

static size_t simple_instruction(const char* name, size_t offset) {
    fprintf(stderr, "%s\n", name);
    return offset + 1;
}

static size_t constant_instruction(const char* name, struct clox_vm_chunk* chunk, size_t offset) {
    uint8_t constant = chunk->codes[offset + 1];
    printf("%-16s %4u '", name, constant);
    clox_vm_value_print(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}
