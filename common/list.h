// vim: filetype=c
#ifndef AOC_COMMON_LIST_H
#define AOC_COMMON_LIST_H

struct value {
    void *data;
    struct value *next;
};

int append(struct value **root, void *data);
void free_list(struct value **root, void (*data_dealloc)(void *));

#endif // AOC_COMMON_LIST_H
