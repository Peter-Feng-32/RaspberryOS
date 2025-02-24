.globl process_bootstrap
/*
Process bootstrap puts arg into x0 from x20, which is how we set it up when we initialize process.
Then we jump to x19 which is the address of our function.
Since at the start of process x0 and x1 are empty anyway.
 */

process_bootstrap:
ldr x0, x20
ble x19
