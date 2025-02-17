#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define SOH  0x01  // Start of Header
#define EOT  0x04  // End of Transmission
#define ACK  0x06  // Acknowledge
#define NAK  0x15  // Negative Acknowledge
#define CAN  0x18  // Cancel
#define PACKET_SIZE 128

int transmit(int fd, char * filename) {
    FILE *file = fopen(filename, "rb"); // Open file in binary mode
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    unsigned char buffer[PACKET_SIZE];
    unsigned char packet[PACKET_SIZE + 4]; // SOH + Block# + ~Block# + Data + CRC
    int bytes_read;
    // Wait for receiver to send NAK
    unsigned char response;
    do {
        read(fd, &response, 1);
    } while (response != NAK);

    char packet_num = 1;

    while ((bytes_read = fread(buffer, 1, PACKET_SIZE, file)) > 0) {

        memset(packet, 0, sizeof(packet));

        packet[0] = SOH;
        packet[1] = packet_num;
        packet[2] = 255 - packet_num;
        memcpy(&packet[3], buffer, bytes_read);

        // ... Send packet here
        int checksum = 0;
        for (int i = 0; i < PACKET_SIZE; i++) {
            checksum += packet[i + 3];
        }
        checksum = checksum % 256;
        packet[PACKET_SIZE + 3] = checksum;

        write(fd, packet, sizeof(packet));

        // Wait for ACK
        read(fd, &response, 1);
        if (response != ACK) {
            fclose(file);
            return -1;
        }
        packet_num = (packet_num + 1) % 256;
        printf("Packet %d transmitted\n", packet_num);
    }
    // Send EOT
    do {
        char c = EOT;
        write(fd, &c, 1);
        read(fd, &response, 1);
    } while (response != ACK);

    fclose(file);
    return 0;
}

// Function to configure the serial port
int configure_serial(int fd) {
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // One stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;     // 8 bits per byte
    tcsetattr(fd, TCSANOW, &options);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <serial_port> <file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Serial port open failed");
        return 1;
    }

    configure_serial(fd);
    transmit(fd, argv[2]);

    close(fd);
    return 0;
}