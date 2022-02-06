#include "program.h"

#include <stdlib.h>
#include <clox/stb_ds.h>
#include <clox/commons.h>
#include "statement.h"

struct clox_ast_program* clox_ast_program_new(void) {
    struct clox_ast_program* prog = malloc(sizeof(struct clox_ast_program));
    CLOX_ERR_PANIC_OOM_IF_NULL(prog);

    prog->statements = NULL;

    return prog;
}

void clox_ast_program_free(struct clox_ast_program* prog) {
    for (long i = 0; i < arrlen(prog->statements); i++) {
        clox_ast_statement_free(prog->statements[i]);
    }
    arrfree(prog->statements);
    prog->statements = NULL;

    free(prog);
}

void clox_ast_program_add_statement(struct clox_ast_program* prog, struct clox_ast_statement* stmt) {
    arrpush(prog->statements, stmt);
}
