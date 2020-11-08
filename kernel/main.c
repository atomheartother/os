#include "screen.h"
#include "isr.h"

int kernel_main() {
    clearScreen();
    isrInstall();
    __asm__ __volatile__("sti");
    irqInstall();
    return 0;
}