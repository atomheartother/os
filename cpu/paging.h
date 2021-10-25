#ifndef PAGING_H_
# define PAGING_H_

#include <stdint.h>

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a >> 5) // We divide by 0x20
#define OFFSET_FROM_BIT(a) (a % 0x20)

// Represents a page table entry. A page table is made up of 1024 page table entries.
// Values from https://wiki.osdev.org/Paging#Page_Table
typedef struct {
    unsigned int present     : 1; // This page is present in memory
    unsigned int writeable   : 1; // 1 if R/W, 0 if readonly
    unsigned int usermode    : 1; // 1 if usermode page, 0 if only accessible by supervisors
    unsigned int writethrough: 1; // Write-through caching is enabled
    unsigned int disablecache: 1; // If set, the page will not be cached
    // Whether a page has been read or written to.
    // This bit will not be cleared by the CPU, so that burden falls on the OS (if it needs this bit at all)
    unsigned int accessed    : 1;
    unsigned int dirty       : 1; // Page has been written to. This flag is not updated by the CPU, and once set will not unset itself. 
    unsigned int pat         : 1; // Should be always 0 unless PAT is supported
    unsigned int global      : 1; // Prevents the TLB from updating the address in its cache if CR3 is reset.
    unsigned int available   : 3; // Free space for the OS
    unsigned int address     : 20; // The actual address, 4kB aligned
}  pageTableEntry;

typedef union {
  uint32_t intValue;
  pageTableEntry entry;
} pageTableEntryUnion;

// Represents a page directory entry. A page directory is made up of 1024 page directory entries.
// Values from https://wiki.osdev.org/Paging#Page_Directory
typedef struct {
    unsigned int present     : 1; // This page is present in memory
    unsigned int writeable   : 1; // 1 if R/W, 0 if readonly
    unsigned int usermode    : 1; // 1 if usermode page, 0 if only accessible by supervisors
    unsigned int writethrough: 1; // Write-through caching is enabled
    unsigned int disablecache: 1; // If set, the page will not be cached
    // Whether a page has been read or written to.
    // This bit will not be cleared by the CPU, so that burden falls on the OS (if it needs this bit at all)
    unsigned int accessed    : 1;
    unsigned int zero        : 1; // This is always 0
    unsigned int size        : 1; // Pages of this entry are 4MB big, not 4kB. Requires PSE to be enabled 
    unsigned int ignored     : 1; // This bit is ignored
    unsigned int available   : 3; // Free space for the OS
    unsigned int address     : 20; // The page table's physical address, 4kB aligned
}   pageDirectoryEntry;

void installPaging(void);
void initPaging(pageDirectoryEntry *pageDirectory);

#endif
