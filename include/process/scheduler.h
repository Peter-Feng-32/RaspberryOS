#ifndef _SCHEDULER_H
#define _SCHEDULER_H

void preempt_disable(void);
void preempt_enable(void);

extern struct process_block * current_process;
void kern_switch_to(struct process_block * prev, struct process_block * curr);

#define CONTEXT_OFFSET_IN_PB 0


#endif