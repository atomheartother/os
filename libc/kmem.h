#ifndef KMEM_H_
# define KMEM_H_

#include <stdint.h>

void os_memcpy(char *dest, const char* src, uint32_t n);

#endif