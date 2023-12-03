// vim: filetype=c
#pragma once

#include <unistd.h>

struct string;

enum ReadReturnValue {
    Success,
    EndOfFile,
    Error,
};

int open_first_arg(int argc, char **argv);

enum ReadReturnValue read_line(int fd, struct string *str);
