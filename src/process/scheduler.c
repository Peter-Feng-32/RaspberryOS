#include "../../include/process/process.h"
#include "../../include/process/scheduler.h"

struct process_block * current_process;

void preempt_disable(void)
{
    current_process->preempt_count++;
}

void preempt_enable(void)
{
    current_process->preempt_count--;
}