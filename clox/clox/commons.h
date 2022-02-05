#ifndef CLOX_COMMONS_H
#define CLOX_COMMONS_H

struct str;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define CLOX_ERR_OOM_EPRINTLN() \
    fprintf(stderr, "error: %s:%d: out of memory", __FILE__, __LINE__)

int file_read_contents(const char* file_path, struct str* contents);

#endif
