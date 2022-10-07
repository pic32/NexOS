/*
    NexOS Kernel Version v1.01.05
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

#ifndef OS_EXCEPTIONS_H
    #define OS_EXCEPTIONS_H

#include "GenericTypes.h"

typedef enum
{
	INTERRUPT_EXCEPTION = 0,				// interrupt
	ADDRESS_LOAD_EXCEPTION = 4,				// address error exception (load or ifetch)
	ADDRESS_STORE_EXCEPTION_EXCEPTION,		// address error exception (store)
	INSTRUCTION_BUS_EXCEPTION,				// bus error (ifetch)
	DEBUG_EXCEPTION,						// bus error (load/store)
	SYSCALL_EXCEPTION,						// syscall
	BREAKPOINT_EXCEPTION,					// breakpoint
	RESERVED_INSTRUCTION_EXCEPTION,			// reserved instruction
	CPU_UNUSABLE_EXCEPTION,					// coprocessor unusable
	OVERFLOW_EXCEPTION,						// arithmetic overflow
	TRAP_EXCEPTION,							// trap (possible divide by zero)
	IMPLEMENTATION_SPECIFIC_EXCEPTION = 16,	// implementation specfic 1
	COR_EXTEND_UNUSABLE_EXCEPTION,			// CorExtend Unuseable
	COPROCESSOR_2_EXCEPTION,				// coprocessor 2
	UNDEFINED_EXCEPTION,					// unknown why the exception happened
	NUMBER_OF_EXCEPTION_CODES
}OS_EXCEPTION_CODE;

OS_WORD *GeneralExceptionHandler(OS_EXCEPTION_CODE ExceptionCode, UINT32 ExceptionAddress);

#endif // end of 
