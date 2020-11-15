#include "paging.h"
#include "screen.h"
#include "isr.h"

int kernel_main() {
    clearScreen();
    isrInstall();
    irqInstall();
    installPaging();
    printMessage("Hello from paging world");
    return 0;
}