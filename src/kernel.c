#include "../include/devices/uart.h"
#include "../include/utils/shell.h"
#include "../include/interrupts/interrupts.h"
#include "../include/devices/timer.h"
#include "../include/malloc/bin_malloc.h"

void kernel_main(void)
{
    struct shell kernel_shell;
    kernel_shell.input = &recv_with_timeout_uart;
    kernel_shell.output = &send_uart;

    bin_malloc_init();

    run_shell(&kernel_shell);
    
    while(1){
    }
}
