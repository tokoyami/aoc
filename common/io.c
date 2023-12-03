#include "io.h"

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include "str.h"

int open_first_arg(int argc, char **argv)
{
    if (argc < 2) {
        printf("Pass input file\n");
        return -1;
    }

    errno = 0;
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    return fd;
}

enum ReadReturnValue read_line(int fd, struct string *str)
{
    bool stop = true;
    do {
        if (str->len + 1 > str->buffer_len) {
            if (!reallocate(str, str->buffer_len + 1)) {
                printf("%s: failed to allocate enough space to read line\n", __func__);
                return Error;
            }
        }

        errno = 0;
        ssize_t read_bytes = read(fd, str->data + str->len, 1);
        if (read_bytes < 0) {
            perror(__func__);
            return Error;
        } else if (read_bytes == 0) {
            return EndOfFile;
        }
        str->len += read_bytes;

        if (str->len > 0) {
            stop = str->data[str->len - 1] == '\n';
        } else {
            stop = false;
        }
    } while(!stop);

    return Success;
}
