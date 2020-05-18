#include "screen.h"

int main() {
    clearScreen();
    for (int i=0; i < 512 ; i++) {
        printMessage("Hello, world!");
    }
}