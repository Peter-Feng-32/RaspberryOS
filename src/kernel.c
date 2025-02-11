#include "../include/uart.h"
#include "../include/shell.h"

void kernel_main(void)
{
    initialize_uart();
    
    struct shell kernel_shell;
    kernel_shell.input = &recv_uart;
    kernel_shell.output = &send_uart;

    run_shell(&kernel_shell);

    while(1){
    }
}