#include "../../include/process/process.h"
#include "../../include/process/scheduler.h"
#include "../../include/malloc/bin_malloc.h"
#include "../../include/utils/strings.h"
#include "../../include/devices/uart.h"

struct process_block * current_process;

void preempt_disable(void)
{
    current_process->preempt_count++;
}

void preempt_enable(void)
{
    current_process->preempt_count--;
}

void initialize_scheduler(void * fn, void * arg) {
    current_process = bin_malloc(sizeof(struct process_block));
    current_process->priority = MAX_PRIORITY;
    current_process->state = STATE_STARTED;
    current_process->counter = 10;
    current_process->preempt_count = 0; 

    current_process->context.x19 = (u64) fn;
    current_process->context.x20 = (u64) arg;
    current_process->context.pc = (u64) &process_bootstrap;
    void * stack = bin_malloc(PAGE_SIZE);
    current_process->context.sp = (u64) stack;

    processes[0] = current_process;
    num_processes = 1;
}

void switch_to(struct process_block * p) {
    if(p == current_process) {
        return;
    }
    struct process_block * prev = current_process;
    current_process = p;
    kern_switch_to(prev, current_process);
}

void _schedule(void)
{
    preempt_disable();
    int next,c;
    struct process_block * p;
    while (1) {
        c = -1;
        next = 0;
        for (int i = 0; i < num_processes; i++){
            p = processes[i];
            if (p && p->state == STATE_STARTED && p->counter > c) {
                c = p->counter;
                next = i;
            }
        }
        if (c) {
            break;
        }
        for (int i = 0; i < num_processes; i++) {
            p = processes[i];
            if (p) {
                p->counter = (p->counter >> 1) + p->priority;
            }
        }
    }
    switch_to(processes[next]);
    preempt_enable();
}