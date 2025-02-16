#ifndef _UART_H
#define _UART_H
#include "peripherals.h"

void initialize_uart();
int send_uart(char c);
int recv_uart(char* c);

//Write and read data from here.
#define AUX_MU_IO_REG_ADDR (MMAP_DEVICE_BASE + 0x00215040)
//Enable UART1 from here.
#define AUX_ENB_ADDR (MMAP_DEVICE_BASE + 0x00215004)
//Enable/disable interrupts from here.
#define AUX_MU_IER_REG_ADDR (MMAP_DEVICE_BASE + 0x00215044)
//Control bit mode here and access the baud rate register.
#define AUX_MU_LCR_REG_ADDR (MMAP_DEVICE_BASE + 0x0021504C)
//Control RTS here
#define AUX_MU_MCR_REG_ADDR (MMAP_DEVICE_BASE + 0x00215050)
//Check the data status here
#define AUX_MU_LSR_REG_ADDR (MMAP_DEVICE_BASE + 0x00215054)
//Extra features here
#define AUX_MU_CNTL_REG_ADDR (MMAP_DEVICE_BASE + 0x00215060)
//Set baud rate here
#define AUX_MU_BAUD_ADDR (MMAP_DEVICE_BASE + 0x00215068)

#define BYTE_READY 0
#define TIMED_OUT 1

#endif