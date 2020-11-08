#include "ports.h"

unsigned char inb(const uint16_t port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void outb(const uint16_t port, const unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}