#include "paging.h"
#include "screen.h"
#include "isr.h"
#include "shell.h"
#include "multiboot.h"
#include "debug.h"
#include "tar.h"

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
    tarHeader* initrdTar;
    for (
          i = 0, m = (multibootModule*) mbi->mods_addr;
          i < mbi->mods_count;
          i++, m++
        ) {
      initrdTar = (tarHeader*)m->start;
    }

    uint32_t fileIdx=0;
    while (*(initrdTar->name)) {
      printMessage("initrd file ");
      printHex(fileIdx);
      newline();

      printMessage("name: ");
      printMessage(initrdTar->name);
      newline();

      char* tarContents = (char*)initrdTar + sizeof(tarHeader);
      printMessage("contents: ");
      printMessage(tarContents);
      newline();
      newline();
      initrdTar = (tarHeader*)(tarContents + 512);
      fileIdx+=1;
    }
    //printMessage("Welcome to lizOS!");
    //newline();
    //shellInit();
    return 0;
}
