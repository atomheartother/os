#include "paging.h"
#include "screen.h"
#include "isr.h"
#include "shell.h"
#include "multiboot.h"
#include "debug.h"

int kernel_main(uint32_t magic, multiboot_info_t* mbinfo) {
    isrInstall();
    irqInstall();
    installPaging();
    setVideoMode(WHITE_ON_BLACK);
    clearScreen();
    printMessage("GRUB magic: ");
    printHex(magic);
    newline();
    printMessage("Number of modules: ");
    printHex(mbinfo->mods_count);
    newline();
    printMessage("Module address: ");
    printHex(mbinfo->mods_addr);
    newline();
    uint32_t mainAddress = (uint32_t)kernel_main;
    printMessage("Main address: ");
    printHex(mainAddress);
    newline();
    //printMessage("Welcome to lizOS!");
    //newline();
    //shellInit();
    return 0;
}
