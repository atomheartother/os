#include "screen.h"

/*
 * This module is handy utility functions to debug stuff
 */

char hexDict[] = "0123456789ABCDEF";

void printHex(uint32_t n) {
  char x[11];
  x[0] = '0';
  x[1] = 'x';
  x[10] = 0;

  for (int i=4; i>0 ; i-=1) {
    uint8_t part = n % 0x100;


    x[(i*2)+1] = hexDict[part % 0x10];
    x[i*2] = hexDict[part / 0x10];

    n /= 0x100;
  }
  printMessage(x);
}

