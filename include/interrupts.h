#include "peripherals.h"

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#define INTERRUPTS_BASE_ADDRESS (MMAP_DEVICE_BASE + 45056)
#define IRQ_BASIC_PENDING (INTERRUPTS_BASE_ADDRESS + 512)
#define IRQ_1_PENDING (INTERRUPTS_BASE_ADDRESS + 516)
#define IRQ_2_PENDING (INTERRUPTS_BASE_ADDRESS + 520)
#define FIQ_CONTROL (INTERRUPTS_BASE_ADDRESS + 524)
#define ENABLE_IRQS_1 (INTERRUPTS_BASE_ADDRESS + 528)
#define ENABLE_IRQS_2 (INTERRUPTS_BASE_ADDRESS + 532)
#define ENABLE_IRQS_BASIC (INTERRUPTS_BASE_ADDRESS + 536)
#define DISABLE_IRQS_1 (INTERRUPTS_BASE_ADDRESS + 540)
#define DISABLE_IRQS_2 (INTERRUPTS_BASE_ADDRESS + 544)
#define DISABLE_IRQS_BASIC (INTERRUPTS_BASE_ADDRESS + 548)

// Defines bitmask corresponding to the IRQ based on the ARM peripherals interrupt table
#define SYSTEM_TIMER_1_IRQ 2

void irq_vector_init();
void enable_disable_irq(int interrupt_register, int interrupt);

#endif