// vim: filetype=c
#pragma once

struct list {
    void *data;
    struct list *next;
};

bool append(struct list **root, void *data);
void free_list(struct list **root, void (*data_dealloc)(void *));
