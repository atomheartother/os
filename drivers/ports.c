unsigned char inb(const unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void outb(const unsigned short port, const unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}