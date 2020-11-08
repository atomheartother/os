#include "keyboard.h"
#include "isr.h"
#include "ports.h"
#include "kstring.h"
#include "screen.h"

static void keyboardCallback(interruptRegisters r) {
    u8 scancode = inb(0x60);
    char letter[2];
    int_to_ascii(scancode, letter);
    printMessage("Keyboard scancode: ");
    printMessage(letter);
    newline();
}

void init_keyboard() {
    registerIsrCallback(IRQ1, keyboardCallback);
}