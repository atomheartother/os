#include "paging.h"
#include "screen.h"
#include "isr.h"
#include "shell.h"
#include "multiboot.h"
#include "debug.h"
#include "tar.h"

extern uint32_t _kernelEnd;
extern uint32_t _kernelStart;

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
#define KERNEL_START (uint32_t)&_kernelStart
#define KERNEL_END (uint32_t)&_kernelEnd

uint32_t intFromTarOctal(char* buf, uint8_t size) {
  // Check the buffer is zero-terminated
  uint32_t n = 0;

  for (uint8_t i=0; i < size ; i += 1) {
    if (buf[i] < '0' || buf[i] > '7') return n;
    n += n * 010 + (buf[i] - '0');
  }
  // number wasn't 0-terminated, we return 0
  return 0;
}

int kernel_main(uint32_t magic, multiboot_info_t* mbi) {
    isrInstall();
    irqInstall();
    installPaging();
    setVideoMode(WHITE_ON_BLACK);
    clearScreen();
    
    if (magic != 0x2BADB002) {
      printMessage("[Kernel Init] Error: This OS must be started in a multiboot context.");
      return 1;
    }

    if (mbi->mods_count < 1) {
      printMessage("[Kernel Init] Error: There should be one provided module (initrd)");
      return 2;
    }

    if (!CHECK_FLAG(mbi->flags, 6)) {
      printMessage("[Kernel Init] Error: BIOS did not provide us with a memory map.");
      return 3;
    }

    
    multiboot_memory_map_t* largestAvailableRegion = 0;
    for (
        multiboot_memory_map_t* buf = (multiboot_memory_map_t*) mbi->mmap_addr;
        (uint32_t)buf < mbi->mmap_addr + mbi->mmap_length;
         buf = (multiboot_memory_map_t*)((uint32_t)buf + buf->size + sizeof(buf->size))
        ) {
      if (
          !(KERNEL_START <= buf->addr_low && KERNEL_END >= buf->addr_low + buf->len_low) &&
          buf->type == MULTIBOOT_MEMORY_AVAILABLE &&
          (!largestAvailableRegion || (buf->len_low > largestAvailableRegion->len_low && buf->len_high >= largestAvailableRegion->len_high))) {
        largestAvailableRegion = buf;
      }
    }

    if (!largestAvailableRegion) {
      printMessage("[Kernel Init] Error: No available memory.");
      return 3;
    }

    uint32_t memStart = largestAvailableRegion->addr_low;
    uint32_t memEnd = largestAvailableRegion->addr_low + largestAvailableRegion->len_low;
    if (memStart < KERNEL_END && memEnd > KERNEL_END) {
      //     |--- MEMORY ---|
      // -- KRNL --|
      memStart = KERNEL_END; 
    } else if (KERNEL_START < memEnd && KERNEL_START > memStart) {
      // |--- MEMORY ---|
      //         |-- KRNL --|
      memEnd = KERNEL_START;
    } else if (KERNEL_START > memStart && KERNEL_END < memEnd) {
      // |----- MEMORY -----|
      //    |-- KRNL --|
      uint32_t startGap = KERNEL_START - memStart;
      uint32_t endGap = memEnd - KERNEL_END;
      memStart = (startGap > endGap) * memStart + (endGap > startGap) * KERNEL_END;
      memEnd = (startGap > endGap) * KERNEL_START + (endGap > startGap) * memEnd;
    }

    printHex(memStart);

    if (CHECK_FLAG(mbi->flags, 7)) {
      printMessage("drives_length: ");
      printHex(mbi->drives_length);
      newline();
      printMessage("drives_addr: ");
      printHex(mbi->drives_addr);
      newline();
      newline();
    }

    if (0 && CHECK_FLAG(mbi->flags, 3)) {
      uint32_t i;
      multibootModule* m;
      tarHeader* initrdTar;
      for (
            i = 0, m = (multibootModule*) mbi->mods_addr;
            i < mbi->mods_count;
            i++, m++
          ) {
        initrdTar = (tarHeader*)m->start;
      }

      while (*(initrdTar->name)) {
        printMessage("name: ");
        printMessage(initrdTar->name);
        newline();

        char* tarContents = (char*)initrdTar + sizeof(tarHeader);
        //printMessage("contents: ");
        //printMessage(tarContents);
        // newline();

        uint32_t size = intFromTarOctal(initrdTar->size, 12);
        uint8_t blocks = (size/512) + 1;

        printMessage("size: ");
        printHex(size);

        newline();
        newline();
        initrdTar = (tarHeader*)(tarContents + 512 * blocks);
      }
    }
    //printMessage("Welcome to lizOS!");
    //newline();
    //shellInit();
    return 0;
}
