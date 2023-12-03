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
bool reallocate(struct string *v, ssize_t new_len);
bool append_char(struct string *v, const char *data, ssize_t data_len);
bool append_string(struct string *v, struct string *new_v);
