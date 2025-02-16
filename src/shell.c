#include "../include/shell.h"
#include "../include/strings.h"
#include "../include/timer.h"

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

void echo(struct shell* shell, char* arg) {
    write_string(shell, arg);
    write_string(shell, "\r\n");
}

void run_shell(struct shell* shell) {
    write_string(shell, "Welcome to Raspberry Pi Kernel Shell!\r\n");

    while(1) {
        write_string(shell, SHELL_PREFIX);

        char command[COMMAND_MAX_LEN];
        int current_char_index = 0;
        int arg_count = 0;
        char is_argument_start = 0;

        while (current_char_index < COMMAND_MAX_LEN) {
            char c = shell->input();

            if(arg_count == COMMAND_MAX_ARGUMENTS) {
                if(c != BACKSPACE && c != DELETE) {
                    continue;
                }
            }

            if (c == ' ') {
                // Next argument
                if(is_argument_start == 0) {
                    is_argument_start = 1;
                }
                command[current_char_index++] = c;
                shell->output(c);
            } else if (c == BACKSPACE || c == DELETE) {
                if(current_char_index > 0) {
                    current_char_index--;
                    command[current_char_index] = '\0';
                    if (current_char_index > 0 && command[current_char_index - 1] == ' ') {
                        is_argument_start = 1;
                    } else {
                        is_argument_start = 0;
                    }
                    write_string(shell, DELETE_CHAR_SEQ);
                }
            } else if (c == CARRIAGE_DOWN || c == NEWLINE) {
                command[current_char_index] = '\0';
                write_string(shell, "\r\n");
                break;
            }
            else {
                if(is_argument_start == 1) {
                    is_argument_start = 0;
                    arg_count++;
                }
                command[current_char_index++] = c;
                shell->output(c);
            }
        }

        if(!strncmp(command, "echo ", 5)) {
            echo(shell, command + 5);
        } else {
            write_string(shell, "Unrecognized command: ");
            echo(shell, command);   
        }

    }
}

void parse_command(char * command, int command_length) {
    // Todo for when we actually need this for different programs.
}

