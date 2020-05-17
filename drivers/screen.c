#include "screen.h"
#include "ports.h"

/* 
 * Private function prototypes
 */
unsigned short getCursorOffset(void);
void setCursorOffset(unsigned offset);
void printCharAtOffset(const char c, unsigned short offset, char properties);
void printAtOffset(const char* message, unsigned short offset, char properties);
void getCoordsFromOffset(unsigned offset, unsigned char* row, unsigned char* col);
unsigned short getOffsetFromCoords(unsigned char row, unsigned char col);

/* 
 * Public functions
 */
void printMessage(const char* message) {
    printAtOffset(message, getCursorOffset(), 0x0f);
}

void printMessageAt(const char* message, unsigned row, unsigned col) {
    const unsigned short offset = getOffsetFromCoords((unsigned char)(row & 0xFF), (unsigned char)(col & 0xFF));
    printAtOffset(message, offset, 0x0f);
}

void clearScreen(void) {
    const unsigned bufferSize = MAX_COLS * MAX_ROWS;
    unsigned index = 0;
    while (index < bufferSize) {
        printCharAtOffset(' ', index * 2, 0x0f);
        index += 1;
    }
    setCursorOffset(0);
}

/* 
 * Private funtions
 */
unsigned short getCursorOffset(void) {
    unsigned short pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= inb(0x3D5) << 8;
    // * 2 because each "pixel" in the position is 2 bytes
    return pos * 2;
}

void setCursorOffset(unsigned offset) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(offset & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((offset >> 8) & 0xFF));
}

void printCharAtOffset(const char c, unsigned short offset, char properties) {
    char* vgaMemory = (char*)VGA_ADDRESS + offset;
    *vgaMemory = c;
    *(vgaMemory + 1) = properties;
    setCursorOffset((unsigned)(vgaMemory - VGA_ADDRESS) / 2);
}

void printAtOffset(const char* message, unsigned short offset, char properties) {
    char* vgaMemory = (char*)VGA_ADDRESS + offset;
    while (*message) {
        *vgaMemory = *message;
        *(vgaMemory + 1) = properties;
        vgaMemory += 2;
        message++;
    }
    setCursorOffset((unsigned)(vgaMemory - VGA_ADDRESS) / 2);
}

void getCoordsFromOffset(unsigned offset, unsigned char* row, unsigned char* col) {
    *row = offset / (MAX_COLS * 2);
    *col = (offset % (MAX_COLS * 2)) / 2;
}

unsigned short getOffsetFromCoords(unsigned char row, unsigned char col) {
    return row * MAX_COLS * 2 + col * 2;
}