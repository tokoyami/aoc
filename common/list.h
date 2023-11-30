// vim: filetype=c
#ifndef AOC_COMMON_LIST_H
#define AOC_COMMON_LIST_H

#include <stdbool.h>

struct value {
    void *data;
    struct value *next;
};

bool append(struct value **root, void *data);
void free_list(struct value **root, void (*data_dealloc)(void *));

#endif // AOC_COMMON_LIST_H
