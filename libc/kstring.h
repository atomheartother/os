#ifndef KSTRING_H_
# define KSTRING_H_

#include <stdint.h>

uint32_t strlen(char s[]);
void reverse(char s[]);
void int_to_ascii(int n, char str[]);
void uint_to_ascii(uint32_t n, char str[]);

#endif
