#include "value.h"

#include <assert.h>
#include <math.h>

struct clox_value clox_value_bool(bool val) {
    return (struct clox_value) {
        .kind = CLOX_VALUE_KIND_BOOL,
        .as.boolean = val,
    };
}

struct clox_value clox_value_nil(void) {
    return (struct clox_value) {
        .kind = CLOX_VALUE_KIND_NIL,
        .as = {0}
    };
}

struct clox_value clox_value_number(double val) {
    return (struct clox_value) {
        .kind = CLOX_VALUE_KIND_NUMBER,
        .as.number = val,
    };
}

struct clox_value clox_value_string_str_dup(struct str val) {
    return (struct clox_value) {
        .kind = CLOX_VALUE_KIND_STRING,
        .as.string = str_dup(val),
    };
}

struct clox_value clox_value_string_str_borrow(struct str val) {
    return (struct clox_value) {
        .kind = CLOX_VALUE_KIND_STRING,
        .as.string = val,
    };
}

void clox_value_free(struct clox_value* val) {
    if (val->kind == CLOX_VALUE_KIND_STRING) {
        str_free(&val->as.string);
    }
    *val = clox_value_nil();
}

void clox_value_fprintln(FILE* file, struct clox_value value) {
    switch (value.kind) {
    case CLOX_VALUE_KIND_BOOL:
        fprintf(file, "%s\n", (value.as.boolean) ? "true" : "false");
        break;

    case CLOX_VALUE_KIND_NIL:
        fputs("nil\n", file);
        break;

    case CLOX_VALUE_KIND_NUMBER:
        fprintf(file, "%lf\n", value.as.number);
        break;

    case CLOX_VALUE_KIND_STRING:
        //FIXME this is spefic to the repl mode. this doesn't make sense for scripting mode
        fprintf(file, "\"%s\"\n", value.as.string.ptr);
        break;
    }
}

bool clox_value_is_truthy(struct clox_value value) {
    switch (value.kind) {
    case CLOX_VALUE_KIND_NIL:
        return false;
        
    case CLOX_VALUE_KIND_BOOL:
        return value.as.boolean;

    default:
        return true;
    }
}

bool clox_value_is_equal(struct clox_value left, struct clox_value right) {
    if (left.kind != right.kind) {
        return false;
    }

    switch (left.kind) {
    case CLOX_VALUE_KIND_BOOL:
        return left.as.boolean == right.as.boolean;
        
    case CLOX_VALUE_KIND_NIL:
        return true;

    case CLOX_VALUE_KIND_NUMBER: {
        static const double epsilon = 0.00000001;
        return fabs(left.as.number - right.as.number) <= epsilon;
    }

    case CLOX_VALUE_KIND_STRING:
        return str_equals(left.as.string, right.as.string);
    }

    return false;
}
