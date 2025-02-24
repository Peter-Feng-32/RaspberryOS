#include "../include/devices/uart.h"
#include "../include/utils/shell.h"
#include "../include/interrupts/interrupts.h"
#include "../include/devices/timer.h"
#include "../include/malloc/bin_malloc.h"
#include "../include/process/scheduler.h"
#include "../include/process/process.h"
#include "../include/utils/utils.h"
#include "../include/utils/strings.h"


void test_fun(void) {
    //enable_irq();

    while (1) {
    }
}

void kernel_main(void)
{
    struct shell kernel_shell;
    kernel_shell.input = &recv_with_timeout_uart;
    kernel_shell.output = &send_uart;


    irq_vector_init();
    enable_irq();
    enable_interrupt_controller();


    bin_malloc_init();
    initialize_scheduler(&run_shell, &kernel_shell);
    //tick_in(200000);

    put32(TIMER_C1,  (int) get32(TIMER_CLO) + 200000);


    make_process(&test_fun, 0);
    
    run_shell(&kernel_shell);
    
    while(1){
        /*
        print_number(get32(TIMER_CS));
        send_uart('\n');
        send_uart('\r');
        print_number(get32(TIMER_C1));
        send_uart('\n');
        send_uart('\r');
        print_number(get32(TIMER_CLO));
        send_uart('\n');
        send_uart('\r');
        
        print_number(get32(ENABLE_IRQS_1));
        send_uart('\n');
        send_uart('\r');

        delay(10000000);
        */
    }
}
