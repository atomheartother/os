#include "keyboard.h"
#include "isr.h"
#include "ports.h"
#include "kstring.h"
#include "screen.h"

#define SC_COUNT 58

#define BACKSPACE 0x0E
#define ENTER 0x1C

const char sc_ascii[SC_COUNT] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
    'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
    'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
    'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};


static void keyboardCallback() {
    unsigned char scancode = inb(0x60);
    if (scancode >= SC_COUNT) return;
    if (scancode == ENTER) {
        newline();
    } else if (scancode == BACKSPACE) {
        backspace();
    } else {
        printChar(sc_ascii[scancode]);
    }
}

void init_keyboard() {
    registerIsrCallback(IRQ1, keyboardCallback);
}