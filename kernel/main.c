#include "screen.h"
#include "isr.h"
#include "timer.h"

int main() {
    clearScreen();
    isrInstall();
    __asm__ __volatile__("sti");
    init_timer(50);
}