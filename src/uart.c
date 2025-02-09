#include "../include/gpio.h"
#include "../include/utils.h"
#include "../include/uart.h"

void uart_init(){
    /**
    UART Transmit (TXD1) and UART Receive(RXD1) are on GPIO pins GPIO14 and GPIO15 respectively.

    1) Set gpfsel to select the correct GPIO functionality for pins 14, 15
    
    2) Configure pull-up / pull-down of GPIO pins?  Is this necessary?

    3) Initialize Mini UART
     */
}

/**
    Set GPIO14 and GPIO15 to ALT5 functionality
        In GPFSEL1, FSEL14 and FSEL15 are set to 010
 */
void set_gpfsel(){
    unsigned int selector = get32(GPFSEL1_ADDR);

    selector &= ~(7 << 12); //Clear existing values
    selector &= ~(7 << 15);
    selector |= (ALT5 << 12); //Set new values
    selector |= (ALT5 << 15); 
    put32(GPFSEL1_ADDR, selector);
}

/**
Initialize our mini uart.

Set AUXENB register bit 0 to high which enables the mini UART module.
This enables access to all other registers.

Disable auto flow control, receiver, and transmitter in AUX_MU_CNTL_REG

Disable interrupts while we can't handle them.
Configure baud rate and data frame.

Set RTS(request-to-send) to always be on.

Enable receiver and transmitter.
 */

void initialize_uart() {
    put32(AUX_ENB_ADDR, get32(AUX_ENB_ADDR) | 1);
    put32(AUX_MU_CNTL_REG_ADDR, 0);
    unsigned int mask = 3; // Clear last 2 bits
    put32(AUX_MU_IER_REG_ADDR, get32(AUX_MU_IER_REG_ADDR) & ~(mask));
    put32(AUX_MU_BAUD_ADDR, 270);
    put32(AUX_MU_LCR_REG_ADDR, 3);
    put32(AUX_MU_MCR_REG_ADDR, 0);

    unsigned int r;
    put32(GPPUD_ADDR,0);
    r = 150; while (r--) {asm volatile ("nop");};
    put32(GPPUD_ADDR,(1<<14)|(1<<15));
    r = 150; while (r--) {asm volatile ("nop");};
    put32(GPPUD_ADDR,0);

    //Enable
    put32(AUX_MU_CNTL_REG_ADDR, 3);
}

void send_uart(char c) {
    while(1) {
        if(get32(AUX_MU_LSR_REG_ADDR) & 32) {
            break;
        }
    }
    put32(AUX_MU_IO_REG_ADDR, c);
}

char recv_uart() {
    while(1) {
        if(get32(AUX_MU_LSR_REG_ADDR) & 1) {
            break;
        }
    }
    return (char) get32(AUX_MU_IO_REG_ADDR);
}