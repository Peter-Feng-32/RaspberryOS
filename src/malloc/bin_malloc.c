#include "../../include/utils/types.h"
#include "../../include/malloc/bin_malloc.h"

static struct bin_list my_bin_list;


void bin_malloc_init() {
    for (int bin = 0; bin < NUM_BINS; bin++) {
        u32 bin_alloc_size = 1;
        for (int i = 0; i < bin + BIN_OFFSET; i++) {
            bin_alloc_size *= 2;
        }

        struct bin_info * low_bounds = ((void *) LOW_MEMORY + (PAGE_SIZE * NUM_PAGES_PER_BIN_INIT * bin));
        void * high_bounds = ((void *) LOW_MEMORY + (PAGE_SIZE * NUM_PAGES_PER_BIN_INIT * (bin + 1)));

        // Setup circular linked list with memory.
        my_bin_list.bin_ptrs[bin] = low_bounds;
        my_bin_list.bin_free_node_count[bin] = 1;

        struct bin_info * curr = low_bounds;
        curr->next = curr;
        curr->prev = curr;
        for (void * next = (low_bounds + bin_alloc_size + sizeof(struct bin_info)); next < high_bounds; next += bin_alloc_size + sizeof(struct bin_info)) {
            curr->next = next;
            ((struct bin_info *) next)->prev = curr;
            curr = next;
            my_bin_list.bin_free_node_count[bin]++;
        }
        curr->next = my_bin_list.bin_ptrs[bin];
    }
}

void * bin_malloc(u32 size) {
    u32 bin_num = -BIN_OFFSET;
    u32 temp = size;
    while(temp > 1) {
        temp = temp >> 1;
        bin_num++;
    }
    if(bin_num < 0 || bin_num > NUM_BINS - 1) {
        return (void *) -1; // Todo: make exception
    }
    if(my_bin_list.bin_free_node_count[bin_num] == 0) {
        return (void *) -1;
    }

    struct bin_info * curr_ptr = my_bin_list.bin_ptrs[bin_num];
    curr_ptr->prev->next = curr_ptr->next;
    curr_ptr->next->prev = curr_ptr->prev;
    my_bin_list.bin_ptrs[bin_num] = curr_ptr->next;
    my_bin_list.bin_free_node_count[bin_num]--;

    curr_ptr->prev = curr_ptr;
    curr_ptr->next = curr_ptr;
    return (void *) curr_ptr + sizeof(struct bin_info);
}

void bin_free(void * ptr) {
    struct bin_info * curr_info = ptr - sizeof(struct bin_info);

    if (my_bin_list.bin_free_node_count[curr_info->bin_num] == 0) {
        my_bin_list.bin_ptrs[curr_info->bin_num] = curr_info;
        curr_info->prev = curr_info;
        curr_info->next = curr_info;
    } else if (my_bin_list.bin_free_node_count[curr_info->bin_num] == 1) { 
        my_bin_list.bin_ptrs[curr_info->bin_num]->prev = curr_info;
        my_bin_list.bin_ptrs[curr_info->bin_num]->next = curr_info;
        curr_info->prev = my_bin_list.bin_ptrs[curr_info->bin_num];
        curr_info->next = my_bin_list.bin_ptrs[curr_info->bin_num];
    } else {
        curr_info->prev = my_bin_list.bin_ptrs[curr_info->bin_num]->prev;
        curr_info->prev->next = curr_info;
        my_bin_list.bin_ptrs[curr_info->bin_num]->prev = curr_info;
        curr_info->next = my_bin_list.bin_ptrs[curr_info->bin_num];
    }
    my_bin_list.bin_free_node_count[curr_info->bin_num]++;
}
