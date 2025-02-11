#include "../include/shell.h"

/*

Whenever we receive a character, we want to output it back to the user.
We also want to parse commands.
We want to implement echo built in.

implement the echo built-in: echo $a $b $c should print $a $b $c
accept both \r and \n as “enter”, marking the end of a line
accept both backspace and delete (ASCII 8 and 127) to erase a single character
ring the bell (ASCII 7) if an unrecognized non-visible character is sent to it
print unknown command: $command for an unknown command $command
disallow backspacing through the prefix
disallow typing more characters than allowed
accept commands at most 512 bytes in length
accept at most 64 arguments per command
start a new line, without error, with the prefix if the user enters an empty command
print error: too many arguments if the user passes in too many arguments

 */

void write_string(struct shell* shell, const char * string) {
    int i = 0;
    while(string[i] != '\0') {
        shell->output(string[i++]);
    }
}

void run_shell(struct shell* shell) {
    write_string(shell, "Welcome to Raspberry Pi Kernel Shell!\r\n");
    write_string(shell, SHELL_PREFIX);
}