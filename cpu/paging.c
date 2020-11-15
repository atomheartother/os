#include "paging.h"
#include "kstring.h"
#include "screen.h"
#include <stdint.h>

void installPaging(void) {
    pageDirectoryEntry pageDirectory[1024]  __attribute__((aligned(4096)));
    pageTableEntry firstPageTable[1024] __attribute__((aligned(4096)));
    const pageDirectoryEntry emptyPage = { .writeable = 1 };
    for (unsigned i = 0 ; i < 1024 ; i += 1) {
        pageDirectory[i] = emptyPage;
    }
    for (unsigned i = 0 ; i < 1024 ; i += 1) {
        // Set i as the address, set the page to present & read/write
        const uint32_t field = (i * 0x1000) | 3;
        firstPageTable[i] = *(pageTableEntry*)&field;
        
    }
    pageDirectory[0].address = (uint32_t)firstPageTable >> 12;
    pageDirectory[0].present = 1;
    initPaging(pageDirectory);
    return;
}