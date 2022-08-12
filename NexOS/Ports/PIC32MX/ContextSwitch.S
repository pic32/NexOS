/*
    NexOS Kernel Version v1.01.04
    Copyright (c) 2022 brodie

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */
	
#include <xc.h>
#include <sys/asm.h>
#include "RTOSConfig.h"

	.equ		TASK_CONTEXT_SIZE_IN_BYTES,		140

	.set		nomips16
 	.set		noreorder
	.set 		noat
	.global 	GetGP

	.ent		GetGP

GetGP:
	
	addu  v0, gp, zero
	jr ra
	nop

	.end 		GetGP


	.extern		NextTask
	.extern		gSystemStackPointer
	.global		ContextSwitch
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ContextSwitch

/*
	131 instructions per context switch on NextTaskJustIterate
*/

ContextSwitch:
	
	addi $29, $29, -TASK_CONTEXT_SIZE_IN_BYTES	 /* Make room for the context of the currently running task */	

	/*
		Save k1 and k0 so we can use them as temporaries in saving and manipulating some of the
		CP0 Registers.
	*/
	sw k1, 36(sp)

	sw k0, 32(sp)

	mfc0 k1, _CP0_EPC
	sw k1, 16(sp)
	
	mfc0 k1, _CP0_STATUS
	sw k1, 12(sp)
	
	// enable interrupts above the OS priority
	// this is ok as long as they don't make any interaction with the OS
	ins k1, zero, 10, 6
	ori k1, k1, ((OS_PRIORITY + 1) << 10)
	ins k1, zero, 1, 4
	
	// this sets the priortiy altered in the above area
	mtc0 k1, _CP0_STATUS

	/*
		Save all the GPR's onto the stack
	*/
	sw $1, 136(sp)
	sw v0, 132(sp)
	sw v1, 128(sp)
	sw a0, 124(sp)
	sw a1, 120(sp)
	sw a2, 116(sp)
	sw a3, 112(sp)
	sw t0, 108(sp)
	sw t1, 104(sp)
	sw t2, 100(sp)
	sw t3, 96(sp)
	sw t4, 92(sp)
	sw t5, 88(sp)
	sw t6, 84(sp)
	sw t7, 80(sp) 
	sw s0, 76(sp)
	sw s1, 72(sp)
	sw s2, 68(sp)
	sw s3, 64(sp)
	sw s4, 60(sp)
	sw s5, 56(sp)
	sw s6, 52(sp)
	sw s7, 48(sp)
	sw t8, 44(sp)
	sw t9, 40(sp)
	sw gp, 28(sp)
	sw fp, 24(sp)
	sw ra, 20(sp)
	
	mfhi k0
	sw k0, 8(sp)
	
	mflo k0
	sw k0, 4(sp)
	
	move a0, sp /* Move the stack pointer to the register for value passing in C */
	
	la sp, gSystemStackPointer
	lw sp, (sp)

	jal OS_NextTask	/* go to switch task */
	nop
	
	/* now begin to restore context */
	
	
	move sp, v0		/* restore the sp, Microchip C32 Compiler Guide says v0,v1 are used to return values */
	
	/* Now restore registers */
	
	lw k0, 4(sp) 
	mtlo k0
	
	lw k0, 8(sp)
	mthi k0
	
	lw k0, 16(sp)
	mtc0 k0, _CP0_EPC
	
	lw ra, 20(sp)
	lw fp, 24(sp)
	lw gp, 28(sp)
	lw k0, 32(sp)
	lw k1, 36(sp)
	lw t9, 40(sp)
	lw t8, 44(sp)
	lw s7, 48(sp)
	lw s6, 52(sp)
	lw s5, 56(sp)
	lw s4, 60(sp)
	lw s3, 64(sp)
	lw s2, 68(sp)
	lw s1, 72(sp)
	lw s0, 76(sp)
	lw t7, 80(sp)
	lw t6, 84(sp)
	lw t5, 88(sp)
	lw t4, 92(sp)
	lw t3, 96(sp)
	lw t2, 100(sp)
	lw t1, 104(sp)
	lw t0, 108(sp)
	lw a3, 112(sp)
	lw a2, 116(sp)
	lw a1, 120(sp)
	lw a0, 124(sp)
	lw v1, 128(sp)
	lw v0, 132(sp)
	lw $1, 136(sp)

	lw k0, 12(sp)
	mtc0 k0, _CP0_STATUS

	addi $29, $29, TASK_CONTEXT_SIZE_IN_BYTES /* Return stack to previous position */

	eret /* Return from exception */
	nop
	
	.end ContextSwitch



	.global OS_StartFirstTask

	.ent OS_StartFirstTask

OS_StartFirstTask:

	move sp, a0		/* restore the sp, Microchip C Compiler Guide says v0,v1 are used to return values */
	
	/* Now restore registers */
	
	lw k0, 4(sp) 
	mtlo k0
	
	lw k0, 8(sp)
	mthi k0
	
	lw k0, 16(sp)
	mtc0 k0, _CP0_EPC

	lw $31, 20(sp)
	lw $30, 24(sp)
	lw $28, 28(sp)
	lw $27, 32(sp)
	lw $26, 36(sp)
	lw $25, 40(sp)
	lw $24, 44(sp)
	lw $23, 48(sp)
	lw $22, 52(sp)
	lw $21, 56(sp)
	lw $20, 60(sp)
	lw $19, 64(sp)
	lw $18, 68(sp)
	lw $17, 72(sp)
	lw $16, 76(sp)
	lw $15, 80(sp)
	lw $14, 84(sp)
	lw $13, 88(sp)
	lw $12, 92(sp)
	lw $11, 96(sp)
	lw $10, 100(sp)
	lw $9, 104(sp)
	lw $8, 108(sp)
	lw $7, 112(sp)
	lw $6, 116(sp)
	lw $5, 120(sp)
	lw $4, 124(sp)
	lw $3, 128(sp)
	lw $2, 132(sp)
	lw $1, 136(sp)
	
	lw k0, 12(sp)
	mtc0 k0, _CP0_STATUS

	addi $29, $29, TASK_CONTEXT_SIZE_IN_BYTES /* Return stack to previous position */

	eret /* Return from exception */
	nop

	.end OS_StartFirstTask
	