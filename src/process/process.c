#include "../../include/process/process.h"
#include "../../include/malloc/bin_malloc.h"
#include "../../include/process/scheduler.h"

static struct process_block* processes[NUM_PROCESSES];
static int num_processes = 0;

int make_process(void * fn, void * arg) {
    preempt_disable();
    struct process_block* process;

    process = bin_malloc(sizeof(struct process_block));
    if(!process) {
        return 1;
    }
    //Todo: fix initialization
    process->priority = current_process->priority;
    process->state = STATE_STARTED;
    process->counter = current_process->counter;
    process->preempt_count = 0; 

    process->context.x19 = (u64) fn;
    process->context.x20 = (u64) arg;
    process->context.pc = (u64) &process_bootstrap;

    void * stack = bin_malloc(PAGE_SIZE);
    process->context.sp = (u64) stack;

    int pid = num_processes++;
    processes[pid] = process;

    preempt_enable();
    return 0;
}