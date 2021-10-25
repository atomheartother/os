#include "paging.h"
#include "screen.h"
#include "isr.h"
#include "shell.h"

int kernel_main() {
    isrInstall();
    irqInstall();
    installPaging();
    setVideoMode(WHITE_ON_BLACK);
    clearScreen();
    printMessage("Welcome to lizOS!");
    newline();
    shellInit();
    return 0;
}
