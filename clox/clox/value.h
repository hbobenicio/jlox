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

/**
 * @brief Creates a new clox string value. The underlying str is duplicated, so it should be freed
 * 
 * @param val 
 * @return struct clox_value 
 */
struct clox_value clox_value_string_str_dup(struct str val);

/**
 * @brief Creates a new clox string value. The underlying str is borrowed. os it must not outlive its reference
 * 
 * @param val 
 * @return struct clox_value 
 */
struct clox_value clox_value_string_str_borrow(struct str val);

void clox_value_free(struct clox_value* val);

void clox_value_fprintln(FILE* file, struct clox_value value);

#endif
