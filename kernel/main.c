#include "paging.h"
#include "screen.h"
#include "isr.h"
#include "shell.h"

int kernel_main() {
    clearScreen();
    isrInstall();
    irqInstall();
    installPaging();
    printMessage("Welcome to lizOS!");
    newline();
    shellInit();
    return 0;
}
