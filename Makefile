CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -Iinclude -mcpu=cortex-a53+nosimd -g

BUILD_DIR = build
SRC_DIR=src

all: kernel8.img

clean: 
	rm -rf $(BUILD_DIR) *.img

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_S.o)

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	clang --target=aarch64-elf $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%_S.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	clang --target=aarch64-elf $(CFLAGS) -c $< -o $@

kernel8.img: $(SRC_DIR)/link.ld $(OBJ_FILES)
	ld.lld -m aarch64elf -nostdlib -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

run: kernel8.img
	qemu-system-aarch64 -M raspi3b -kernel $(BUILD_DIR)/kernel8.img -d in_asm -serial null -serial vc -s