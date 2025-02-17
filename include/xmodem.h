#import "../include/io.h"

#ifndef _XMODEM_H
#define _XMODEM_H

#define SOH 1
#define EOT 4
#define ACK 6
#define NAK 21
#define CAN 24

#define PACKET_NUM_MAX 255
#define PACKET_SIZE 128
#define CHECKSUM_MOD 256
#define MAX_RETRIES 10

#define CANCELED 1
#define IN_PROGRESS 2

#define SUCCESS 0
#define FAILURE -1

int xmodem_receive(sender_func_type sender_func, receiver_func_type receiver_func, char * recv_buffer, int timeout);
int xmodem_send(sender_func_type sender_func, receiver_func_type receiver_func, char * send_buffer, int send_size, int timeout);

#endif
