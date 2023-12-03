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

        append(&list, line);
        if (rv == Success) {
            continue;
        } else if (rv == EndOfFile) {
            break;
        }
    }

    for (struct list *n = list; n != nullptr; n = n->next) {
        printf("%s", ((struct string *)n->data)->data);
    }

    free_list(&list, free_list_data);

    return 0;
}
