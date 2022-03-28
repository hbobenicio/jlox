#include "common.h"
#include "chunk.h"
#include "dbg.h"

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;
    
    struct clox_vm_chunk chunk;
    clox_vm_chunk_init(&chunk);

    clox_vm_chunk_write(&chunk, CLOX_VM_OP_CODE_RETURN);
    clox_vm_dbg_chunk_disassemble(&chunk, "test chunk");

    clox_vm_chunk_free(&chunk);
    return 0;
}
