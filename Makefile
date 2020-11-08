BUILD_DIR = build
KERNEL_DIR = kernel
DRIVERS_DIR = drivers
CPU_DIR = cpu
LIBC_DIR = libc

ASM = nasm
BOOT_DIR = bootloader
BOOT_ASMFLAGS = -f bin -i./$(BOOT_DIR)/

QEMU = qemu-system-x86_64
GDB = gdb

CC = ./cross-tools/bin/i386-elf-gcc 
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
CFLAGS += -I$(KERNEL_DIR)
CFLAGS += -I$(DRIVERS_DIR)
CFLAGS += -I$(CPU_DIR)
CFLAGS += -I$(LIBC_DIR)
CFLAGS += -ffreestanding

LD = ./cross-tools/bin/i386-elf-ld
LD_SOURCES=./cross-tools/binutils.tar.gz
LD_OFFSET = -Ttext 0x1000 # Same offset as in boot.asm
LD_BINARY = --oformat binary

KERNEL_SOURCES = $(wildcard $(KERNEL_DIR)/*.c)
KERNEL_OBJECTS = $(patsubst $(KERNEL_DIR)/%.c, build/$(KERNEL_DIR)/%.o, $(KERNEL_SOURCES))
DRIVERS_SOURCES = $(wildcard $(DRIVERS_DIR)/*.c)
DRIVERS_OBJECTS = $(patsubst $(DRIVERS_DIR)/%.c, build/$(DRIVERS_DIR)/%.o, $(DRIVERS_SOURCES))
LIBC_SOURCES = $(wildcard $(LIBC_DIR)/*.c)
LIBC_OBJECTS = $(patsubst $(LIBC_DIR)/%.c, build/$(LIBC_DIR)/%.o, $(LIBC_SOURCES))
CPU_SOURCES = $(wildcard $(CPU_DIR)/*.c)
CPU_OBJECTS = $(patsubst $(CPU_DIR)/%.c, build/$(CPU_DIR)/%.o, $(CPU_SOURCES))
CPU_ASM = $(wildcard $(CPU_DIR)/*.asm)
CPU_ASM_OBJECTS = $(patsubst $(CPU_DIR)/%.asm, build/$(CPU_DIR)/%.asm.o, $(CPU_ASM))
CPU_ASMFLAGS = -f elf -i./$(CPU_DIR)/

ENTRY_SRC = $(KERNEL_DIR)/kernel_entry.asm
ASM_BOOTFILE = $(BOOT_DIR)/boot.asm
ASM_SRC = $(wildcard $(BOOT_DIR)/*.asm)

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
	$(ASM) $(BOOT_ASMFLAGS) $(ASM_BOOTFILE) -o $@

$(OS): $(ENTRY_BIN) $(KERNEL_OBJECTS) $(DRIVERS_OBJECTS) $(CPU_OBJECTS) $(CPU_ASM_OBJECTS) $(LIBC_OBJECTS)
	$(LD) -o $@ $(LD_OFFSET) $(LD_BINARY) $^

$(KERNEL_SYMBOLS): $(ENTRY_BIN) $(KERNEL_OBJECTS) $(DRIVERS_OBJECTS) $(CPU_OBJECTS) $(CPU_ASM_OBJECTS) $(LIBC_OBJECTS)
	$(LD) -o $@ $(LD_OFFSET) $^

$(ENTRY_BIN): $(ENTRY_SRC)
	$(ASM) $< -f elf -o $@

$(KERNEL_OBJECTS): $(BUILD_DIR)/$(KERNEL_DIR)/%.o : $(KERNEL_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DRIVERS_OBJECTS): $(BUILD_DIR)/$(DRIVERS_DIR)%.o : $(DRIVERS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CPU_OBJECTS): $(BUILD_DIR)/$(CPU_DIR)%.o : $(CPU_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBC_OBJECTS): $(BUILD_DIR)/$(LIBC_DIR)%.o : $(LIBC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CPU_ASM_OBJECTS): $(BUILD_DIR)/$(CPU_DIR)%.asm.o : $(CPU_DIR)/%.asm
	$(ASM) $(CPU_ASMFLAGS) $< -o $@

boot: $(IMAGE)
	$(QEMU) -drive format=raw,file=$(IMAGE)

debug: CFLAGS += -g -DDEBUG
debug: BOOT_ASMFLAGS += -g
debug: CPU_ASMFLAGS += -g
debug: clean $(IMAGE) $(KERNEL_SYMBOLS)
	$(QEMU) -drive format=raw,file=$(IMAGE) -s -S &
	$(GDB) -ex "set architecture i386:x86-64" -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_SYMBOLS)"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/$(KERNEL_DIR)
	@mkdir -p $(BUILD_DIR)/$(DRIVERS_DIR)
	@mkdir -p $(BUILD_DIR)/$(CPU_DIR)
	@mkdir -p $(BUILD_DIR)/$(LIBC_DIR)


tools: $(TOOLS)

$(TOOLS):
	@$(MAKE) -C cross-tools

reset: clean
	@$(MAKE) -C cross-tools reset

clean:
	rm -rf $(BUILD_DIR)

re: clean all
