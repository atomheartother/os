#include "kmem.h"

size_t placementAddress = 0;

void memcpy(void *dest, const void* src, size_t n) {
    while (n--) {
        *((char*)dest++) = *((char*)src++);
    }
}

static void* kmalloc_internal(size_t size, int align, size_t* phys) {
    if (align) {
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