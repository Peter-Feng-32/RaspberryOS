CFLAGS = -Wall -O0 -ffreestanding -nostdinc -nostdlib -Iinclude -mcpu=cortex-a53+nosimd -g

BUILD_DIR = build
SRC_DIR=src
TOOLS_DIR = tools

all: kernel8.img

clean: 
	rm -rf $(BUILD_DIR) *.img

SUBDIRS = bootloader devices interrupts malloc utils xmodem process

C_FILES = $(foreach dir, $(SUBDIRS), $(wildcard $(SRC_DIR)/$(dir)/*.c))
C_FILES += $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(foreach dir, $(SUBDIRS), $(wildcard $(SRC_DIR)/$(dir)/*.S))
ASM_FILES += $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.S, $(BUILD_DIR)/%_S.o, $(ASM_FILES))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%_c.o, $(C_FILES))



BOOTLOADER_OBJ_FILES = $(BUILD_DIR)/bootloader/start_S.o $(BUILD_DIR)/devices/uart_c.o $(BUILD_DIR)/devices/timer_c.o $(BUILD_DIR)/interrupts/interrupts_c.o $(BUILD_DIR)/interrupts/interrupts_S.o $(BUILD_DIR)/xmodem/xmodem_c.o  $(BUILD_DIR)/utils/utils_S.o $(BUILD_DIR)/interrupts/irq_vector_table_S.o $(BUILD_DIR)/bootloader/bootloader_c.o 
BOOTLOADER_OBJ_FILES += $(BUILD_DIR)/process/process_c.o $(BUILD_DIR)/process/process_S.o $(BUILD_DIR)/process/scheduler_c.o $(BUILD_DIR)/process/scheduler_S.o $(BUILD_DIR)/malloc/bin_malloc_c.o $(BUILD_DIR)/kernel_c.o  $(BUILD_DIR)/utils/strings_c.o $(BUILD_DIR)/utils/shell_c.o $(BUILD_DIR)/utils/atags_c.o
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
	clang $(TOOLS_DIR)/transmit.c -o $(BUILD_DIR)/transmit 
	$(BUILD_DIR)/transmit uart1.out uart1.in $(BUILD_DIR)/kernel8.bin

run: kernel8.img
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/kernel8.img -d in_asm -serial null -serial vc 

make run_bootloader: bootloader
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/bootloader/kernel8.img -d in_asm -serial null -serial vc 

uart1.in:
	mkfifo uart1.in

uart1.out:
	mkfifo uart1.out

#This will not work without changing where the bootloader is loaded in linker script as we do not have a config.txt file for the emulator.
make run_bootloader_emulator: bootloader uart1.in uart1.out
	qemu-system-aarch64 -M raspi3b -kernel build/bootloader/kernel8.img -nographic -serial null -chardev pipe,id=uart1,path=uart1 -serial chardev:uart1 -s -S

transmit_pi: kernel8.bin
	clang $(TOOLS_DIR)/transmit.c -o $(BUILD_DIR)/transmit 
	$(BUILD_DIR)/transmit /dev/cu.usbserial-0001 /dev/cu.usbserial-0001 $(BUILD_DIR)/kernel8.bin

