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

typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idtRegister;

#define IDT_ENTRIES 256

extern idtEntry idt[IDT_ENTRIES];
extern idtRegister idtReg;

void fillIdtGate(idtEntry* idt, u32 address);
void initIdt();

#endif