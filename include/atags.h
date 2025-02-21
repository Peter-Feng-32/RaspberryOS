#include "types.h"

#ifndef _ATAGS_H
#define _ATAGS_H

#define ATAG_CORE_VALUE 0x54410001
#define ATAG_NONE_VALUE 0
#define ATAG_MEM_VALUE 0x54410002
#define ATAG_CMDLINE_VALUE 0x54410009

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

union atag_data {
    struct atag_core core;
    struct atag_none none;
    struct atag_mem mem;
    struct atag_cmdline cmdline;
};

struct atag{
    u32 dwords;
    u32 tag;
    union atag_data data;
};


#endif