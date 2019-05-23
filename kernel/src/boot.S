.section ".boot"

.equ SCTLR_ENABLE_DATA_CACHE,	0x4
.equ SCTLR_ENABLE_BRANCH_PREDICTION, 0x800
.equ SCTLR_ENABLE_INSTRUCTION_CACHE, 0x1000

.globl _bootloader
.align 2
_bootloader:
	@ in QEMU all of 4 ARM CPUs are started simultaniously         /* From                                                      */
	                                                               /* https://github.com/mmuszkow/NoOsBootstrap/tree/master/arm */
    @ by default. I don't know if this is the real hw behaviour,
    @ but here I jump to halt if CPU ID (stored in MPIDR
    @ register, first 2 bits) is not 0
    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne halt

		// set vector address.
    ldr r0, =vector
    mcr P15, 0, r0, c12, c0, 0

		// set sp in irq mode.
		//(need this for IRQs)
		mrs r0, cpsr
		bic r1, r0, #0x1F
		orr r1, r1, #0x12
		msr cpsr_c,r1
		mov sp, #0x4000	

		// set sp in svc mode.
		//(I think svc is supervisor mode)
		//See here:
		//    https://www.freertos.org/Using-FreeRTOS-on-Cortex-A-proprietary-interrupt-controller.html#modes-and-stacks
		msr cpsr_c, r0
		ldr r4, =__stack_end 			/* Set up SP */
		mov sp, r4

		// https://github.com/LdB-ECM/Raspberry-Pi/blob/master/GLES2/SmartStart32.S
    mrc p15, 0, r0, c1, c0, 0										  @ R0 = System Control Register
    orr r0, #SCTLR_ENABLE_BRANCH_PREDICTION				@ Branch prediction on
    orr r0, #SCTLR_ENABLE_DATA_CACHE							@ Data cache on
    orr r0, #SCTLR_ENABLE_INSTRUCTION_CACHE				@ Instruction cache on

		// http://www.valvers.com/open-software/raspberry-pi/step05-bare-metal-programming-in-c-pt5/
    mrc p15, #0, r1, c1, c0, #2
    orr r1, r1, #(0xf << 20)
    mcr p15, #0, r1, c1, c0, #2
    mov r1, #0
    mcr p15, #0, r1, c7, c5, #4
    mov r0,#0x40000000
    fmxr fpexc, r0

    mcr p15,0,r0,c1,c0,0													@ System Control Register = R0

		ldr r4, =__bss_start__			/* Fill BSS with Zeroes */
		ldr r9, =__bss_end__
		mov r5, #0

next_word:
		cmp r4, r9
		bge end_bss_init
		stmia r4!, {r5}
		b next_word

end_bss_init:

		/* Set up Vector Table */
		ldr 		r0, =vector			/* The vector table */
		mov     r1, #0x0000			  	/* Where the Vector Table is*/
		ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
		stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
		ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
		stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

		/* Pass control to the Kernel */
		ldr r3, =main
		blx r3

halt:						/* Kernel should not return, but in case it does */
    wfe @ low-power mode
    b halt

hang:
		b hang

.balign 32
vector:
		ldr pc, reset_handler
		ldr pc, undefined_handler
		ldr pc, swi_handler
		ldr pc, prefetch_handler
		ldr pc, data_handler
		ldr pc, unused_handler
		ldr pc, irq_handler
		ldr pc, fiq_handler

		reset_handler:      .word _bootloader
		undefined_handler:  .word hang
		swi_handler:        .word svc_handler
		prefetch_handler:   .word hang
		data_handler:       .word hang
		unused_handler:     .word hang
		irq_handler:        .word hang
		fiq_handler:        .word hang
