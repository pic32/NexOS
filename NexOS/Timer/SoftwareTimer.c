/*
    NexOS Kernel Version v1.02.00
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

#include <string.h>

#include "CriticalSection.h"
#include "../Timer/SoftwareTimer.h"
#include "../Kernel/Memory.h"
#include "../Kernel/Kernel.h"

static DOUBLE_LINKED_LIST_HEAD gSoftwareTimerList;

/*
	This is called by the OS when the user calls InitOS().
*/
BOOL OS_InitSoftwareTimerLib(void)
{
	InitializeDoubleLinkedListHead(&gSoftwareTimerList);

	return TRUE;
}

/*
	This is called by the OS when the timer tick interrupt fires.
*/
void OS_UpdateSoftwareTimers(void)
{
	SOFTWARE_TIMER *Timer;
	DOUBLE_LINKED_LIST_NODE *NodeIterator;

	NodeIterator = gSoftwareTimerList.Beginning;

	while(NodeIterator != (DOUBLE_LINKED_LIST_NODE*)NULL)
	{
		Timer = (SOFTWARE_TIMER*)(NodeIterator->Data);

		Timer->Ticks++;
	
		NodeIterator = NodeIterator->NextNode;
	}
}

void OS_SoftwareTimerClear(SOFTWARE_TIMER *SoftwareTimer)
{
	// just clear the ticks
	SoftwareTimer->Ticks = 0;
}

UINT32 OS_SoftwareTimerGetTicks(SOFTWARE_TIMER *SoftwareTimer)
{
	// just return the ticks
	return SoftwareTimer->Ticks;
}

BOOL OS_SoftwareTimerIsRunning(SOFTWARE_TIMER *SoftwareTimer)
{
	return SoftwareTimer->Active;
}

void OS_SoftwareTimerEnable(SOFTWARE_TIMER *SoftwareTimer, BOOL Enable)
{
	if (SoftwareTimer->Active != Enable)
	{
		SoftwareTimer->Active = Enable;

		if (Enable == TRUE)
			InsertNodeAtEndOfDoubleLinkedList(&gSoftwareTimerList, &(SoftwareTimer->TimerListNode));
		else
			RemoveNodeFromDoubleLinkedList(&gSoftwareTimerList, &(SoftwareTimer->TimerListNode));
	}
}

SOFTWARE_TIMER *CreateSoftwareTimer(SOFTWARE_TIMER *SoftwareTimer, BOOL Enable)
{
    #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
        // if it is not NULL, and not in valid RAM, it is an error
        if (SoftwareTimer != (SOFTWARE_TIMER*)NULL)
            if (RAMAddressValid((OS_WORD)SoftwareTimer) == FALSE)
                return (SOFTWARE_TIMER*)NULL;
    #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

	if (SoftwareTimer == (SOFTWARE_TIMER*)NULL)
	{
		SoftwareTimer = (SOFTWARE_TIMER*)AllocateMemory(sizeof(SOFTWARE_TIMER));

		if (SoftwareTimer == (SOFTWARE_TIMER*)NULL)
		{
			return (SOFTWARE_TIMER*)NULL;
		}
	}

    // initialize the SOFTWARE_TIMER
	SoftwareTimer->Ticks = 0;
	SoftwareTimer->Active = FALSE;
	SoftwareTimer->TimerListNode.Data = (void*)SoftwareTimer;
	SoftwareTimer->TimerListNode.NextNode = SoftwareTimer->TimerListNode.PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;

	if (Enable == TRUE)
	{
		EnterCritical();

		OS_SoftwareTimerEnable(SoftwareTimer, TRUE);

		ExitCritical();
	}

	return SoftwareTimer;
}

#if (USING_SOFTWARE_TIMER_ENABLE_METHOD == 1)
	OS_RESULT SoftwareTimerEnable(SOFTWARE_TIMER *SoftwareTimer, BOOL Enable)
	{
        OS_RESULT Result;
        
		EnterCritical();

		Result = SoftwareTimerEnableFromISR(SoftwareTimer, Enable);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_ENABLE_METHOD == 1)
	
