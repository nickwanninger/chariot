
#include <asmdefs.h>

# start.S - Boot level functionality for ChariotOS on arm64

# AArch64 mode
 
# To keep this in the first portion of the binary.
.section ".text.boot"
 
# Make _start global.
.globl _start
# Entry point for the kernel. Registers:
# x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
# x1 -> 0
# x2 -> 0
# x3 -> 0
# x4 -> 32 bit kernel entry point, _start location
_start:
		/* Disable all interrupts */
		msr daifset, #2

		/* Save the FDT pointer passed in x0 to x20 */
		mov x20, x0

		/* Disable the MMU */
		dsb sy /* Data sync barrier */
		mrs x2, sctlr_el1
		bic x2, x2, #0x1
		msr sctlr_el1, x2
		isb /* Instruction sync barrier */




    # set stack before our code
    ldr     x5, =_start
    mov     sp, x5
 
    # clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
3:  cbz     w6, 4f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 3b
 
    # jump to C code, should not return
4:  bl      kernel_main

.deadloop:
    # for failsafe, halt this core too
    b .deadloop
