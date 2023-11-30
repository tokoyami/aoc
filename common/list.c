#include "list.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

// Cache for the last value to make appending easier.
struct value *last_value = nullptr;

bool append(struct value **root, void *data)
{
    errno = 0;
    struct value *new_value = (struct value *)malloc(sizeof(struct value));
    if (new_value == nullptr) {
        perror("malloc");
        return false;
    }

    new_value->data = data;
    new_value->next = nullptr;

    if (*root == nullptr) {
        *root = new_value;
    } else {
        last_value->next = new_value;
    }
    last_value = new_value;

    return true;
}

void free_list(struct value **root, void (*data_dealloc)(void *))
{
    struct value *r = *root;
    do {
        struct value *next = r->next;
        if (data_dealloc) {
            data_dealloc(r->data);
        }
        free(r);
        r = next;
    } while (r != nullptr);
    *root = nullptr;
    last_value = nullptr;
}
