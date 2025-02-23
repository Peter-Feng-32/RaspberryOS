#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define SOH  0x01  // Start of Header
#define EOT  0x04  // End of Transmission
#define ACK  0x06  // Acknowledge
#define NAK  0x15  // Negative Acknowledge
#define CAN  0x18  // Cancel
#define PACKET_SIZE 128

int fd_out;

int transmit(int fd, char * filename) {
    FILE *file = fopen(filename, "rb"); // Open file in binary mode
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    printf("Started\n");
    unsigned char buffer[PACKET_SIZE];
    unsigned char packet[PACKET_SIZE + 4]; // SOH + Block# + ~Block# + Data + CRC
    int bytes_read;
    // Wait for receiver to send NAK
    unsigned char response;


    do {
        read(fd, &response, 1);
    } while (response != NAK);


    unsigned char packet_num = 1;
    printf("Preparing to transmit packet\n");

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

        packet[0] = SOH;
        packet[1] = packet_num;
        packet[2] = 255 - packet_num;
        printf("Writing packet\n");
        write(fd_out, packet, sizeof(packet));
        fsync(fd_out);
        printf("Waiting for ACK\n");

        // Wait for ACK
        read(fd, &response, 1);
        if (response != ACK) {
            printf("Response %d\n", response);
            for(int i = 0; i < 10; i++) {
                char c;
                read(fd, &c, 1);
                printf("%d\n", c);
            }
            fclose(file);
            return -1;
        }
        packet_num = (packet_num + 1) % 256;
        printf("Packet %d transmitted\n", packet_num);
    }
    // Send EOT
    do {
        char c = EOT;
        write(fd_out, &c, 1);
        read(fd, &response, 1);
    } while (response != ACK);

    fclose(file);
    return 0;
}

// Function to configure the serial port
int configure_serial(int fd) {
    fcntl (fd, F_SETFL, O_RDWR) ;
    int status;

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag = (options.c_cflag & ~CSIZE) | CS8;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);  // Disable software flow control
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Raw mode
    options.c_oflag &= ~OPOST;  // No output processing
    options.c_cc[VMIN] = 1;  // Read at least 1 byte
    options.c_cc[VTIME] = 1;  // Timeout for reading
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <serial_port_in> <serial_port_out> <file>\n", argv[0]);
        return 1;
    }
    printf("Opening Serial Port\n");
    printf("%s\n", argv[1]);
    int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK );
    if (fd < 0) {
        perror("Serial port open failed");
        return 1;
    }
    if(argv[1] == argv[2]) {
        fd_out = fd;
    } else {
        fd_out = open(argv[2], O_RDWR | O_NOCTTY) ;
        if (fd_out < 0) {
            perror("Serial port open failed");
            return 1;
        }
    }

    printf("Opened Serial Port\n");

    configure_serial(fd);

    transmit(fd, argv[3]);

    close(fd);
    return 0;
}