#ifndef DRIVER_SCREEN_H_
# define DRIVER_SCREEN_H_

# define VGA_ADDRESS (char*)(0xb8000)
# define MAX_ROWS 25
# define MAX_COLS 80
# define VGA_END_ADDRESS VGA_ADDRESS + (MAX_ROWS * MAX_COLS * 2)

void printMessage(const char* message);
void printMessageAt(const char* message, unsigned row, unsigned col);
void clearScreen(void);

#endif