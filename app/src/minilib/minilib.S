.section .text
.align 4

.globl putc
putc:
    push {r0-r12, lr}
    svc #0
    pop {r0-r12, pc}

.globl getc
getc:
    push {lr}
    svc #1
    pop {pc}
