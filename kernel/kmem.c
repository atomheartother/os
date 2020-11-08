#include "kmem.h"

void os_memcpy(i8 *dest, const i8* src, u32 n) {
    while (n--) {
        *(dest++) = *(src++);
    }
}
