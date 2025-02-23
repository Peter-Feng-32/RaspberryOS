#include "mm.h"
#include "sysregs.h"

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
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0		

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SPSR_VALUE
	msr	spsr_el2, x0

	adr	x0, el1_entry		
	msr	elr_el2, x0

	eret	

el1_entry:
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0 
    bl memzero

    ldr x0, =0x04000000
    mov sp, x0 // For now, set stack pointer to 0x4000000
    bl receive_kernel   

