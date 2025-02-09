#ifndef _GPIO_H
#define _GPIO_H

#include "peripherals.h"

//Offset from peripheral address base where GPIO starts.
#define GPIO_START_OFFSET (0x00200000) 
#define GPIO_END_OFFSET (0x002000B0)

#define GPFSEL1_ADDR (MMAP_DEVICE_BASE + GPIO_START_OFFSET + 0x00000004)
#define ALT5 (010)

#define GPPUD_ADDR (MMAP_DEVICE_BASE + 0x00200094)
#define GPPUDCLK0_ADDR (MMAP_DEVICE_BASE + 0x00200098)

#endif