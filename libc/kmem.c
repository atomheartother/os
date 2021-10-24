#include <stddef.h>
#include "kmem.h"

size_t placementAddress = 0;

void memcpy(void *dest, const void* src, size_t n) {
    while (n--) {
        *((char*)dest++) = *((char*)src++);
    }
}

// This function is guaranteed to function byte per byte
// Right now it's the same as memcpy, but we could
// optimize memcpy later
static void fwdByteCopy(void *dest, const void* src, size_t n) {
    memcpy(dest, src, n);
}

// Copies byte per byte, backwards
static void bckByteCopy(void *dest, const void* src, size_t n) {
    while (n--) {
        *((char*)dest + n) = *((char*)src + n);
    }
}

void memmove(void *dest, const void* src, size_t n) {
    if (dest == src || !n) return;
    if ((dest > src  && (size_t)(dest - src) >= n) ||
        (dest < src && (size_t)(src - dest) >= n)) {
        // No overlap
        memcpy(dest, src, n);
    } else if (dest > src) {
        bckByteCopy(dest, src, n);
    } else {
        fwdByteCopy(dest, src, n);
    }
}

void memset(void *s, int c, size_t n) {
    while (n--) {
        *((char*)s++) = (char)c;
    }
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char *p1 = s1, *p2 = s2;
    while (n-- > 0) {
        if (*p1++ != *p2++) {
            return *--p1 - *--p2;
        }
    }
    return 0;
}

static void* kmalloc_internal(size_t size, int align, size_t* phys) {
    if (align && (placementAddress & 0x00000FFF)) {
        placementAddress &= 0xFFFFF000;
        placementAddress += 0x1000;
    }
    size_t tmp = placementAddress;
    if (phys) {
        *phys = tmp;
    }
    placementAddress += size;
    return (void*)tmp;
}

void* kmalloc_ap(size_t size, size_t* phys) {
    return kmalloc_internal(size, 1, phys);
}

void* kmalloc_p(size_t size, size_t* phys) {
    return kmalloc_internal(size, 0, phys);
}

void* kmalloc_a(size_t size) {
    return kmalloc_internal(size, 1, 0);
}

void* kmalloc(size_t size) {
    return kmalloc_internal(size, 0, 0);
}
