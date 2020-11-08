#include "screen.h"
#include "isr.h"

int kernel_main() {
    clearScreen();
    isrInstall();
    irqInstall();
    return 0;
}