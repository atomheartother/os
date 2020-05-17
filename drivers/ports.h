#ifndef DRIVER_PORTS_H_
# define DRIVER_PORTS_H_

unsigned char inb(const unsigned short port);
void outb(const unsigned short port, const unsigned char cata);

#endif