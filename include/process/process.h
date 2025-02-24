#include "../../include/utils/types.h"

#ifndef _PROCESS_H
#define _PROCESS_H

#define NUM_PROCESSES 64

#define STATE_STARTED 0
#define MAX_PRIORITY 10

extern volatile int num_processes;
extern struct process_block* processes[NUM_PROCESSES];

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
    int counter;
    int priority;
    int preempt_count;
};

extern void process_bootstrap();
extern int make_process(void * fn, void * arg);

#endif
