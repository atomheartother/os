#ifndef DRIVER_SCREEN_H_
# define DRIVER_SCREEN_H_

#include "types.h"

# define VGA_ADDRESS (i8*)(0xb8000)
# define MAX_ROWS 25
# define MAX_COLS 80
# define VGA_END_ADDRESS (VGA_ADDRESS + (MAX_ROWS * MAX_COLS * 2))

# define WHITE_ON_BLACK 0x0f

void printChar(const u8 c);
void printMessage(const i8* message);
void printMessageAt(const i8* message, u32 row, u32 col);
void newline(void);
void clearScreen(void);
void backspace(void);

#endif