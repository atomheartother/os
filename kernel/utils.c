#include "utils.h"

void os_memcpy(char *dest, const char* src, unsigned n) {
    while (n--) {
        *(dest++) = *(src++);
    }
}