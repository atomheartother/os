BUILD_DIR = build
KERNEL_DIR = kernel
DRIVERS_DIR = drivers
CPU_DIR = cpu
LIBC_DIR = libc

ASM = nasm

QEMU = qemu-system-x86_64
GDB = gdb

CC = ./cross-tools/bin/i386-elf-gcc 
CFLAGS := -Wall -Wextra -Werror
CFLAGS += -m32 -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -nostdlib
CFLAGS += -I$(KERNEL_DIR)
CFLAGS += -I$(DRIVERS_DIR)
CFLAGS += -I$(CPU_DIR)
CFLAGS += -I$(LIBC_DIR)
CFLAGS += -ffreestanding

LD = ./cross-tools/bin/i386-elf-ld
LD_SOURCES=./cross-tools/binutils.tar.gz
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
CPU_ASMFLAGS = -felf32 -i./$(CPU_DIR)/

MULTIBOOT_ASMFLAGS = -felf32 -i./$(KERNEL_DIR)/
MULTIBOOT_ASM = $(KERNEL_DIR)/multiboot.asm
MULTIBOOT_SRC = $(MULTIBOOT_ASM) \
				$(KERNEL_DIR)/32b_gdt.asm

MULTIBOOT_OBJ = $(BUILD_DIR)/multiboot.o

KERNEL = $(BUILD_DIR)/kernel.bin
KERNEL_SYMBOLS = $(BUILD_DIR)/kernel.elf

ISO_DIR = $(BUILD_DIR)/iso
ISO_BOOTDIR = $(ISO_DIR)/boot
GRUBDIR = $(ISO_BOOTDIR)/grub
ISO = $(BUILD_DIR)/os.iso

all: CFLAGS += -O
all: $(ISO)

$(ISO): $(BUILD_DIR) $(GRUBDIR) $(KERNEL)
	cp $(KERNEL) $(ISO_BOOTDIR)/os.bin
	cp grub.cfg $(GRUBDIR)/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

$(KERNEL): $(MULTIBOOT_OBJ) $(KERNEL_OBJECTS) $(DRIVERS_OBJECTS) $(CPU_OBJECTS) $(CPU_ASM_OBJECTS) $(LIBC_OBJECTS)
	$(CC) -T linker.ld -o $@ $(CFLAGS) $^ -lgcc

$(KERNEL_SYMBOLS): $(KERNEL)
	objcopy --only-keep-debug $(KERNEL) $@
	objcopy --strip-debug $(KERNEL)

$(MULTIBOOT_OBJ): $(MULTIBOOT_SRC)
	$(ASM) $(MULTIBOOT_ASMFLAGS) $(MULTIBOOT_ASM) -o $@

$(KERNEL_OBJECTS): $(BUILD_DIR)/$(KERNEL_DIR)/%.o : $(KERNEL_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DRIVERS_OBJECTS): $(BUILD_DIR)/$(DRIVERS_DIR)/%.o : $(DRIVERS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CPU_OBJECTS): $(BUILD_DIR)/$(CPU_DIR)/%.o : $(CPU_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBC_OBJECTS): $(BUILD_DIR)/$(LIBC_DIR)/%.o : $(LIBC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CPU_ASM_OBJECTS): $(BUILD_DIR)/$(CPU_DIR)%.asm.o : $(CPU_DIR)/%.asm
	$(ASM) $(CPU_ASMFLAGS) $< -o $@

multiboot: $(ISO)
	$(QEMU) -cdrom $(ISO)

boot: $(BUILD_DIR) $(KERNEL)
	$(QEMU) -kernel  $(KERNEL)

debug: CFLAGS += -g -DDEBUG
debug: MULTIBOOT_ASMFLAGS += -g
debug: CPU_ASMFLAGS += -g
debug: clean $(BUILD_DIR) $(KERNEL_SYMBOLS)
	$(QEMU) -kernel $(KERNEL) -s -S &
	$(GDB) -ex "set architecture i386:x86-64" -ex "target remote localhost:1234" -ex "symbol-file $(KERNEL_SYMBOLS)"

$(GRUBDIR):
	@mkdir -p $(GRUBDIR)

$(BUILD_DIR):
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
