#import "../../include/interrupts/interrupts.h"
#import "../../include/utils/utils.h"
#import "../../include/devices/timer.h"

//Enable or disable irq depending on interrupt enable/disable register passed in
void enable_interrupt_controller() {
    put32(ENABLE_IRQS_1, SYSTEM_TIMER_1_IRQ);
}

void handle_irq() {
    unsigned int pending_interrupts = get32(IRQ_1_PENDING);
    if (pending_interrupts & SYSTEM_TIMER_1_IRQ) {
        // Handle timer interrupt here.
        put32(TIMER_CS, get32(TIMER_CS) | TIMER_1_MASK);
    }

    
}