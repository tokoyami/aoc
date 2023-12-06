#include "str.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string *init_string(ssize_t initial_len)
{
    errno = 0;
    struct string *const str = (struct string *)malloc(sizeof(struct string));
    if (str == nullptr) {
        perror(__func__);
        return nullptr;
    }

    str->len = 0;
    str->buffer_len = 0;
    str->data = nullptr;

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

bool is_space_enough_to_append(struct string *v, ssize_t extra_size)
{
    // Make sure there is at least one zero byte for string termination.
    return v->len + 1 + extra_size <= v->buffer_len;
}

bool reallocate(struct string *v, ssize_t new_len)
{
    if (new_len < 0) {
        printf("%s: negative size\n", __func__);
        return false;
    }

    // Allocate a buffer 1.5 times bigger than requested.
    // TODO: Maybe also round the value to the next alignment value.
    const ssize_t buf_len = (new_len * 3) / 2;
    errno = 0;
    char *const new_buf = (char *)realloc(v->data, buf_len);
    if (new_buf == nullptr) {
        perror(__func__);
        return false;
    }

    if (v->data == nullptr) {
        v->data = new_buf;
    }

    const ssize_t old_len = v->buffer_len;
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

    const ssize_t new_len = v->len + data_len;
    if (!is_space_enough_to_append(v, new_len)) {
        if (!reallocate(v, new_len)) {
            return false;
        }
    }

    errno = 0;
    strcpy(v->data + v->len, data);
    if (errno != 0) {
        perror(__func__);
        return false;
    }
    v->len = new_len;

    return true;
}

bool append_string(struct string *v, struct string *new_v)
{
    return append_char(v, new_v->data, new_v->len);
}
