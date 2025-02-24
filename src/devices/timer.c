#include "../../include/devices/timer.h"
#include "../../include/utils/utils.h"
#include "../../include//devices/uart.h"

/**

Write timer-specific interrupt handler here.
As well as code to set up timers.

Timer upper and lower registers combined are read_time.
*/

unsigned long long read_time() {
    volatile unsigned long lower = get32(TIMER_CLO);
    volatile unsigned long higher = get32(TIMER_CHI);
    volatile unsigned long long time = ((unsigned long long) higher) << 32 | lower;

    return time;
}

void tick_in(unsigned long delay){
    volatile unsigned long lower = get32(TIMER_CLO);
    put32(TIMER_C1, lower + delay);
}
