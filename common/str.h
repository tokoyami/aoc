// vim: filetype=c
#pragma once

#include <unistd.h>

struct string {
    ssize_t len;
    ssize_t buffer_len;
    char *data;
};

struct string *init_string(ssize_t initial_len);
void free_string(struct string *v);
bool is_space_enough_to_append(struct string *v, ssize_t extra_size);
bool reallocate(struct string *v, ssize_t new_len);
bool append_char(struct string *v, const char *data, ssize_t data_len);
bool append_string(struct string *v, struct string *new_v);
