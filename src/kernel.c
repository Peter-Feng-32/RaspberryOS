#include "../include/uart.h"

void kernel_main(void)
{
    initialize_uart();
    send_uart('H');
    send_uart('E');
    send_uart('L');
    send_uart('L');
    send_uart('O');
    send_uart(' ');
    send_uart('W');
    send_uart('O');
    send_uart('R');
    send_uart('L');
    send_uart('D');
    send_uart('!');

    while(1){

    }
}