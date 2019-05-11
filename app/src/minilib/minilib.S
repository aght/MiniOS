.globl putc
putc:
    push {r0-r12, lr}
    svc #0
    pop {r0-r12, pc}
