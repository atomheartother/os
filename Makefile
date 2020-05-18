BUILD_DIR = build
KERNEL_DIR = kernel
DRIVERS_DIR = drivers
CPU_DIR = cpu

ASM = nasm
ASM_DIR = bootloader
ASMFLAGS = -f bin -i./$(ASM_DIR)/

QEMU = qemu-system-x86_64
GDB = gdb

CC = ./cross-tools/bin/i386-elf-gcc 
CFLAGS := -Wall -Wextra
CFLAGS += -I$(KERNEL_DIR)
CFLAGS += -I$(DRIVERS_DIR)

LD = ./cross-tools/bin/i386-elf-ld
LD_SOURCES=./cross-tools/binutils.tar.gz
LD_OFFSET = -Ttext 0x1000 # Same offset as in boot.asm
LD_BINARY = --oformat binary

KERNEL_SOURCES = $(wildcard $(KERNEL_DIR)/*.c)
KERNEL_OBJECTS = $(patsubst $(KERNEL_DIR)/%.c, build/$(KERNEL_DIR)/%.o, $(KERNEL_SOURCES))
DRIVERS_SOURCES = $(wildcard $(DRIVERS_DIR)/*.c)
DRIVERS_OBJECTS = $(patsubst $(DRIVERS_DIR)/%.c, build/$(DRIVERS_DIR)/%.o, $(DRIVERS_SOURCES))
CPU_SOURCES = $(wildcard $(CPU_DIR)/*.c)
CPU_OBJECTS = $(patsubst $(CPU_DIR)/%.c, build/$(CPU_DIR)/%.o, $(CPU_SOURCES))

ENTRY_SRC = $(KERNEL_DIR)/kernel_entry.asm
ASM_SRC = $(ASM_DIR)/boot.asm

BOOTLOADER = $(BUILD_DIR)/boot_sector.bin
OS = $(BUILD_DIR)/os.bin
ENTRY_BIN = $(BUILD_DIR)/entry.bin
IMAGE = $(BUILD_DIR)/boot_image.bin
KERNEL_SYMBOLS = $(BUILD_DIR)/kernel.elf

all: CFLAGS += -O
all: $(IMAGE)

$(IMAGE): $(BUILD_DIR) $(BOOTLOADER) $(OS)
	cat $(BOOTLOADER) $(OS) > $@

$(BOOTLOADER): $(ASM_SRC)
	$(ASM) $(ASMFLAGS) $(ASM_SRC) -o $@

$(OS): $(ENTRY_BIN) $(KERNEL_OBJECTS) $(DRIVERS_OBJECTS) $(CPU_OBJECTS)
	$(LD) -o $@ $(LD_OFFSET) $(LD_BINARY) $^

$(KERNEL_SYMBOLS): $(ENTRY_BIN) $(KERNEL_OBJECTS) $(DRIVERS_OBJECTS) $(CPU_OBJECTS)
	$(LD) -o $@ $(LD_OFFSET) $^

$(ENTRY_BIN): $(ENTRY_SRC)
	$(ASM) $< -f elf -o $@

$(KERNEL_OBJECTS): $(BUILD_DIR)/$(KERNEL_DIR)/%.o : $(KERNEL_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DRIVERS_OBJECTS): $(BUILD_DIR)/$(DRIVERS_DIR)%.o : $(DRIVERS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CPU_OBJECTS): $(BUILD_DIR)/$(CPU_DIR)%.o : $(CPU_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

boot: $(IMAGE)
	$(QEMU) -drive format=raw,file=$(IMAGE)

debug: CFLAGS += -g -DDEBUG
debug: clean $(IMAGE) $(KERNEL_SYMBOLS)
	$(QEMU) -drive format=raw,file=$(IMAGE) -s -S &
	$(GDB) -ex "set architecture i386:x86-64" -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_SYMBOLS)"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(KERNEL_DIR)
	@mkdir -p $(BUILD_DIR)/$(DRIVERS_DIR)
	@mkdir -p $(BUILD_DIR)/$(CPU_DIR)


tools: $(TOOLS)

$(TOOLS):
	@$(MAKE) -C cross-tools

reset: clean
	@$(MAKE) -C cross-tools reset

clean:
	rm -rf $(BUILD_DIR)

re: clean all
