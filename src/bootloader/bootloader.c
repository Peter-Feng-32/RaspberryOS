#include "../../include/devices/uart.h"
#include "../../include/interrupts/interrupts.h"
#include "../../include/devices/timer.h"
#include "../../include/xmodem/xmodem.h"
#include "../../include/bootloader/bootloader.h"

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
    volatile char c;

    int received = 0;
    while (!received) {
        int x = xmodem_receive(send_uart, recv_with_timeout_uart, (volatile char *) (BINARY_START), BOOTLOADER_TRANSFER_TIMEOUT);
        if (x) {
            send_uart('F');
            send_uart('A');
            send_uart('I');
            send_uart('L');
            send_uart('\r');
            send_uart('\n');
        } else {
            send_uart('S');
            send_uart('U');
            send_uart('C');
            send_uart('C');
            send_uart('E');
            send_uart('S');
            send_uart('S');
            send_uart('\r');
            send_uart('\n');
            branch_unconditionally(BINARY_START);
        }
    } 


}

