#include "paging.h"
#include "screen.h"
#include "isr.h"
#include "shell.h"
#include "multiboot.h"
#include "debug.h"

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

    uint32_t i;
    multibootModule* m;
    for (
          i = 0, m = (multibootModule*) mbi->mods_addr;
          i < mbi->mods_count;
          i++, m++
        ) {
      printMessage("Module starting at ");
      printHex(m->start);
      printMessage(", ending at ");
      printHex(m->end);
      if (m->str) {
        printMessage(". Str: ");
        printMessage((char*) m->str);
      }
      newline();
      printMessage("First string: ");
      printMessage((char*) m->start);
      newline();
    }

    //printMessage("Welcome to lizOS!");
    //newline();
    //shellInit();
    return 0;
}
