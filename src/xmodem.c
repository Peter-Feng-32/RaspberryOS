#import "../include/xmodem.h"
/**
Packets are 128 bytes.

Receiver sends a NAK byte to initiate.

Sender sends a SOH and the packet number
Then the 1's complement of the packet number
Then the packet itself.
Then the packet checksum ( sum of all bytes mod 256 )

The sender reads a byte from the receiver.
If byte is NAK, transmission is retried up to 10 times.
If byte is ACK, next packet is sent.

Either the receiver or sender can cancel the transmission by sending a CAN byte (Error out) - TODO

To end the transmission: 
Sender sends an EOT byte
Sender waits for a NAK byte.
Sender sends a second EOT byte.
Sender waits for an ACK byte.

Receiver:
Sends a NAK byte.
Waits for a second EOT byte.
Sends an ACK byte.

TODO: Error handling and CAN handling.

 */

int xmodem_receive(sender_func_type sender_func, receiver_func_type receiver_func, char * recv_buffer, int timeout) {
    sender_func(NAK);
    
    int recv_buffer_index = 0;

    int packet_num = 1;
    while(1) {
        char signal_byte;
        if (receiver_func(&signal_byte, timeout)) {
            return FAILURE;
        }

        if (signal_byte == EOT) {
            sender_func(NAK);
            char c;
            if (receiver_func(&c, timeout)) {
                sender_func(CAN);
                return FAILURE;
            }else if (c == EOT) {
                sender_func(ACK);
            } else {
                sender_func(CAN);
                return FAILURE;
            }
            break; // End transmission
        } else if (signal_byte != SOH) {
            sender_func(CAN);
            return FAILURE;
        }

        char recvd_packet_num;
        if(receiver_func(&recvd_packet_num, timeout) || recvd_packet_num != packet_num) {
            sender_func(CAN);
            return FAILURE;
        }
        char recvd_packet_num_complement;
        if(receiver_func(&recvd_packet_num_complement, timeout) || recvd_packet_num_complement != (255 - packet_num)) {
            sender_func(CAN);
            return FAILURE;
        }

        char checksum = 0;
        for (int i = 0; i < PACKET_SIZE; i++) {
            char recvd;
            if (receiver_func(&recvd, timeout)) {
                sender_func(CAN);
                return FAILURE;
            }
            recv_buffer[recv_buffer_index + i] = recvd;
            checksum += recvd;
        }
        checksum = checksum % CHECKSUM_MOD;

        char recvd_checksum;
        if (receiver_func(&recvd_checksum, timeout)) {
            sender_func(CAN);
            return FAILURE;
        }
        else if(recvd_checksum != checksum) {
            sender_func(NAK);
        } else {
            sender_func(ACK);
            recv_buffer_index += PACKET_SIZE;
            packet_num = (packet_num + 1) % (PACKET_NUM_MAX + 1);
        }

    }
    
    return SUCCESS;
}

int xmodem_send(sender_func_type sender_func, receiver_func_type receiver_func, char * send_buffer, int send_size, int timeout) {
    char start_NAK;
    if (receiver_func(&start_NAK, timeout) || start_NAK != NAK) {
        sender_func(CAN);
        return FAILURE;
    }

    // Calculate packets.
    int remaining_bytes = send_size;
    char packet_num = 1;
    int packet_unpadded_size = 0;
    int curr_sendbuffer_index = 0;

    int num_retries = 0;

    while (remaining_bytes > 0) {
        if (remaining_bytes >= PACKET_SIZE) {
            packet_unpadded_size = PACKET_SIZE;
        } else if (remaining_bytes > 0 && remaining_bytes < PACKET_SIZE) {
            packet_unpadded_size = remaining_bytes;
        }

        sender_func(SOH);
        sender_func(packet_num);
        sender_func(255 - packet_num);
        
        // ... Send packet here
        int checksum = 0;
        for (int i = 0; i < packet_unpadded_size; i++) {
            char c = send_buffer[curr_sendbuffer_index + i];
            checksum += c;
            sender_func(c);
        }
        checksum = checksum % CHECKSUM_MOD;

        // Handle padding with zeroes
        for (int i = 0; i < PACKET_SIZE - packet_unpadded_size; i++) {
            sender_func(0);
        }

        sender_func(checksum);

        char packet_recieved;
        if (receiver_func(&packet_recieved, timeout)) {
            sender_func(CAN);
            return FAILURE;
        }
        if(packet_recieved == ACK){
            // Packet received, move on.
            packet_num = (packet_num + 1) % (PACKET_NUM_MAX + 1);
            curr_sendbuffer_index += packet_unpadded_size;
            remaining_bytes -= packet_unpadded_size;
            num_retries = 0;
        } else {
            //Checksum doesn't match. Send again.
            if(num_retries < MAX_RETRIES) {
                num_retries += 1;
            } else {
                sender_func(CAN);
                break;
            }
        }
    }

    sender_func(EOT);
    char c;
    if (receiver_func(&c, timeout) || c != NAK) {
        return FAILURE;
    }
    sender_func(EOT);
    if (receiver_func(&c, timeout) || c != ACK) {
        return FAILURE;
    }

    return SUCCESS;
}