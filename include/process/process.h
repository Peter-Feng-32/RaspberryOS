#include "../../include/utils/types.h"

#ifndef _PROCESS_H
#define _PROCESS_H

#define NUM_PROCESSES 64

#define STATE_STARTED 0


struct cpu_context {
    u64 x19;
    u64 x20;
    u64 x21;
    u64 x22;
    u64 x23;
    u64 x24;
    u64 x25;
    u64 x26;
    u64 x27;
    u64 x28;
    u64 fp;
    u64 sp;
    u64 pc;
};

struct process_block {
    struct cpu_context context;
    u32 state;
    u32 counter;
    u32 priority;
    u32 preempt_count;
};

void process_bootstrap();

#endif
