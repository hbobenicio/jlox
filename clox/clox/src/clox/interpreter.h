#ifndef CLOX_INTERPRETER_H
#define CLOX_INTERPRETER_H

#include "value.h"

struct clox_ast_expr;
struct clox_ast_statement;
struct clox_ast_program;

struct clox_interpreter {
    /**
     * @brief evaluation resulting value
     */
    struct clox_value value;
};

/**
 * @brief Initializes the Interpreter. Its initial evaluation value is set to nil
 * 
 * @param interpreter 
 */
void clox_interpreter_init(struct clox_interpreter* interpreter);

void clox_interpreter_free(struct clox_interpreter* interpreter);

/**
 * @brief Evaluates the given AST expression. The value is a borrow which is owned by the interpreter.
 * 
 * @param interpreter 
 * @param expr The root expression where evaluation starts
 * @return struct clox_value 
 */
struct clox_value clox_interpreter_eval(struct clox_interpreter* interpreter, struct clox_ast_expr* expr);

/**
 * @brief Executes the given AST statement.
 * 
 * @param interpreter 
 * @param stmt 
 */
int clox_interpreter_exec_statement(struct clox_interpreter* interpreter, struct clox_ast_statement* stmt);

/**
 * @brief Executes the given AST program.
 * 
 * @param interpreter 
 * @param prog 
 * @return int 
 */
int clox_interpreter_exec_program(struct clox_interpreter* interpreter, struct clox_ast_program* prog);

#endif
