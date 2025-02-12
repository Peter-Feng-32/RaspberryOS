#import "../include/interrupts.h"
#import "../include/utils.h"
#import "../include/uart.h"
#import "../include/timer.h"

//Enable or disable irq depending on interrupt enable/disable register passed in
void enable_disable_irq(int interrupt_register, int interrupt) {
    put32(interrupt_register, interrupt);
}

void handle_irq() {
    send_uart('I');

    unsigned int pending_interrupts = get32(IRQ_1_PENDING);
    if (pending_interrupts & SYSTEM_TIMER_1_IRQ) {
        send_uart('T');
        send_uart('\n');

        put32(TIMER_CS, get32(TIMER_CS) | TIMER_1_MASK);
    }
    
}