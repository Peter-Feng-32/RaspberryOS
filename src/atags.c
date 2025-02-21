#include "../include/atags.h"
#include "../include/io.h"
#include "../include/shell.h"


void print_atag(string_sender_func_type sender, struct atag* tag) {
    sender("Tag is type ");
    if(tag->tag == ATAG_CORE_VALUE) {
        sender("core");
    } else if (tag->tag == ATAG_NONE_VALUE) {
        sender("none");
    } else if (tag->tag == ATAG_MEM_VALUE) {
        sender("mem");
    } else if (tag->tag == ATAG_CMDLINE_VALUE) {
        sender("cmdline");
    }
    sender("\r\n");
}

void print_atags(string_sender_func_type sender, struct atag* tag) {
    struct atag * curr_tag = tag;
    while (curr_tag->tag != ATAG_NONE_VALUE) {
        print_atag(sender, curr_tag);
        curr_tag = (struct atag *) (((u32 *) tag) + tag->dwords);
    }

}