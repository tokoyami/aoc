#include "list.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

// Cache for the last value to make appending easier.
struct list *last_value = nullptr;

bool append(struct list **root, void *data)
{
    errno = 0;
    struct list *new_value = (struct list *)malloc(sizeof(struct list));
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

void free_list(struct list **root, void (*data_dealloc)(void *))
{
    struct list *r = *root;
    do {
        struct list *next = r->next;
        if (data_dealloc) {
            data_dealloc(r->data);
        }
        free(r);
        r = next;
    } while (r != nullptr);
    *root = nullptr;
    last_value = nullptr;
}
