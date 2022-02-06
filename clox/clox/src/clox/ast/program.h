#ifndef CLOX_AST_PROGRAM_H
#define CLOX_AST_PROGRAM_H

struct clox_ast_statement;

struct clox_ast_program {
    /**
     * @brief Dynamic array of statements
     */
    struct clox_ast_statement** statements;
};

struct clox_ast_program* clox_ast_program_new(void);
void clox_ast_program_free(struct clox_ast_program* prog);
void clox_ast_program_add_statement(struct clox_ast_program* prog, struct clox_ast_statement* stmt);

#endif
