CFLAGS = -Wall -O0 -ffreestanding -nostdinc -nostdlib -Iinclude -mcpu=cortex-a53+nosimd -g

BUILD_DIR = build
SRC_DIR=src
UTILS_DIR = utils

all: kernel8.img

clean: 
	rm -rf $(BUILD_DIR) *.img

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_S.o)

BOOTLOADER_OBJ_FILES = $(BUILD_DIR)/start_S.o $(BUILD_DIR)/uart_c.o $(BUILD_DIR)/timer_c.o $(BUILD_DIR)/interrupts_c.o $(BUILD_DIR)/interrupts_S.o $(BUILD_DIR)/xmodem_c.o  $(BUILD_DIR)/utils_S.o $(BUILD_DIR)/irq_vector_table_S.o $(BUILD_DIR)/bootloader_c.o 

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	clang --target=aarch64-elf $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%_S.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	clang --target=aarch64-elf $(CFLAGS) -c $< -o $@

kernel8.img: $(SRC_DIR)/link.ld $(OBJ_FILES)
	ld.lld -v -m aarch64elf -nostdlib -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

kernel8.bin: $(SRC_DIR)/link.ld $(OBJ_FILES)
	ld.lld -v -m aarch64elf -nostdlib -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.bin

bootloader: $(BOOTLOADER_OBJ_FILES)
	mkdir -p $(BUILD_DIR)/bootloader
	ld.lld -m aarch64elf -nostdlib -T $(SRC_DIR)/bootloader/link.ld -o $(BUILD_DIR)/bootloader/kernel8.elf $(BOOTLOADER_OBJ_FILES)
	llvm-objcopy -O binary $(BUILD_DIR)/bootloader/kernel8.elf $(BUILD_DIR)/bootloader/kernel8.img

transmit: kernel8.bin
	clang $(UTILS_DIR)/transmit.c -o $(BUILD_DIR)/transmit 
	$(BUILD_DIR)/transmit uart1.out uart1.in $(BUILD_DIR)/kernel8.bin

run: kernel8.img
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/kernel8.img -d in_asm -serial null -serial vc 

make run_bootloader: bootloader
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/bootloader/kernel8.img -d in_asm -serial null -serial vc 

transmit_pi: kernel8.bin
	clang $(UTILS_DIR)/transmit.c -o $(BUILD_DIR)/transmit 
	$(BUILD_DIR)/transmit /dev/cu.usbserial-0001 /dev/cu.usbserial-0001 $(BUILD_DIR)/kernel8.bin
