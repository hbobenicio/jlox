#include "env.h"

#include <stdio.h>
#include <time.h>
#include "stb_ds.h"

static clox_env_key hash_strview(const struct clox_env* env, struct strview sv) {
    return stbds_hash_bytes(sv.ptr, sv.len, env->seed);
}

void clox_env_init(struct clox_env* env) {
    env->table = NULL;
    env->seed = time(NULL);

    // Sets the default value for the hashmap, the value which
    // will be returned by hmget/shget if the key is not present.
    shdefault(env->table, clox_value_nil());
}

void clox_env_free(struct clox_env* env) {
    for (long int i = 0; i < hmlen(env->table); i++) {
        clox_value_free(&env->table[i].value);
    }
    hmfree(env->table);
    env->table = NULL;
    env->seed = 0;
}

void clox_env_define(struct clox_env* env, struct strview var_name, struct clox_value var_value) {
    hmput(env->table, hash_strview(env, var_name), var_value);
}

int clox_env_get(struct clox_env* env, struct strview var_name, struct clox_value* out_var_value) {
    struct clox_env_kv* entry = hmgetp_null(env->table, hash_strview(env, var_name));
    if (entry == NULL) {
        fputs("error: env: undefined variable '", stderr);
        strview_fprint(var_name, stderr);
        fputs("'\n", stderr);
        *out_var_value = clox_value_nil();
        return 1;
    }

    *out_var_value = entry->value;
    return 0;
}
