#include "mm.h"

.section ".text.boot"
.global _start

_start:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF // Get last 8 bits of mpdir_el1 (core id)
    cbz x0, master // Branch to master if 0
    b proc_hang

proc_hang:
    b proc_hang
    
memzero:
    str xzr, [x0], #8
    subs x1, x1, #8
    cmp x1, #0
    bgt memzero
    ret

master:
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0 
    bl memzero

    mov sp, #LOW_MEMORY // For now, set stack pointer to 0x4000000
    bl kernel_main   

