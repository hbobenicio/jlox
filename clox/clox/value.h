#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include <stdio.h>
#include <stdbool.h>
#include "str.h"

enum clox_value_kind {
    CLOX_VALUE_KIND_BOOL,
    CLOX_VALUE_KIND_NIL,
    CLOX_VALUE_KIND_NUMBER,
    CLOX_VALUE_KIND_STRING,
};

struct clox_value {
    enum clox_value_kind kind;
    union {
        bool boolean;
        double number;
        struct str string;
    } as;
};

struct clox_value clox_value_bool(bool val);
struct clox_value clox_value_nil(void);
struct clox_value clox_value_number(double val);
struct clox_value clox_value_string(struct str val);

void clox_value_fprintln(FILE* file, struct clox_value value);

#endif
