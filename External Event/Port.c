/*
    NexOS Kernel Version v1.01.00
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

#include "Port.h"
#include "HardwareProfile.h"
#include "RTOSConfig.h"
#include "../NexOS/Kernel/OS_Exception.h"

void __attribute__((interrupt(IPL1SAVEALL), vector(_CORE_TIMER_VECTOR))) ContextSwitch(void);

void __attribute__((interrupt(IPL2SAVEALL), vector(_CORE_SOFTWARE_0_VECTOR))) ContextSwitch(void);

void OS_StartFirstTask(OS_WORD *FirstTaskStackPointer);

/*
 * This function is unique to the PIC32 and gets the global pointer value in register 28.
 */
OS_WORD GetGP(void);

#if (USING_ENTER_DEVICE_SLEEP_MODE_METHOD == 1)
    #if(0)
        void PortStopOSTickTimer(void)
        {

        }
    #endif // end of #if(0)

    void PortStartOSTickTimer(void)
    {
        PortUpdateOSTimer();
        
        INTEnable(INT_CT, INT_ENABLED);
    }
#endif // end of #if (USING_ENTER_DEVICE_SLEEP_MODE_METHOD == 1)

void PortStartOSScheduler(void)
{
    // configure the core software interrupt, this is used for SurrenderCPU()
    INTSetVectorPriority(INT_CORE_SOFTWARE_0_VECTOR, INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_CORE_SOFTWARE_0_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    INTClearFlag(INT_CS0);
    INTEnable(INT_CS0, INT_ENABLED);

    // configure up the core timer interrupt, this is the RTOS Tick
    INTSetVectorPriority(INT_CORE_TIMER_VECTOR, INT_PRIORITY_LEVEL_1);
    INTSetVectorSubPriority(INT_CORE_TIMER_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

    // it is a very good idea to use OS_TICK_RATE_IN_HZ in our calculation here
    OpenCoreTimer(GetInstructionClock() / 2 / OS_TICK_RATE_IN_HZ);

    INTClearFlag(INT_CT);
    INTEnable(INT_CT, INT_ENABLED);

    INTEnableSystemMultiVectoredInt();
}

OS_WORD *PortInitializeTaskStack(OS_WORD *Stack, UINT32 StackSizeInWords, TASK_ENTRY_POINT StartingAddress, void *Args)
{
    int i;

    //The below code is untested, but allegedly you need the stack aligned to the nearest 8 byte boundary...

    // point to the end of the stack
    Stack += StackSizeInWords;

    // now make it 8 byte aligned
    Stack = (OS_WORD*)((OS_WORD)Stack & (OS_WORD)~7);
    
    Stack -= 2;

    for(i = 0; i < 30; i++)
    {
        if(i == 27)
        {
            *Stack-- =  GetGP();// This is the Global pointer value. (OS_WORD)GetGP();
        }
        else
        {
            if(i == 3)
            {
                *Stack-- = (OS_WORD)Args;
            }
            else
            {
                *Stack-- = i;
            }
        }
    }

    *Stack-- = (OS_WORD)StartingAddress;

    // Starting SR value for a Task
    *Stack-- = 0x00000003;

    // The high and lo registers.
    *Stack-- = 0;
    *Stack-- = 0;

    return Stack;
}

void PortSetInterruptPriority(BYTE NewInterruptPriority)
{
    OS_WORD NewStatus;

    NewStatus = _CP0_GET_STATUS();

    NewStatus &= ~(0x3F << 10);

    NewStatus |= (NewInterruptPriority << 10);

    _CP0_SET_STATUS(NewStatus);
}

void SurrenderCPU(void)
{
    UINT32 TempCause;							
												
	// Get the current cause value		
	TempCause = _CP0_GET_CAUSE();
    
    // now set the software interrupt request bit
	TempCause |= 0x00000100;
    
    // now set the cause register itself for the interrupt to take place
	_CP0_SET_CAUSE(TempCause);
}

void ClearSoftwareInterrupt(void)
{
    UINT32 TempCause;
    
    INTClearFlag(INT_CS0);
												
	// Get the current cause value		
	TempCause = _CP0_GET_CAUSE();
    
    // now set the software interrupt request bit
	TempCause &= ~(0x00000100);
    
    // now set the cause register itself for the interrupt to take place
	_CP0_SET_CAUSE(TempCause);
}

void _general_exception_handler(void)
{
	OS_EXCEPTION_CODE ExceptionCode;
	UINT32 ExceptionAddress;

	asm volatile("mfc0 %0,$13" : "=r" (ExceptionCode));
	asm volatile("mfc0 %0,$14" : "=r" (ExceptionAddress));

	ExceptionCode = (OS_EXCEPTION_CODE)((ExceptionCode & 0x0000007C) >> 2);

    #if (USING_OS_GENERAL_EXCEPTION_HANDLER == 1)
    {
        OS_WORD *TaskStack = GeneralExceptionHandler(ExceptionCode, ExceptionAddress);

        if(TaskStack != (OS_WORD*)NULL)
            OS_StartFirstTask(TaskStack);
    }
    #endif // end of #if (USING_OS_GENERAL_EXCEPTION_HANDLER == 1)
    
    // we need to do something drastic here
    while(1);
}