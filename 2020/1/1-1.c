#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <common/list.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Pass input file\n");
        return 1;
    }

    errno = 0;
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct list *value_list = nullptr;
    char buf = 0;
    // Number string will never be above 4 characters.
    int nbuf_index = 0;
    char nbuf[5] = {0};
    ssize_t bytes = 0;
    errno = 0;
    while ((bytes = read(fd, &buf, 1)) != 0) {
        switch (bytes) {
            case -1:    perror("read");
                        return 1;
            case 1:
                switch (buf) {
                    case '\n': {
                        char *endptr = "\0";
                        errno = 0;
                        int num = (int)strtol(nbuf, &endptr, 10);
                        if (num == 0 && endptr == nbuf) {
                            printf("read empty line\n");
                        } else if (nbuf[0] != '\0' && endptr[0] == '\0') {
                            append(&value_list, (void *)(uintptr_t)num);
                        } else {
                            perror("strtol");
                        }
                        if (memset(nbuf, 0, sizeof nbuf) == nullptr) {
                            printf("memset error\n");
                        }
                        nbuf_index = 0;
                        break;
                    }
                    default: {
                        if (isdigit(buf) != 0) {
                            nbuf[nbuf_index] = buf;
                            ++nbuf_index;
                        } else {
                            printf("%c: not a digit\n", buf);
                            return 1;
                        }
                        break;
                    }
                }
                break;
        }

        errno = 0;
    }

    if (value_list == nullptr) {
        printf("no numbers in input\n");
        return 1;
    }

    int done = 0;
    for (struct list *start_left = value_list;
         start_left != nullptr && done == 0;
         start_left = start_left->next) {
        for (struct list *start_right = start_left->next;
             start_right != nullptr;
             start_right = start_right->next) {
            if ((int)(uintptr_t)start_left->data + (int)(uintptr_t)start_right->data == 2020) {
                printf("answer: %d\n",
                       (int)(uintptr_t)start_left->data * (int)(uintptr_t)start_right->data);
                done = 1;
                break;
            }
        }
    }

    free_list(&value_list, nullptr);

    return 0;
}