#if (USING_SOFTWARE_TIMER_RESET_METHOD == 1)
	// stops timer and sets the CurrentCount to 0
	OS_RESULT SoftwareTimerReset(SOFTWARE_TIMER *SoftwareTimer)
	{
        OS_RESULT Result;

		EnterCritical();

		Result = SoftwareTimerResetFromISR(SoftwareTimer);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_RESET_METHOD == 1)

#if (USING_SOFTWARE_TIMER_RESTART_METHOD == 1)
	// puts the current count to 0 and enables the counter if disabled
	OS_RESULT SoftwareTimerRestart(SOFTWARE_TIMER *SoftwareTimer)
	{
        OS_RESULT Result;

    	EnterCritical();

		Result = SoftwareTimerRestartFromISR(SoftwareTimer);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_RESTART_METHOD == 1)

#if (USING_SOFTWARE_TIMER_CLEAR_METHOD == 1)
	// clears the current timer, but does not change the state
	OS_RESULT SoftwareTimerClear(SOFTWARE_TIMER *SoftwareTimer)
	{
		OS_RESULT Result;

		EnterCritical();

		// just clear the ticks
		Result = SoftwareTimerClearFromISR(SoftwareTimer);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_CLEAR_METHOD == 1)

#if (USING_SOFTWARE_TIMER_GET_TICKS_METHOD == 1)
	UINT32 SoftwareTimerGetTicks(SOFTWARE_TIMER *SoftwareTimer)
	{
        UINT32 Ticks;

		EnterCritical();

		Ticks = SoftwareTimerGetTicksFromISR(SoftwareTimer);

		ExitCritical();

		return Ticks;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_GET_TICKS_METHOD == 1)

#if (USING_SOFTWARE_TIMER_GET_MILLISECONDS_METHOD == 1)
	UINT32 SoftwareTimerGetMilliseconds(SOFTWARE_TIMER *SoftwareTimer)
	{
		UINT32 Result = SoftwareTimerGetTicks(SoftwareTimer);

		Result = TicksToMilliseconds(Result);

		return Result;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_GET_MILLISECONDS_METHOD == 1)

#if (USING_SOFTWARE_TIMER_IS_RUNNING_METHOD == 1)
	BOOL SoftwareTimerIsRunning(SOFTWARE_TIMER *SoftwareTimer)
	{
        BOOL Running;

		EnterCritical();

		Running = SoftwareTimerIsRunningFromISR(SoftwareTimer);

		ExitCritical();

		return Running;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_IS_RUNNING_METHOD == 1)

