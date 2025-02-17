/// Start address of the binary to load and of the bootloader.
#define BINARY_START_ADDR 0x80000
#define BOOTLOADER_START_ADDR 0x4000000

/// Pointer to where the loaded binary expects to be laoded.
#define BINARY_START ((char *) BINARY_START_ADDR)

/// Free space between the bootloader and the loaded binary's start address.
#define MAX_BINARY_SIZE (BOOTLOADER_START_ADDR - BINARY_START_ADDR)

#define BOOTLOADER_TRANSFER_TIMEOUT 7000000