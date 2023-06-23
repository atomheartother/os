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
      printMessage("Error: This OS must be started in a multiboot context.");
      return 1;
    }

    if (mbi->mods_count < 1) {
      printMessage("Error: There should be one provided module (initrd)");
      return 2;
    }


    printMessage("flags: ");
    printHex(mbi->flags);
    newline();
    printMessage("memLwr: ");
    printHex(mbi->mem_lower);
    newline();
    printMessage("memUpr: ");
    printHex(mbi->mem_upper + 1000);
    newline();
    printMessage("kStart: ");
    printHex((uint32_t)&_kernelStart);
    newline();
    printMessage("kEnd:   ");
    printHex((uint32_t)&_kernelEnd);
    newline();
    newline();
    if (CHECK_FLAG(mbi->flags, 6)) {
      printMessage("mmap length: ");
      printHex(mbi->mmap_length);
      newline();
      printMessage("mmap addr: ");
      printHex(mbi->mmap_addr);
      newline();
      newline();
    }
    if (CHECK_FLAG(mbi->flags, 7)) {
      printMessage("drives_length: ");
      printHex(mbi->drives_length);
      newline();
      printMessage("drives_addr: ");
      printHex(mbi->drives_addr);
      newline();
      newline();
    }

    if (CHECK_FLAG(mbi->flags, 3)) {
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
