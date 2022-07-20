/*
    NexOS Kernel Version v1.01.03
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

#include "CriticalSection.h"
#include "../NexOS/Kernel/Kernel.h"
#include "Port.h"

volatile OS_WORD gCurrentCriticalCount = 0;

void EnterCritical(void)
{
	if(gCurrentCriticalCount == 0)
	{
		PortSetInterruptPriority(OS_PRIORITY);
	}

	gCurrentCriticalCount++;
}

void ExitCritical(void)
{
	gCurrentCriticalCount--;

	if(gCurrentCriticalCount == 0)
	{
		PortSetInterruptPriority(0);
	}
}

#if (USING_GET_CURRENT_CRITICAL_COUNT_METHOD == 1)
    OS_WORD GetCurrentCriticalCount(void)
    {
        OS_WORD TempCurrentPirority;
        
        // set the interrupt priority to the OS level
        PortSetInterruptPriority(OS_PRIORITY);
        
        // now get the critical count
        TempCurrentPirority = gCurrentCriticalCount;
        
        // if we were at zero for a critical count, re-enable interrupts below OS_PRIORITY
        if(gCurrentCriticalCount == 0)
            PortSetInterruptPriority(0);
        
        return TempCurrentPirority;
    }
#endif // end of #if (USING_GET_CURRENT_CRITICAL_COUNT_METHOD == 1)

#if (USING_EXIT_CRITICAL_UNCONDITIONAL_METHOD == 1)
    void ExitCriticalUnconditional(void)
    {
        // disable interrupts as we are about to modify the critical count
        // they could have a critical count of zero before calling this...
        PortSetInterruptPriority(OS_PRIORITY);
        
        gCurrentCriticalCount = 0;
        
        PortSetInterruptPriority(0);
    }
#endif // end of #if (USING_EXIT_CRITICAL_UNCONDITIONAL_METHOD == 1)