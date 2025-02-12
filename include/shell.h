#include "../include/io.h"

#ifndef _SHELL_H
#define _SHELL_H

#define SHELL_PREFIX "> "
#define COMMAND_MAX_LEN 512
#define COMMAND_MAX_ARGUMENTS 64

#define BELL 7
#define BACKSPACE 8
#define DELETE 127
#define CARRIAGE_DOWN '\r'
#define NEWLINE '\n' 
#define DELETE_CHAR_SEQ "\b \b"

#define ECHO "echo"

struct shell {
    receiver_func_type input;
    sender_func_type output;
};

void run_shell(struct shell* shell);

#endif