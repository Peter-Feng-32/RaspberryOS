#include "../include/atags.h"
#include "../include/io.h"
#include "../include/shell.h"
#include "../include/strings.h"


void print_atag(sender_func_type sender, struct atag* tag) {
    strwrite(sender, "Tag is type ");
    if(tag->tag == ATAG_CORE_VALUE) {
        strwrite(sender, "core");
    } else if (tag->tag == ATAG_NONE_VALUE) {
        strwrite(sender, "none");
    } else if (tag->tag == ATAG_MEM_VALUE) {
        strwrite(sender, "mem");
    } else if (tag->tag == ATAG_CMDLINE_VALUE) {
        strwrite(sender, "cmdline");
    }
    strwrite(sender, "\r\n");
}



void print_atags(sender_func_type sender, struct atag* tag) {
    struct atag * curr_tag = tag;
    while (curr_tag->tag != ATAG_NONE_VALUE) {
        print_atag(sender, curr_tag);
        curr_tag = (struct atag *) (((u32 *) tag) + tag->dwords);
    }

}