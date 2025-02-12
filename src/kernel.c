#include "../include/uart.h"
#include "../include/shell.h"
#include "../include/interrupts.h"
#include "../include/timer.h"

void kernel_main(void)
{
    // Todo: set up exception table
    irq_vector_init();
    enable_disable_irq(ENABLE_IRQS_1, SYSTEM_TIMER_1_IRQ);
    
    initialize_uart();
    
    struct shell kernel_shell;
    kernel_shell.input = &recv_uart;
    kernel_shell.output = &send_uart;

    run_shell(&kernel_shell);

    while(1){
    }
}