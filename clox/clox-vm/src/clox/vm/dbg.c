#include "dbg.h"

#include "chunk.h"

static size_t simple_instruction(const char* name, size_t offset);

void clox_vm_dbg_chunk_disassemble(struct clox_vm_chunk* chunk, const char* name) {
    fprintf(stderr, "== %s ==\n", name);

    for (size_t offset = 0; offset < chunk->codes_count; ) {
        offset = clox_vm_dbg_chunk_disassemble_instruction(chunk, offset);
    }
}

int clox_vm_dbg_chunk_disassemble_instruction(struct clox_vm_chunk* chunk, size_t offset) {
    fprintf(stderr, "%04zu ", offset);

    uint8_t instruction = chunk->codes[offset];
    switch (instruction)
    {
    case CLOX_VM_OP_CODE_RETURN:
        return simple_instruction("OP_RETURN", offset);

    default:
        fprintf(stderr, "Unknown opcode %hhu\n", instruction);
        return offset + 1;
    }
}

static size_t simple_instruction(const char* name, size_t offset) {
    fprintf(stderr, "%s\n", name);
    return offset + 1;
}
