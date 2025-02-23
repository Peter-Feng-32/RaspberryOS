#include "../utils/types.h"
#include "../mm.h"

#ifndef _MALLOC_H
#define _MALLOC_H

/*
Bin list allocator
Bins of size 2^3 to 2^16

Each bin contains a circular linked list.
Deciding boundaries - equal (total) amounts of memory per bin.

Consider pages. x pages per bin.  This way we can work with virtual memory later.

Pages are 4kb in size.
Bin node contain a pointer to next bin node

Does not handle allocating more than 2^16 bytes at a time currently.

Init:
Start at low memory
Allocate 10 pages per bin

Malloc: Return the next free node from the correct bin
    - Set prev-> next to next
    Set next->prev to prev
    set bin_ptr[i] to next

Free: Add the pointec node to the correct bin
*/

#define NUM_BINS 13
#define NUM_PAGES_PER_BIN_INIT 1000
#define BIN_OFFSET 3

struct bin_info {
    u32 bin_num;
    struct bin_info * next;
    struct bin_info * prev;
};

struct bin_list {
    struct bin_info * bin_ptrs[NUM_BINS];
    u32 bin_free_node_count[NUM_BINS];
};

void bin_malloc_init();
void * bin_malloc(u32 size);
void bin_free(void * ptr);

#endif

