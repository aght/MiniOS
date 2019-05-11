.globl svc_handler
svc_handler:
    push {r0-r12, lr}

    mov r1, r0
    mov r2, r1
    mov r3, r2
    mov r4, r3

    ldr r0, [lr, #-4]
    bic r0, r0, #0xFF000000

    bl c_svc_handler

    ldm sp!, {r0-r12, pc}^
