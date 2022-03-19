/// This is closely tighted to the interpreter. Consider this fact if refactoring the interpreter to its own folder
#ifndef CLOX_ENV_H
#define CLOX_ENV_H

#include <stddef.h>

#include "strview.h"
#include "value.h"

typedef size_t clox_env_key;

struct clox_env_kv {
    clox_env_key key;
    struct clox_value value;
};

struct clox_env {
    /**
     * @brief The stb_ds hash table to store the items
     */
    struct clox_env_kv* table;

    /**
     * @brief Random number
     */
    size_t seed;
};

void clox_env_init(struct clox_env* env);
void clox_env_free(struct clox_env* env);
void clox_env_define(struct clox_env* env, struct strview var_name, struct clox_value var_value);
int clox_env_get(struct clox_env* env, struct strview var_name, struct clox_value* out_var_value);
int clox_env_assign(struct clox_env* env, struct strview var_name, struct clox_value var_value);

#endif
