#include "../../include/utils/shell.h"
#include "../../include/utils/strings.h"
#include "../../include/devices/timer.h"
#include "../../include/utils/atags.h"

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

void echo(struct shell* shell, char* arg) {
    strwrite(shell->output, arg);
    strwrite(shell->output, "\r\n");
}

void run_shell(struct shell* shell) {
    strwrite(shell->output, "Welcome to Raspberry Pi Kernel Shell!\r\n");

    while(1) {
        strwrite(shell->output, SHELL_PREFIX);

        char command[COMMAND_MAX_LEN];
        int current_char_index = 0;
        int arg_count = 0;
        char is_argument_start = 0;

        while (current_char_index < COMMAND_MAX_LEN) {
            char c;
            shell->input(&c, 0);

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
                    strwrite(shell->output, DELETE_CHAR_SEQ);
                }
            } else if (c == CARRIAGE_DOWN || c == NEWLINE) {
                command[current_char_index] = '\0';
                strwrite(shell->output, "\r\n");
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
        } else if (!strncmp(command, "atags ", 6)) {
            print_atags(shell->output, (struct atag *) 0x100);
        } else {
            strwrite(shell->output, "Unrecognized command: ");
            echo(shell, command);   
        }

    }
}

void parse_command(char * command, int command_length) {
    // Todo for when we actually need this for different programs.
}

