#ifndef DRIVER_SCREEN_H_
# define DRIVER_SCREEN_H_

#include <stdint.h>

# define VGA_ADDRESS (char*)(0xb8000)
# define MAX_ROWS 25
# define MAX_COLS 80
# define VGA_END_ADDRESS (VGA_ADDRESS + (MAX_ROWS * MAX_COLS * 2))

# define FG_Black 0
# define FG_Blue 1
# define FG_Green 2
# define FG_Cyan 3
# define FG_Red 4
# define FG_Magenta 5
# define FG_Brown 6
# define FG_LightGray 7
# define FG_BRIGHT 8

# define BG_Black (0 << 4)
# define BG_Blue (1 << 4)
# define BG_Green (2 << 4)
# define BG_Cyan (3 << 4)
# define BG_Red (4 << 4)
# define BG_Magenta (5 << 4)
# define BG_Brown (6 << 4)
# define BG_LightGray (7 << 4)
# define BG_BLINK (8 << 4)

# define WHITE_ON_BLACK (FG_BRIGHT | FG_LightGray)

// Print a single char to terminal and advance the cursor
void printChar(const unsigned char c);
// Remove the latest character and move the cursor back one
void backspace(void);
// Print a message to terminal and advance the cursor after the message
void printMessage(const char* message);
// Print a message over the current line, then put the cursor after the printed message
// This is useful for shells.
void printMessageOverLine(const char* message);
// Print a message at a specific row/col location, for overlays maybe?
void printMessageAt(const char* message, uint32_t row, uint32_t col);
// Print a newline
void newline(void);
// Clear the screen completely
void clearScreen(void);

void setVideoMode(char mode);
char getVideoMode();

#endif
