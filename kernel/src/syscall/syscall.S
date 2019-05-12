.section .text
.align 4

.globl svc_handler
svc_handler:
    @ push {r0-r12, lr}
    push {lr}

    @ shift params up to allow for svc number in r0
    mov r1, r0
    mov r2, r1
    mov r3, r2
    mov r4, r3

    @ extract svc number from svc instruction by masking
    @ top 8 bits
    ldr r0, [lr, #-4]
    bic r0, r0, #0xFF000000

    bl c_svc_handler

    @ ldm sp!, {r0-r12, pc}^
    pop {pc}
