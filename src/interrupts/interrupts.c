#include "../../include/interrupts/interrupts.h"
#include "../../include/utils/utils.h"
#include "../../include/devices/timer.h"
#include "../../include/process/scheduler.h"
#include "../../include/process/process.h"
#include "../../include/devices/uart.h"
#include "../../include/utils/strings.h"

//Enable or disable irq depending on interrupt enable/disable register passed in
void enable_interrupt_controller() {
    put32(ENABLE_IRQS_1, SYSTEM_TIMER_1_IRQ);
}

void handle_irq() {

    unsigned int pending_interrupts = get32(IRQ_1_PENDING);
    if (pending_interrupts & SYSTEM_TIMER_1_IRQ) {
        // Handle timer interrupt here
        put32(TIMER_C1,  (int) get32(TIMER_CLO) + 2000000);
        put32(TIMER_CS, get32(TIMER_CS) | TIMER_1_MASK);

        send_uart('T');
        send_uart('\n');
        send_uart('\r');
        print_number(&send_uart, current_process->counter);
        send_uart('\n');
        send_uart('\r');        

        if(num_processes == 0) {
            //tick_in(200000);
            return;
        }


        current_process->counter--;
        if(current_process->counter > 0 || current_process->preempt_count > 0) {
            //tick_in(200000);
            return;
        }



        current_process->counter = 0;

        enable_irq();
        _schedule();
        disable_irq();
        
        //tick_in(200000);

        send_uart('X');
        send_uart('\n');
        send_uart('\r');
        print_number(&send_uart, current_process->counter);
        send_uart('\n');
        send_uart('\r');  
    }

    
}