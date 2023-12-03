#include "str.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string *init_string(ssize_t initial_len)
{
    errno = 0;
    struct string *str = (struct string *)malloc(sizeof(struct string));
    if (str == nullptr) {
        perror(__func__);
        return nullptr;
    }

    errno = 0;
    memset(str, 0, sizeof(struct string));
    if (errno != 0) {
        perror(__func__);
        free(str);
        return nullptr;
    }

    if (initial_len > 0) {
        if (!reallocate(str, initial_len)) {
            free(str);
            return nullptr;
        }
    }

    return str;
}

void free_string(struct string *v)
{
    free(v->data);
    v->data = nullptr;
    v->len = 0;
    v->buffer_len = 0;
}

bool reallocate(struct string *v, ssize_t new_len)
{
    if (new_len < 0) {
        printf("%s: negative size\n", __func__);
        return false;
    }

    // Allocate a buffer 1.5 times bigger than requested.
    // TODO: Maybe also round the value to the next alignment value.
    ssize_t buf_len = (new_len * 3) / 2;
    errno = 0;
    char *new_buf = (char *)realloc(v->data, buf_len);
    if (new_buf == nullptr) {
        perror(__func__);
        return false;
    }

    if (v->data == nullptr) {
        v->data = new_buf;
    }

    ssize_t old_len = v->buffer_len;
    v->buffer_len = buf_len;

    errno = 0;
    memset(v->data + old_len, 0, v->buffer_len - old_len);
    if (errno != 0) {
        perror(__func__);
        return false;
    }

    return true;
}

bool append_char(struct string *v, const char *data, ssize_t data_len)
{
    if (data_len < 0) {
        printf("%s: negative data length\n", __func__);
        return false;
    }

    // -1 because we want to keep only one terminating null byte.
    if (v->len + data_len - 1 > v->buffer_len) {
        if (!reallocate(v, v->len + data_len - 1)) {
            return false;
        }
    }

    errno = 0;
    strcpy(v->data + v->len, data);
    if (errno != 0) {
        perror(__func__);
        return false;
    }
    v->len = v->len + data_len - 1;

    return true;
}

bool append_string(struct string *v, struct string *new_v)
{
    return append_char(v, new_v->data, new_v->len);
}
