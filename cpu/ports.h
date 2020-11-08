#ifndef DRIVER_PORTS_H_
# define DRIVER_PORTS_H_

#include "types.h"

u8 inb(const u16 port);
void outb(const u16 port, const u8 cata);
void io_wait(void);

#endif