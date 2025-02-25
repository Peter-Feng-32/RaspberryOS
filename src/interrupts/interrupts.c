#include "../../include/interrupts/interrupts.h"
#include "../../include/utils/utils.h"
#include "../../include/devices/timer.h"
#include "../../include/process/scheduler.h"
#include "../../include/process/process.h"
#include "../../include/devices/uart.h"
#include "../../include/utils/strings.h"
#include "../../include/utils/shell.h"

//Enable or disable irq depending on interrupt enable/disable register passed in
void enable_interrupt_controller() {
    put32(ENABLE_IRQS_1, SYSTEM_TIMER_1_IRQ);
}
extern void test_fun(void);
extern void test_fun2(void);
void handle_irq() {

    unsigned int pending_interrupts = get32(IRQ_1_PENDING);
    if (pending_interrupts & SYSTEM_TIMER_1_IRQ) {
        // Handle timer interrupt here
        put32(TIMER_C1,  (int) get32(TIMER_CLO) + 2000000);
        put32(TIMER_CS, get32(TIMER_CS) | TIMER_1_MASK);
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

    }

    
}