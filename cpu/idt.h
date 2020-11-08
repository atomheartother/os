#ifndef CPU_IDT_H_
# define CPU_IDT_H_

#include <stdint.h>

#define GDT_CS_OFFSET 0x08

typedef struct {
    uint16_t     offsetL; // Lowest 16b of the function address
    uint16_t     selector; // Kernel code segment offset (0x08 generally since null segment is 8 bytes long)
    unsigned char      zero; // Always 0
    unsigned char      flags;
    uint16_t     offsetH; // Highest 16b of the function address
} __attribute__((packed)) idtEntry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtRegister;

#define IDT_ENTRIES 256

// Tutorials mention needing these as globals but that hasn't been necessary so far...
// extern idtEntry idt[IDT_ENTRIES];
// extern idtRegister idtReg;

void setIdtGate(uint16_t n, uint32_t address);
void initIdt();

#endif