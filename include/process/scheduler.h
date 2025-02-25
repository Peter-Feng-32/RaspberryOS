#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#define CONTEXT_OFFSET_IN_PB 0

#ifndef __ASSEMBLER__

void preempt_disable();
void preempt_enable();
extern struct process_block * current_process;
void kern_load_only(struct process_block * to_load);
void kern_switch_to(struct process_block * prev, struct process_block * curr);
void _schedule();
void initialize_scheduler(void * fn, void * arg);
void switch_to(struct process_block * p);
void schedule_tail();

#endif
#endif