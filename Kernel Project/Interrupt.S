/*
    NexOS Kernel Version v1.02.01
    Copyright (c) 2023 brodie

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
	
#if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)
	.extern		ExternalInterrupt0Handler
	.extern		gSystemStackPointer
	.global		ExternalInterrupt0Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ExternalInterrupt0Interrupt

ExternalInterrupt0Interrupt:
	
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

	jal ExternalInterrupt0Handler /* go to the interrupt handler */
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
	
	.end ExternalInterrupt0Interrupt
#endif // end of #if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)
	
#if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)
	.extern		ExternalInterrupt1Handler
	.extern		gSystemStackPointer
	.global		ExternalInterrupt1Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ExternalInterrupt1Interrupt

ExternalInterrupt1Interrupt:
	
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
	
	jal ExternalInterrupt1Handler /* go to the interrupt handler */
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
	
	.end ExternalInterrupt1Interrupt
#endif // end of #if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)
		
#if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
	.extern		ExternalInterrupt2Handler
	.extern		gSystemStackPointer
	.global		ExternalInterrupt2Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ExternalInterrupt2Interrupt

ExternalInterrupt2Interrupt:
	
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
	
	jal ExternalInterrupt2Handler /* go to the interrupt handler */
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
	
	.end ExternalInterrupt2Interrupt
#endif // end of #if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
	
#if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
	.extern		ExternalInterrupt3Handler
	.extern		gSystemStackPointer
	.global		ExternalInterrupt3Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ExternalInterrupt3Interrupt

ExternalInterrupt3Interrupt:
	
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
	
	jal ExternalInterrupt3Handler /* go to the interrupt handler */
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
	
	.end ExternalInterrupt3Interrupt
#endif // end of #if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
	
#if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)
	.extern		ExternalInterrupt4Handler
	.extern		gSystemStackPointer
	.global		ExternalInterrupt4Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ExternalInterrupt4Interrupt

ExternalInterrupt4Interrupt:
	
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
	
	jal ExternalInterrupt4Handler /* go to the interrupt handler */
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
	
	.end ExternalInterrupt4Interrupt
#endif // end of #if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)
	
#if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)
	.extern		ChangeNotificationInterruptHandler
	.extern		gSystemStackPointer
	.global		ChangeNotificationInterrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ChangeNotificationInterrupt

ChangeNotificationInterrupt:
	
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
	
	jal ChangeNotificationInterruptHandler /* go to the interrupt handler */
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
	
	.end ChangeNotificationInterrupt
#endif // end of #if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)	
	
#if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)
	.extern		Timer1InterruptHandler
	.extern		gSystemStackPointer
	.global		Timer1Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		Timer1Interrupt

Timer1Interrupt:
	
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
	
	jal Timer1InterruptHandler /* go to the interrupt handler */
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
	
	.end Timer1Interrupt
#endif // end of #if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)

#if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)
	.extern		Timer2InterruptHandler
	.extern		gSystemStackPointer
	.global		Timer2Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		Timer2Interrupt

Timer2Interrupt:
	
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
	
	jal Timer2InterruptHandler /* go to the interrupt handler */
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
	
	.end Timer2Interrupt
#endif // end of #if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)

#if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)
	.extern		Timer3InterruptHandler
	.extern		gSystemStackPointer
	.global		Timer3Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		Timer3Interrupt

Timer3Interrupt:
	
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
	
	jal Timer3InterruptHandler /* go to the interrupt handler */
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
	
	.end Timer3Interrupt
#endif // end of #if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)

#if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)
	.extern		Timer4InterruptHandler
	.extern		gSystemStackPointer
	.global		Timer4Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		Timer4Interrupt

Timer4Interrupt:
	
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
	
	jal Timer4InterruptHandler /* go to the interrupt handler */
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
	
	.end Timer4Interrupt
#endif // end of #if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)

#if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)
	.extern		Timer5InterruptHandler
	.extern		gSystemStackPointer
	.global		Timer5Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		Timer5Interrupt

Timer5Interrupt:
	
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
	
	jal Timer5InterruptHandler /* go to the interrupt handler */
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
	
	.end Timer5Interrupt
#endif // end of #if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)

#if (USING_ADC_1_EVENT == 1 || USING_ADC_1_CALLBACK == 1)
	.extern		ADC1InterruptHandler
	.extern		gSystemStackPointer
	.global		ADC1Interrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ADC1Interrupt

ADC1Interrupt:
	
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
	
	jal ADC1InterruptHandler /* go to the interrupt handler */
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
	
	.end ADC1Interrupt
#endif // end of #if (USING_ADC_1_EVENT == 1 || USING_ADC_1_CALLBACK == 1)
	
#if (USING_RTCC_ALARM_EVENT == 1 || USING_RTCC_ALARM_CALLBACK == 1)
	.extern		RTCCAlarmInterruptHandler
	.extern		gSystemStackPointer
	.global		RTCCAlarmInterrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		RTCCAlarmInterrupt

RTCCAlarmInterrupt:
	
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
	
	jal RTCCAlarmInterruptHandler /* go to the interrupt handler */
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
	
	.end RTCCAlarmInterrupt
#endif // end of #if (USING_RTCC_ALARM_EVENT == 1 || USING_RTCC_ALARM_CALLBACK == 1)
	
#if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)
	.extern		ExternalOscillatorFailedInterruptHandler
	.extern		gSystemStackPointer
	.global		ExternalOscillatorFailedInterrupt
	.set		nomips16
 	.set		noreorder
	.set 		noat
 	.ent		ExternalOscillatorFailedInterrupt

