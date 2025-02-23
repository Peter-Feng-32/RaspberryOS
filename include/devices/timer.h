#include "peripherals.h"
#include "../interrupts/interrupts.h"
#ifndef _TIMER_H
#define _TIMER_H

#define TIMER_BASE_ADDRESS (MMAP_DEVICE_BASE + 12288)
#define TIMER_CS TIMER_BASE_ADDRESS
#define TIMER_CLO (TIMER_BASE_ADDRESS + 4)
#define TIMER_CHI (TIMER_BASE_ADDRESS + 8)
#define TIMER_C0 (TIMER_BASE_ADDRESS + 12)
#define TIMER_C1 (TIMER_BASE_ADDRESS + 16)
#define TIMER_C2 (TIMER_BASE_ADDRESS + 20)
#define TIMER_C3 (TIMER_BASE_ADDRESS + 24)

#define TIMER_1_MASK 2

void tick_in(unsigned long delay);
unsigned long long read_time();
#endif