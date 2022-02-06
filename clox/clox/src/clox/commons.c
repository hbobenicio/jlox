#include "commons.h"

#include <stdio.h>

// POSIX
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "str.h"

int file_read_contents(const char* file_path, struct str* contents) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "error: failed to open file %s for reading\n", file_path);
        return 1;
    }

    struct stat file_stats;
    if (fstat(fd, &file_stats) == -1) {
        fprintf(stderr, "error: failed to get file size from \"%s\"\n", file_path);
        close(fd);
        return 1;
    }
    contents->len = file_stats.st_size;
    contents->cap = contents->len + 1; // '\0'

    contents->ptr = mmap(NULL, contents->cap, PROT_READ, MAP_PRIVATE, fd, 0);
    if (contents->ptr == MAP_FAILED) {
        fprintf(stderr, "error: failed to read file contents from \"%s\"\n", file_path);
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
