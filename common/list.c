#include "list.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defines.h"

// Cache for the last value to make appending easier.
struct value *last_value = NULL;

int append(struct value **root, void *data)
{
    errno = 0;
    struct value *new_value = (struct value *)malloc(sizeof(struct value));
    if (new_value == NULL) {
        perror("malloc");
        return FALSE;
    }

    new_value->data = data;
    new_value->next = NULL;

    if (*root == NULL) {
        *root = new_value;
    } else {
        last_value->next = new_value;
    }
    last_value = new_value;

    return TRUE;
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
    } while (r != NULL);
    *root = NULL;
    last_value = NULL;
}
