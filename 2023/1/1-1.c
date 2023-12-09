#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <common/io.h>
#include <common/str.h>
#include <common/list.h>

void free_list_data(void *data)
{
    free_string(data);
}

int main(int argc, char **argv)
{
    int fd = open_first_arg(argc, argv);
    if (fd < 0) {
        return fd;
    }

    struct list *list = nullptr;
    while (true) {
        struct string *line = init_string(16);
        enum ReadReturnValue rv = read_line(fd, line);
        if (rv == Error) {
            free_string(line);
            free(line);
            free_list(&list, free_list_data);
            return 1;
        }

        if (!append(&list, line)) {
            free_string(line);
            free(line);
            free_list(&list, free_list_data);
            return 1;
        }

        if (rv == Success) {
            continue;
        } else if (rv == EndOfFile) {
            break;
        }
    }

    unsigned long sum = 0;
    for (struct list *n = list; n != nullptr; n = n->next) {
        char number[2] = {0, 0};
        struct string *start = n->data;
        int s = 0;
        for (; s <= start->len; ++s) {
            if (isdigit(start->data[s])) {
                number[0] = start->data[s];
                break;
            }
        }
        if (s == start->len && number[0] == 0) {
            // No numbers on this line.
            continue;
        }
        int e = start->len;
        for (; e >= 0; --e) {
            if (isdigit(start->data[e])) {
                number[1] = start->data[e];
                break;
            }
        }
        sum += atoi(number);
    }

    printf("sum = %lu\n", sum);

    free_list(&list, free_list_data);

    return 0;
}
