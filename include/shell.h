#include "../include/io.h"

#ifndef _SHELL_H
#define _SHELL_H

#define SHELL_PREFIX "> "

struct shell {
    receiver_func_type input;
    sender_func_type output;
};

void run_shell(struct shell* shell);

#endif