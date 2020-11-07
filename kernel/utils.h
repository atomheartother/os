#ifndef KERNEL_UTILS_H_
# define KERNEL_UTILS_H_

#include "types.h"

void os_memcpy(i8 *dest, const i8* src, u32 n);
void int_to_ascii(int n, char str[]);

#endif