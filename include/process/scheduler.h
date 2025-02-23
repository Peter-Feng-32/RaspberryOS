#ifndef _SCHEDULER_H
#define _SCHEDULER_H

void preempt_disable(void);
void preempt_enable(void);

extern struct process_block * current_process;

#endif