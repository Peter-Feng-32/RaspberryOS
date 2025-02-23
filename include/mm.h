#ifndef _MM_H
#define _MM_H

#define LOW_MEMORY 0x08000000
#define HIGH_MEMORY 0x3F000000

#define PAGE_SIZE 4096

#ifndef __ASSEMBLER__
void memzero(unsigned long start, unsigned long end);
#endif

#endif