#ifndef CLOX_COMMONS_H
#define CLOX_COMMONS_H

struct str;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int file_read_contents(const char* file_path, struct str* contents);

#endif
