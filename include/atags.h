#include "types.h"

#ifndef _ATAGS_H
#define _ATAGS_H


struct atag_core {
    u32 flags;              /* bit 0 = read-only */
    u32 pagesize;           /* systems page size (usually 4k) */
    u32 rootdev;            /* root device number */
};

struct atag_none {

};

struct atag_mem {
        u32     size;   /* size of the area */
        u32     start;  /* physical start address */
};

struct atag_cmdline {
        char    cmdline[1];     /* this is the minimum size */
};

union Data {
    atag_core core;
    atag_none none;
    atag_mem mem;
    atag_cmdline cmdline;
};

struct ATag{
    u32 dwords;
    u32 tag;
    Data data;
};


#endif