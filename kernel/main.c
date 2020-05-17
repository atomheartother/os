#include "ports.h"

unsigned short getCursorPosition() {
    unsigned short pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= inb(0x3D5) << 8;
    return pos;
}

int main() {
    unsigned short cursorPos = getCursorPosition();
    int offsetFromVGA = cursorPos * 2;
    char *vga = 0xb8000;
    vga[offsetFromVGA] = 'X';
    vga[offsetFromVGA + 1] = 0x0f;
    return 0;
}