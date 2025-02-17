#include "../include/uart.h"
#include "../include/shell.h"
#include "../include/interrupts.h"
#include "../include/timer.h"

void kernel_main(void)
{
    struct shell kernel_shell;
    kernel_shell.input = &recv_with_timeout_uart;
    kernel_shell.output = &send_uart;
    run_shell(&kernel_shell);
    
    while(1){
    }
}
