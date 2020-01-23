BUILD_DIR = build
SRC_DIR = kernel

ASM = nasm
ASM_DIR = bootloader
ASMFLAGS = -f bin -i./$(ASM_DIR)/

QEMU = qemu-system-x86_64

CC = i386-elf-gcc
CFLAGS := -Wall -Wextra
# CFLAGS := -O2

LD = i386-elf-ld
LDFLAGS := -Ttext 0x1000 # Same offset as in boot.asm
LDFLAGS := --oformat binary

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, build/%.o, $(SOURCES))
ENTRY_SRC = $(SRC_DIR)/kernel_entry.asm
ASM_SRC = $(ASM_DIR)/boot.asm

BOOTLOADER = $(BUILD_DIR)/boot_sector.bin
KERNEL = $(BUILD_DIR)/kernel.bin
ENTRY_BIN = $(BUILD_DIR)/entry.bin
IMAGE = $(BUILD_DIR)/boot_image.bin

all: $(IMAGE)

$(IMAGE): $(BUILD_DIR) $(BOOTLOADER) $(KERNEL)
	cat $(BOOTLOADER) $(KERNEL) > $@

$(BOOTLOADER): $(ASM_SRC)
	$(ASM) $(ASMFLAGS) $(ASM_SRC) -o $@

$(KERNEL): $(ENTRY_BIN) $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $^

$(ENTRY_BIN): $(ENTRY_SRC)
	$(ASM) $< -f elf -o $@

$(OBJECTS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

boot: $(IMAGE)
	$(QEMU) -fda $(IMAGE)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean all