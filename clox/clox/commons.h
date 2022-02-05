#ifndef CLOX_COMMONS_H
#define CLOX_COMMONS_H

#include <stdio.h>
#include <stdlib.h>

// Posix (I think...)
#include <unistd.h>

// Linux (Posix?)
#include <execinfo.h>

struct str;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

// https://stackoverflow.com/questions/77005/how-to-automatically-generate-a-stacktrace-when-my-program-crashes
// https://www.gnu.org/software/libc/manual/html_node/Backtraces.html
// How to show line numbers? addr2line?
#define CLOX_ERR_PANIC_OOM_IF_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "error: %s:%d: out of memory!\nerror: backtrace:\n", __FILE__, __LINE__); \
            void *array[10] = {0}; \
            size_t size = backtrace(array, 10); \
            backtrace_symbols_fd(array, size, STDERR_FILENO); \
            exit(1); \
        } \
    } while(0)

int file_read_contents(const char* file_path, struct str* contents);

#endif
