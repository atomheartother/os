BUILD_DIR = build
SRC_DIR = kernel

ASM = nasm
ASM_DIR = bootloader
ASMFLAGS = -f bin -i./$(ASM_DIR)/

QEMU = qemu-system-x86_64
GDB = i386-elf-gdb

CC = i386-elf-gcc
CFLAGS := -Wall -Wextra
CFLAGS := -g
# CFLAGS := -O2

LD = i386-elf-ld
LD_OFFSET = -Ttext 0x1000 # Same offset as in boot.asm
LD_BINARY = --oformat binary

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, build/%.o, $(SOURCES))
ENTRY_SRC = $(SRC_DIR)/kernel_entry.asm
ASM_SRC = $(ASM_DIR)/boot.asm

BOOTLOADER = $(BUILD_DIR)/boot_sector.bin
KERNEL = $(BUILD_DIR)/kernel.bin
ENTRY_BIN = $(BUILD_DIR)/entry.bin
IMAGE = $(BUILD_DIR)/boot_image.bin
KERNEL_SYMBOLS = $(BUILD_DIR)/kernel.elf

all: $(IMAGE)

$(IMAGE): $(BUILD_DIR) $(BOOTLOADER) $(KERNEL)
	cat $(BOOTLOADER) $(KERNEL) > $@

$(BOOTLOADER): $(ASM_SRC)
	$(ASM) $(ASMFLAGS) $(ASM_SRC) -o $@

$(KERNEL): $(ENTRY_BIN) $(OBJECTS)
	$(LD) -o $@ $(LD_OFFSET) $(LD_BINARY) $^

$(KERNEL_SYMBOLS): $(ENTRY_BIN) $(OBJECTS)
	$(LD) -o $@ $(LD_OFFSET) $^

$(ENTRY_BIN): $(ENTRY_SRC)
	$(ASM) $< -f elf -o $@

$(OBJECTS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

boot: $(IMAGE)
	$(QEMU) -fda $(IMAGE)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

debug: $(IMAGE) $(KERNEL_SYMBOLS)
	$(QEMU) -s -fda $(IMAGE) &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_SYMBOLS)"

clean:
	rm -rf $(BUILD_DIR)

fclean: clean all