#if (USING_SOFTWARE_TIMER_DELETE_METHOD == 1)
	OS_RESULT SoftwareTimerDelete(SOFTWARE_TIMER *SoftwareTimer)
	{
        OS_RESULT Result;
        
        #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
            if (AddressInHeap((OS_WORD)SoftwareTimer) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
        #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

		EnterCritical();

		// if it is active, first remove it from the list
		if (SoftwareTimer->Active == TRUE)
			RemoveNodeFromDoubleLinkedList(&gSoftwareTimerList, &(SoftwareTimer->TimerListNode));

        // clear it out
        memset((void*)SoftwareTimer, 0, sizeof(SOFTWARE_TIMER));
        
        if(ReleaseMemory((void*)SoftwareTimer) == TRUE)
			Result = OS_SUCCESS;
		else
			Result = OS_RESOURCE_NOT_IN_OS_HEAP;

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_DELETE_METHOD == 1)

#if (USING_SOFTWARE_TIMER_ENABLE_FROM_ISR_METHOD == 1)
	OS_RESULT SoftwareTimerEnableFromISR(SOFTWARE_TIMER *SoftwareTimer, BOOL Enable)
	{
        #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)SoftwareTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

		OS_SoftwareTimerEnable(SoftwareTimer, Enable);

		return OS_SUCCESS;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_ENABLE_FROM_ISR_METHOD == 1)
    
#if (USING_SOFTWARE_TIMER_RESET_FROM_ISR_METHOD == 1)
	// stops timer and sets the CurrentCount to 0
	OS_RESULT SoftwareTimerResetFromISR(SOFTWARE_TIMER *SoftwareTimer)
	{
        #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)SoftwareTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

		OS_SoftwareTimerClear(SoftwareTimer);

		// if it is on, turn it off and remove it from the list
		if (SoftwareTimer->Active == TRUE)
		{
			SoftwareTimer->Active = FALSE;

			RemoveNodeFromDoubleLinkedList(&gSoftwareTimerList, &(SoftwareTimer->TimerListNode));
		}

		return OS_SUCCESS;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_RESET_FROM_ISR_METHOD == 1)
    
#if (USING_SOFTWARE_TIMER_RESTART_FROM_ISR_METHOD == 1)
	// puts the current count to 0 and enables the counter if disabled
	OS_RESULT SoftwareTimerRestartFromISR(SOFTWARE_TIMER *SoftwareTimer)
	{
        #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)SoftwareTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

		OS_SoftwareTimerClear(SoftwareTimer);

		// if it is off, turn it on and remove it from the list
		if (SoftwareTimer->Active == FALSE)
		{
			SoftwareTimer->Active = TRUE;

			InsertNodeAtEndOfDoubleLinkedList(&gSoftwareTimerList, &(SoftwareTimer->TimerListNode));
		}

		return OS_SUCCESS;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_RESTART_FROM_ISR_METHOD == 1)
    
#if (USING_SOFTWARE_TIMER_CLEAR_FROM_ISR_METHOD == 1)
	// clears the current timer, but does not change the state
	OS_RESULT SoftwareTimerClearFromISR(SOFTWARE_TIMER *SoftwareTimer)
	{
        #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)SoftwareTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

		// just clear the ticks
		OS_SoftwareTimerClear(SoftwareTimer);

		return OS_SUCCESS;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_CLEAR_FROM_ISR_METHOD == 1)
    
#if (USING_SOFTWARE_TIMER_GET_TICKS_FROM_ISR_METHOD == 1)
	UINT32 SoftwareTimerGetTicksFromISR(SOFTWARE_TIMER *SoftwareTimer)
	{
        #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)SoftwareTimer) == FALSE)
                return 0;
        #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

		return OS_SoftwareTimerGetTicks(SoftwareTimer);
	}
#endif // end of #if (USING_SOFTWARE_TIMER_GET_TICKS_FROM_ISR_METHOD == 1)
    
#if (USING_SOFTWARE_TIMER_GET_MILLISECONDS_FROM_ISR_METHOD == 1)
	UINT32 SoftwareTimerGetMillisecondsFromISR(SOFTWARE_TIMER *SoftwareTimer)
	{
		UINT32 Result = SoftwareTimerGetTicksFromISR(SoftwareTimer);

		Result = TicksToMilliseconds(Result);

		return Result;
	}
#endif // end of #if (USING_SOFTWARE_TIMER_GET_MILLISECONDS_FROM_ISR_METHOD == 1)
    
#if (USING_SOFTWARE_TIMER_IS_RUNNING_FROM_ISR_METHOD == 1)
	BOOL SoftwareTimerIsRunningFromISR(SOFTWARE_TIMER *SoftwareTimer)
	{
        #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)SoftwareTimer) == FALSE)
                return FALSE;
        #endif // end of #if (USING_CHECK_SOFTWARE_TIMER_PARAMETERS == 1)

		return OS_SoftwareTimerIsRunning(SoftwareTimer);
	}
#endif // end of #if (USING_SOFTWARE_TIMER_IS_RUNNING_FROM_ISR_METHOD == 1)