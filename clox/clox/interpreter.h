#ifndef CLOX_INTERPRETER_H
#define CLOX_INTERPRETER_H

#include "value.h"

struct expr;

struct clox_interpreter {
    // evaluation resulting value
    struct clox_value value;
};

struct clox_value clox_interpreter_eval(struct clox_interpreter* interpreter, struct expr* expr);

#endif
