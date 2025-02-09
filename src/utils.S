.section ".text.boot"

.globl put32
put32:
    str w1, [x0] //w register = lower 32 bits of x register, x register = 64 bit
    ret

.globl get32
get32:
    ldr w0, [x0]
    ret