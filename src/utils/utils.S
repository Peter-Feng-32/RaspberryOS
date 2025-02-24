.section ".text.boot"

.globl put32
put32:
    str w1, [x0] //w register = lower 32 bits of x register, x register = 64 bit
    ret

.globl get32
get32:
    ldr x2, =0x0
    ldr w0, [x0]
    ldr x2, =0x300
    ret

.globl get_el
get_el:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret