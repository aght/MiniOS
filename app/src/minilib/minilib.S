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

.globl clear
clear:
    push {r0-r12, lr}
    svc #2
    pop {r0-r12, pc}

.globl getcwd
getcwd:
    push {lr}
    svc #3
    pop {lr}
