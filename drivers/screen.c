#include "screen.h"
#include "ports.h"
#include "utils.h"

/* 
 * Private function prototypes
 */
u16 getCursorOffset(void);
void setCursorOffset(u32 offset);
void printCharAtOffset(i8 c, u16 offset, i8 properties);
void printAtOffset(const i8* message, u16 offset, i8 properties);
void getCoordsFromOffset(u32 offset, u8* row, u8* col);
u16 getOffsetFromCoords(u8 row, u8 col);
void printCharAtAddress(i8 **address, i8 c, i8 properties);

/* 
 * Public functions
 */
void printMessage(const i8* message) {
    printAtOffset(message, getCursorOffset(), 0x0f);
}

void printMessageAt(const i8* message, u32 row, u32 col) {
    const u16 offset = getOffsetFromCoords((u8)(row & 0xFF), (u8)(col & 0xFF));
    printAtOffset(message, offset, 0x0f);
}

void clearScreen(void) {
    const u32 bufferSize = MAX_COLS * MAX_ROWS;
    u32 index = 0;
    while (index < bufferSize) {
        printCharAtOffset(' ', index * 2, WHITE_ON_BLACK);
        index += 1;
    }
    setCursorOffset(0);
}

/* 
 * Private funtions
 */
u16 getCursorOffset(void) {
    u16 pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= inb(0x3D5) << 8;
    // * 2 because each "pixel" in the position is 2 bytes
    return pos * 2;
}

void setCursorOffset(u32 offset) {
    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(offset & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((offset >> 8) & 0xFF));
}

void printCharAtAddress(i8 **address, i8 c, i8 properties) {
    *(*address) = c;
    *(*(address + 1)) = properties;
    if (*address + 2 < VGA_END_ADDRESS) {
        *address = *address + 2;
    } else {
        // Scroll up and put the cursor back to the start of the last line.
        os_memcpy(VGA_ADDRESS, VGA_ADDRESS + MAX_COLS * 2, VGA_END_ADDRESS - (VGA_ADDRESS + MAX_COLS * 2));
        *address = VGA_END_ADDRESS - (MAX_COLS * 2);
        for (i8* ptr = *address ; ptr < VGA_END_ADDRESS ; ptr += 2) {
            *ptr = ' ';
            *(ptr + 1) = WHITE_ON_BLACK;
        }
    }
}

void printCharAtOffset(i8 c, u16 offset, i8 properties) {
    i8* vgaMemory = VGA_ADDRESS + offset;
    printCharAtAddress(&vgaMemory, c, properties);
    setCursorOffset((u32)(vgaMemory - VGA_ADDRESS) / 2);
}

void printAtOffset(const i8* message, u16 offset, i8 properties) {
    i8* vgaMemory = VGA_ADDRESS + offset;
    while (*message) {
        printCharAtAddress(&vgaMemory, *message, properties);
        message++;
    }
    setCursorOffset((u32)(vgaMemory - VGA_ADDRESS) / 2);
}

void getCoordsFromOffset(u32 offset, u8* row, u8* col) {
    *row = offset / (MAX_COLS * 2);
    *col = (offset % (MAX_COLS * 2)) / 2;
}

u16 getOffsetFromCoords(u8 row, u8 col) {
    return row * MAX_COLS * 2 + col * 2;
}