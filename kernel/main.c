#include "screen.h"
#include "isr.h"

int main() {
    clearScreen();
    isrInstall();
    __asm__ __volatile__("int $2");
}