#ifndef CPU_IDT_H_
# define CPU_IDT_H_

#include "types.h"

#define GDT_CS_OFFSET 0x08

typedef struct {
    u16     offsetL; // Lowest 16b of the function address
    u16     selector; // Kernel code segment offset (0x08 generally since null segment is 8 bytes long)
    u8      zero; // Always 0
    u8      flags;
    u16     offsetH; // Highest 16b of the function address
} __attribute__((packed)) idtEntry;

void fillIdtGate(idtEntry* idt, u32 address);

#endif