#include "../include/timer.h"
#include "../include/utils.h"
/**

Write timer-specific interrupt handler here.
As well as code to set up timers.

Timer upper and lower registers combined are read_time.


 */

unsigned long long read_time() {
    unsigned long lower = get32(TIMER_CLO);
    unsigned long higher = get32(TIMER_CHI);
    unsigned long long time = ((unsigned long long) higher) << 32 | lower;

    return time;
}

void tick_in(unsigned long delay){
    unsigned long lower = get32(TIMER_CLO);
    put32(TIMER_C1, lower + delay);
}

void set_timer_handler();