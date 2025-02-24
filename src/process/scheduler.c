#include "../../include/process/process.h"
#include "../../include/process/scheduler.h"

extern int num_processes;
extern struct process_block* processes[NUM_PROCESSES];
struct process_block * current_process;

void preempt_disable(void)
{
    current_process->preempt_count++;
}

void preempt_enable(void)
{
    current_process->preempt_count--;
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