ExternalOscillatorFailedInterrupt:
	
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
	
	jal ExternalOscillatorFailedInterruptHandler /* go to the interrupt handler */
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
	
	.end ExternalOscillatorFailedInterrupt
#endif // end of #if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)

#if (USING_IO_BUFFERS == 1)
    #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_RX_CALLBACK == 1 || USING_UART_1_TX_CALLBACK == 1 || USING_UART_1_ERROR_CALLBACK == 1 || USING_UART_1_IO_BUFFER == 1)
	    .extern		UART1InterruptHandler
	    .extern		gSystemStackPointer
	    .global		UART1Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		UART1Interrupt

    UART1Interrupt:

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
	
	    jal UART1InterruptHandler /* go to the interrupt handler */
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

	    .end UART1Interrupt
    #endif // end of #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_RX_CALLBACK == 1 || USING_UART_1_TX_CALLBACK == 1 || USING_UART_1_ERROR_CALLBACK == 1 || USING_UART_1_IO_BUFFER == 1)

    #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_RX_CALLBACK == 1 || USING_UART_2_TX_CALLBACK == 1 || USING_UART_2_ERROR_CALLBACK == 1 || USING_UART_2_IO_BUFFER == 1)
	    .extern		UART2InterruptHandler
	    .extern		gSystemStackPointer
	    .global		UART2Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		UART2Interrupt

    UART2Interrupt:

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
	    
	    jal UART2InterruptHandler /* go to the interrupt handler */
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

	    .end UART2Interrupt
    #endif // end of #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_RX_CALLBACK == 1 || USING_UART_2_TX_CALLBACK == 1 || USING_UART_2_ERROR_CALLBACK == 1 || USING_UART_2_IO_BUFFER == 1)
    
    #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_RX_CALLBACK == 1 || USING_UART_3_TX_CALLBACK == 1 || USING_UART_3_ERROR_CALLBACK == 1 || USING_UART_3_IO_BUFFER == 1)
	    .extern		UART3InterruptHandler
	    .extern		gSystemStackPointer
	    .global		UART3Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		UART3Interrupt

    UART3Interrupt:

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
	    
	    jal UART3InterruptHandler /* go to the interrupt handler */
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

	    .end UART3Interrupt
    #endif // end of #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_RX_CALLBACK == 1 || USING_UART_3_TX_CALLBACK == 1 || USING_UART_3_ERROR_CALLBACK == 1 || USING_UART_3_IO_BUFFER == 1)
    
    #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_RX_CALLBACK == 1 || USING_UART_3_TX_CALLBACK == 1 || USING_UART_4_ERROR_CALLBACK == 1 || USING_UART_4_IO_BUFFER == 1)
	    .extern		UART4InterruptHandler
	    .extern		gSystemStackPointer
	    .global		UART4Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		UART4Interrupt

    UART4Interrupt:

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
	    
	    jal UART4InterruptHandler /* go to the interrupt handler */
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

	    .end UART4Interrupt
    #endif // end of #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_RX_CALLBACK == 1 || USING_UART_4_TX_CALLBACK == 1 || USING_UART_4_ERROR_CALLBACK == 1 || USING_UART_4_IO_BUFFER == 1)
    
    #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_RX_CALLBACK == 1 || USING_UART_5_TX_CALLBACK == 1 || USING_UART_5_ERROR_CALLBACK == 1 || USING_UART_5_IO_BUFFER == 1)
	    .extern		UART5InterruptHandler
	    .extern		gSystemStackPointer
	    .global		UART5Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		UART5Interrupt

    UART5Interrupt:

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
	    
	    jal UART5InterruptHandler /* go to the interrupt handler */
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

	    .end UART5Interrupt
    #endif // end of #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_RX_CALLBACK == 1 || USING_UART_5_TX_CALLBACK == 1 || USING_UART_5_ERROR_CALLBACK == 1 || USING_UART_5_IO_BUFFER == 1)
    
    #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_RX_CALLBACK == 1 || USING_UART_6_TX_CALLBACK == 1 || USING_UART_6_ERROR_CALLBACK == 1 || USING_UART_6_IO_BUFFER == 1)
	    .extern		UART6InterruptHandler
	    .extern		gSystemStackPointer
	    .global		UART6Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		UART6Interrupt

    UART6Interrupt:

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
	    
	    jal UART6InterruptHandler /* go to the interrupt handler */
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

	    .end UART6Interrupt
    #endif // end of #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_RX_CALLBACK == 1 || USING_UART_6_TX_CALLBACK == 1 || USING_UART_6_ERROR_CALLBACK == 1 || USING_UART_6_IO_BUFFER == 1)
    
    #if (USING_CAN_1_IO_BUFFER == 1)
	    .extern		CAN1InterruptHandler
	    .extern		gSystemStackPointer
	    .global		CAN1Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		CAN1Interrupt

    CAN1Interrupt:

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
	    
	    jal CAN1InterruptHandler /* go to the interrupt handler */
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

	    .end CAN1Interrupt
    #endif // end of #if (USING_CAN_1_IO_BUFFER == 1)
    
    #if (USING_CAN_2_IO_BUFFER == 1)
	    .extern		CAN2InterruptHandler
	    .extern		gSystemStackPointer
	    .global		CAN2Interrupt
	    .set		nomips16
	    .set		noreorder
	    .set 		noat
	    .ent		CAN2Interrupt

    CAN2Interrupt:

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
	    
	    jal CAN2InterruptHandler /* go to the interrupt handler */
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

	    .end CAN2Interrupt
    #endif // end of #if (USING_CAN_2_IO_BUFFER == 1)
#endif // end of #if (USING_IO_BUFFERS == 1)