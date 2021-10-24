#include "keyboard.h"
#include "isr.h"
#include "ports.h"
#include "kstring.h"
#include "shell.h"

#define SC_COUNT 58

#define BACKSPACE 0x0E
#define ENTER 0x1C

const char sc_ascii[SC_COUNT] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
    'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 
    'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 
    'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};


static void keyboardCallback() {
    unsigned char scancode = inb(0x60);
    if (scancode >= SC_COUNT) return;
    if (scancode == ENTER) {
        shellRun();
    } else if (scancode == BACKSPACE) {
        shellBackspace();
    } else {
        shellInput(sc_ascii[scancode]);
    }
}

void init_keyboard() {
    registerIsrCallback(IRQ1, keyboardCallback);
}
