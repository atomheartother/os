#ifndef KMEM_H_
# define KMEM_H_

#include <stddef.h>

void memcpy(void *dest, const void* src, size_t n);
void memset(void *s, int c, size_t n);
void memmove(void *dest, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

void* kmalloc_ap(size_t size, size_t* phys);
void* kmalloc_p(size_t size, size_t* phys);
void* kmalloc_a(size_t size);
void* kmalloc(size_t size);

#endif