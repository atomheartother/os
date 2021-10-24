#include "screen.h"
#include "ports.h"
#include "kmem.h"

/* 
 * Private function prototypes
 */
static uint16_t getCursorOffset(void);
static void setCursorOffset(uint32_t offset);
static void printCharAtOffset(char c, uint16_t offset, char properties);
static void printAtOffset(const char* message, uint16_t offset, char properties);
static void getCoordsFromOffset(uint32_t offset, unsigned char* row, unsigned char* col);
static uint16_t getOffsetFromCoords(unsigned char row, unsigned char col);
static void printCharAtAddress(char **address, char c, char properties);

/* 
 * Public functions
 */
void printMessage(const char* message) {
    printAtOffset(message, getCursorOffset(), WHITE_ON_BLACK);
}

void printChar(const unsigned char c) {
    printCharAtOffset(c, getCursorOffset(), WHITE_ON_BLACK);
}

void printMessageAt(const char* message, uint32_t row, uint32_t col) {
    const uint16_t offset = getOffsetFromCoords((unsigned char)(row & 0xFF), (unsigned char)(col & 0xFF));
    printAtOffset(message, offset, 0x0f);
}

void printMessageOverLine(const char* message) {
  unsigned char row;
  unsigned char col;
  getCoordsFromOffset(getCursorOffset(), &row, &col);
  printMessageAt(message, row, 0);
  getCoordsFromOffset(getCursorOffset(), &row, &col);
  const unsigned baseOffset = getOffsetFromCoords(row, col);
  unsigned offset = baseOffset;
  unsigned maxOffset = getOffsetFromCoords(row + 1, 0);
  while (offset < maxOffset) {
    printCharAtOffset(' ', offset, WHITE_ON_BLACK);
    offset += 2;
  }
  setCursorOffset(baseOffset);
}

void clearScreen(void) {
    const uint32_t bufferSize = MAX_COLS * MAX_ROWS;
    uint32_t index = 0;
    while (index < bufferSize) {
        printCharAtOffset(' ', index * 2, WHITE_ON_BLACK);
        index += 1;
    }
    setCursorOffset(0);
}

void newline(void) {
    unsigned char row;
    unsigned char col;
    getCoordsFromOffset(getCursorOffset(), &row, &col);
    while (col < MAX_COLS) {
        printCharAtOffset(' ', getCursorOffset(), WHITE_ON_BLACK);
        col += 1;
    }
}

void backspace(void) {
    const uint16_t offset = getCursorOffset();
    if (offset < 2) return;
    char* vgaMemory = VGA_ADDRESS + offset - 2;
    *vgaMemory = ' ';
    setCursorOffset((uint32_t)(vgaMemory - VGA_ADDRESS));
}

/* 
 * Private funtions
 */
static uint16_t getCursorOffset(void) {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= inb(0x3D5) << 8;
    // * 2 because each "pixel" in the position is 2 bytes
    return pos * 2;
}

static void setCursorOffset(uint32_t offset) {
    offset /= 2;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(offset & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((offset >> 8) & 0xFF));
}

static void printCharAtAddress(char **address, char c, char properties) {
    *(*address) = c;
    *(*(address + 1)) = properties;
    if (*address + 2 < VGA_END_ADDRESS) {
        *address = *address + 2;
    } else {
        // Scroll up and put the cursor back to the start of the last line.
        memcpy(VGA_ADDRESS, VGA_ADDRESS + MAX_COLS * 2, VGA_END_ADDRESS - (VGA_ADDRESS + MAX_COLS * 2));
        *address = VGA_END_ADDRESS - (MAX_COLS * 2);
        for (char* ptr = *address ; ptr < VGA_END_ADDRESS ; ptr += 2) {
            *ptr = ' ';
            *(ptr + 1) = WHITE_ON_BLACK;
        }
    }
}

static void printCharAtOffset(char c, uint16_t offset, char properties) {
    char* vgaMemory = VGA_ADDRESS + offset;
    printCharAtAddress(&vgaMemory, c, properties);
    setCursorOffset((uint32_t)(vgaMemory - VGA_ADDRESS));
}

static void printAtOffset(const char* message, uint16_t offset, char properties) {
    char* vgaMemory = VGA_ADDRESS + offset;
    while (*message) {
        printCharAtAddress(&vgaMemory, *message, properties);
        message++;
    }
    setCursorOffset((uint32_t)(vgaMemory - VGA_ADDRESS));
}

static void getCoordsFromOffset(uint32_t offset, unsigned char* row, unsigned char* col) {
    *row = offset / (MAX_COLS * 2);
    *col = (offset % (MAX_COLS * 2)) / 2;
}

static uint16_t getOffsetFromCoords(unsigned char row, unsigned char col) {
    return row * MAX_COLS * 2 + col * 2;
}
