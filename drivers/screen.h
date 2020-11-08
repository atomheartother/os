#ifndef DRIVER_SCREEN_H_
# define DRIVER_SCREEN_H_

#include <stdint.h>

# define VGA_ADDRESS (char*)(0xb8000)
# define MAX_ROWS 25
# define MAX_COLS 80
# define VGA_END_ADDRESS (VGA_ADDRESS + (MAX_ROWS * MAX_COLS * 2))

# define WHITE_ON_BLACK 0x0f

void printChar(const unsigned char c);
void printMessage(const char* message);
void printMessageAt(const char* message, uint32_t row, uint32_t col);
void newline(void);
void clearScreen(void);
void backspace(void);

#endif