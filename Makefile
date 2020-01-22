BUILD_DIR=build

ASM=nasm
ASM_DIR=bootloader
ASMFLAGS= -f bin -i./$(ASM_DIR)/
ASM_SRC=$(ASM_DIR)/boot.asm

CC=i386-elf-gcc
CFLAGS=-Wall -Wextra -O2
LD=i386-elf-ld
LDFLAGS=-Ttext 0x0 --oformat binary
SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst src/%.c, build/%.o, $(SOURCES))

BOOTLOADER=$(BUILD_DIR)/boot.bin
TESTBINARY=out.bin

all: $(BUILD_DIR) $(BOOTLOADER) $(TESTBINARY)

$(BOOTLOADER): $(ASM_SRC)
	$(ASM) $(ASMFLAGS) $(ASM_SRC) -o $(BOOTLOADER)

$(TESTBINARY): $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $(OBJECTS)  

$(OBJECTS) : build/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(TESTBINARY) $(BOOTLOADER) $(OBJECTS) $(BUILD_DIR)