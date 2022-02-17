#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STB_DS_IMPLEMENTATION
#include "clox/src/clox/stb_ds.h"

struct pair {
    int x, y;
};

struct my_kv {
    // char* key;
    size_t key;
    struct pair value;
};

char* cstr_new(const char* s) {
    char* buf = calloc(strlen(s) + 1, sizeof(char));
    return strcpy(buf, s);
}

int main() {
    stbds_rand_seed(time(NULL));

    char* hugo = cstr_new("hugo");
    char* fulano = cstr_new("fulano");
    char* fulano2 = cstr_new("fulano");
    char* cicrano = cstr_new("cicrano");

    struct my_kv* table = NULL;
    // hmput(table, "hugo", ((struct pair){.x = 10, .y = 20}));
    // hmput(table, "fulano", ((struct pair){.x = 20, .y = 30}));
    // hmput(table, "cicrano", ((struct pair){.x = 10, .y = 20}));
    hmput(table, stbds_hash_string(hugo, time(NULL)), ((struct pair){.x = 10, .y = 20}));
    hmput(table, stbds_hash_string(fulano, time(NULL)), ((struct pair){.x = 20, .y = 30}));
    hmput(table, stbds_hash_string(cicrano, time(NULL)), ((struct pair){.x = 10, .y = 20}));

    printf("(%d)\n", hmget(table, stbds_hash_string(fulano2, time(NULL))).x);
    printf("(%d)\n", hmget(table, stbds_hash_string(hugo, time(NULL))).x);
    printf("(%d)\n", hmget(table, stbds_hash_string("fulano", time(NULL))).x);

    hmfree(table);
    free(cicrano);
    free(fulano2);
    free(fulano);
    free(hugo);
}
