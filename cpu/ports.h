#ifndef DRIVER_PORTS_H_
# define DRIVER_PORTS_H_

#include <stdint.h>

unsigned char inb(const uint16_t port);
void outb(const uint16_t port, const unsigned char cata);
void io_wait(void);

#endif