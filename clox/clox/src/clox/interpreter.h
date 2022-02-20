#ifndef CLOX_INTERPRETER_H
#define CLOX_INTERPRETER_H

#include "value.h"
#include "env.h"

struct clox_ast_expr;
struct clox_ast_statement;
struct clox_ast_program;

/**
 * @brief The AST Interpreter.
 * 
 * It traverses recursively (through visitor implementations) the AST of Clox programs parsed with
 * clox_parser, executing all statements in order, evaluating expressions and saving its values in the environment state as needed.
 */
struct clox_interpreter {
    /**
     * @brief evaluation resulting value
     */
    struct clox_value value;
    
    /**
     * @brief Environment state where global variables are stored
     * 
     */
    struct clox_env env;
};

/**
 * @brief Initializes the Interpreter. Its initial evaluation value is set to nil
 * 
 * @param interpreter 
 */
void clox_interpreter_init(struct clox_interpreter* interpreter);

/**
 * @brief Cleans up the Interpreter after use.
 * 
 * @param interpreter 
 */
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

/**
 * @brief Sets a new value in the interpreter state.
 * 
 * If the previous value is a string, then free's it (because the old value own's the str buffer which were heap allocated).
 * 
 * @param interpreter 
 * @param val The new value to be set
 */
void clox_interpreter_set_value(struct clox_interpreter* interpreter, struct clox_value val);

#endif
