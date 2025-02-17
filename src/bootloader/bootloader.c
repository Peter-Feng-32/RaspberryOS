#include "../../include/uart.h"
#include "../../include/interrupts.h"
#include "../../include/timer.h"
#include "../../include/xmodem.h"
#include "../../include/bootloader.h"

void branch_unconditionally(void * target) {
    {asm volatile ("br %0" :: "r"(target));};
    while(1);
}

void receive_kernel(void) {
    // Todo: set up exception table
    irq_vector_init();
    enable_irq();
    enable_interrupt_controller();
    
    initialize_uart();

    int received = 0;
    while (!received) {
        if (xmodem_receive(send_uart, recv_with_timeout_uart, BINARY_START, BOOTLOADER_TRANSFER_TIMEOUT)) {
            received = 1;
        } else {
            send_uart('P');
            send_uart('E');
            send_uart('N');
            send_uart('D');
            send_uart('I');
            send_uart('N');
            send_uart('G');
            send_uart('\n');

        }
    } 
    branch_unconditionally(BINARY_START);

}

