// vim: filetype=c
#pragma once

#include <stdbool.h>

struct value {
    void *data;
    struct value *next;
};

bool append(struct value **root, void *data);
void free_list(struct value **root, void (*data_dealloc)(void *));
