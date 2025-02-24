#include "../include/devices/uart.h"
#include "../include/utils/shell.h"
#include "../include/interrupts/interrupts.h"
#include "../include/devices/timer.h"
#include "../include/malloc/bin_malloc.h"
#include "../include/process/scheduler.h"
#include "../include/process/process.h"
#include "../include/utils/utils.h"


void test_fun(void) {
    while (1) {
        send_uart('c');
        delay(10000);
    }
}

void kernel_main(void)
{
    struct shell kernel_shell;
    kernel_shell.input = &recv_with_timeout_uart;
    kernel_shell.output = &send_uart;

    tick_in(200000);

    bin_malloc_init();
    initialize_scheduler(&run_shell, &kernel_shell);
    make_process(&test_fun, 0);
    
    run_shell(&kernel_shell);
    
    while(1){
    }
}
