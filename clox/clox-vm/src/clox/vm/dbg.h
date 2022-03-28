#ifndef CLOX_VM_DBG_H
#define CLOX_VM_DBG_H

#include "common.h"

struct clox_vm_chunk;

void clox_vm_dbg_chunk_disassemble(struct clox_vm_chunk* chunk, const char* name);
int  clox_vm_dbg_chunk_disassemble_instruction(struct clox_vm_chunk* chunk, size_t offset);

#endif
