#ifndef CLOX_INTERPRETER_H
#define CLOX_INTERPRETER_H

#include "value.h"

struct expr;

struct clox_interpreter {
    // evaluation resulting value
    struct clox_value value;
};

/**
 * @brief Initializes the Interpreter. It's initial evaluation value is set to nil
 * 
 * @param interpreter 
 */
void clox_interpreter_init(struct clox_interpreter* interpreter);

void clox_interpreter_free(struct clox_interpreter* interpreter);

/**
 * @brief Evaluates the given expression. The value is a borrow which is owned by the interpreter.
 * 
 * @param interpreter 
 * @param expr 
 * @return struct clox_value 
 */
struct clox_value clox_interpreter_eval(struct clox_interpreter* interpreter, struct expr* expr);

#endif
