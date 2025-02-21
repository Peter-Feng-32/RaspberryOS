#include "../include/atags.h"


void print_atag(struct atag* tag) {
    
}

void print_atags(struct atag* tag) {
    struct atag * curr_tag = tag;
    while (curr_tag->tag != ATAG_NONE_VALUE) {
        print_atag(curr_tag);
        curr_tag = (struct atag *) (((u32 *) tag) + tag->dwords);
    }

}