#ifndef _MM_H
#define _MM_H

#define LOW_MEMORY 0x05000000

#ifndef __ASSEMBLER__
void memzero(unsigned long start, unsigned long end);
#endif

#endif