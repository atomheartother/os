#include "kmem.h"

void os_memcpy(char *dest, const char* src, uint32_t n) {
    while (n--) {
        *(dest++) = *(src++);
    }
}
