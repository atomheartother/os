#ifndef DRIVER_SCREEN_H_
# define DRIVER_SCREEN_H_

# define VGA_ADDRESS 0xb8000
# define MAX_ROWS 25
# define MAX_COLS 80

void printMessage(const char* message);
void printMessageAt(const char* message, unsigned row, unsigned col);
void clearScreen(void);

#endif