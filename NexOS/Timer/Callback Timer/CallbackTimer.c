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

#include "../NexOS/Kernel/Kernel.h"
#include "../NexOS/Kernel/Memory.h"
#include "../SoftwareTimer.h"
#include "CallbackTimer.h"
#include "CriticalSection.h"

static DOUBLE_LINKED_LIST_HEAD gCallbackTimerList;

void OS_UpdateCallbackTimers(UINT32 CurrentOSTickCount)
{
	DOUBLE_LINKED_LIST_NODE *TempCurrentNode, *TempNextNode;
	CALLBACK_TIMER *TempCallbackTimer;

	// now update the timer list
	// we just have to iterate through and see if anybody is done yet
	TempCurrentNode = gCallbackTimerList.Beginning;

	while(TempCurrentNode != (DOUBLE_LINKED_LIST_NODE*)NULL)
	{
		TempNextNode = TempCurrentNode->NextNode;

		TempCallbackTimer = (CALLBACK_TIMER*)TempCurrentNode->Data;

		if(OS_SoftwareTimerGetTicks(&TempCallbackTimer->Timer) == TempCallbackTimer->PeriodicityInTicks)
		{
			// execute the callback timers callback method
			TempCallbackTimer->CallbackTimerCallback(CurrentOSTickCount);

			OS_SoftwareTimerClear(&TempCallbackTimer->Timer);
		}

		TempCurrentNode = TempNextNode;
	}	
}

BOOL OS_InitCallbackTimersLib(void)
{
	InitializeDoubleLinkedListHead(&gCallbackTimerList);

	return TRUE;
}

CALLBACK_TIMER *CreateCallbackTimer(CALLBACK_TIMER *CallbackTimer, UINT32 PeriodicityInTicks, CALLBACK_TIMER_CALLBACK CallbackTimerCallback, BOOL Enable)
{
    #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
        if (PeriodicityInTicks == 0 || PeriodicityInTicks == INVALID_TIMER_TICKS_VALUE)
            return (CALLBACK_TIMER*)NULL;

        if (ProgramAddressValid((OS_WORD)CallbackTimerCallback) == FALSE)
            return (CALLBACK_TIMER*)NULL;
    #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
    
	if(CallbackTimer != (CALLBACK_TIMER*)NULL)
	{
		if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
			return (CALLBACK_TIMER*)NULL;
	}
	else
	{
		CallbackTimer = (CALLBACK_TIMER*)AllocateMemory(sizeof(CALLBACK_TIMER));

		if(CallbackTimer == (CALLBACK_TIMER*)NULL)
			return (CALLBACK_TIMER*)NULL;
	}

	CreateSoftwareTimer(&(CallbackTimer->Timer), FALSE);

	CallbackTimer->PeriodicityInTicks = PeriodicityInTicks;
	CallbackTimer->CallbackTimerCallback = CallbackTimerCallback;
	CallbackTimer->Node.NextNode = CallbackTimer->Node.PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;
	CallbackTimer->Node.Data = CallbackTimer;

	if(Enable == TRUE)
	{
		EnterCritical();

		InsertNodeAtEndOfDoubleLinkedList(&gCallbackTimerList, &CallbackTimer->Node);

		OS_SoftwareTimerEnable(&(CallbackTimer->Timer), TRUE);

		ExitCritical();
	}

	return CallbackTimer;
}

#if (USING_CALLBACK_TIMER_ENABLE_METHOD == 1)
	OS_RESULT CallbackTimerEnable(CALLBACK_TIMER *CallbackTimer, BOOL Enable)
	{
        OS_RESULT Result;

		EnterCritical();

        Result = CallbackTimerEnableFromISR(CallbackTimer, Enable);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_CALLBACK_TIMER_ENABLE_METHOD == 1)

#if (USING_CALLBACK_TIMER_RESET_METHOD == 1)
	// stops timer and sets the CurrentCount to 0
	OS_RESULT CallbackTimerReset(CALLBACK_TIMER *CallbackTimer)
	{
        OS_RESULT Result;

		EnterCritical();

        Result = CallbackTimerResetFromISR(CallbackTimer);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_CALLBACK_TIMER_RESET_METHOD == 1)

#if (USING_CALLBACK_TIMER_RESTART_METHOD == 1)
	// puts the current count to 0 and enables the counter if disabled
	OS_RESULT CallbackTimerRestart(CALLBACK_TIMER *CallbackTimer)
	{
        OS_RESULT Result;

		EnterCritical();

        Result = CallbackTimerRestartFromISR(CallbackTimer);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_CALLBACK_TIMER_RESTART_METHOD == 1)

#if (USING_CALLBACK_TIMER_GET_TICKS_REMAINING_METHOD == 1)
	// gets how long until the method fires in ticks
	UINT32 CallbackTimerGetTicksRemaining(CALLBACK_TIMER *CallbackTimer)
	{
        UINT32 TicksRemaining;

		EnterCritical();

		TicksRemaining = CallbackTimerGetTicksRemainingFromISR(CallbackTimer);

		ExitCritical();

		return TicksRemaining;
	}
#endif // end of #if (USING_CALLBACK_TIMER_GET_TICKS_REMAINING_METHOD == 1)

#if (USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_METHOD == 1)
	UINT32 CallbackTimerGetPeriodicityInTicks(CALLBACK_TIMER *CallbackTimer)
	{
        UINT32 PeriodicityInTicks;

		EnterCritical();

		PeriodicityInTicks = CallbackTimerGetPeriodicityInTicksFromISR(CallbackTimer);

		ExitCritical();

		return PeriodicityInTicks;
	}
#endif // end of #if (USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_METHOD == 1)

#if (USING_CALLBACK_TIMER_SET_PERIODICITY_METHOD == 1)
	OS_RESULT CallbackTimerSetPeriodicity(CALLBACK_TIMER *CallbackTimer, UINT32 PeriodicityInTicks)
	{
        OS_RESULT Result;
        
		EnterCritical();

		Result = CallbackTimerSetPeriodicityFromISR(CallbackTimer, PeriodicityInTicks);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_CALLBACK_TIMER_SET_PERIODICITY_METHOD == 1)

#if(USING_CALLBACK_TIMER_SET_CALLBACK_METHOD == 1)
    OS_RESULT CallbackTimerSetCallback(CALLBACK_TIMER *CallbackTimer, CALLBACK_TIMER_CALLBACK CallbackTimerCallback)
	{
        OS_RESULT Result;
        
		EnterCritical();

		Result = CallbackTimerSetCallbackFromISR(CallbackTimer, CallbackTimerCallback);

		ExitCritical();

		return Result;
	}
#endif // end of #if(USING_CALLBACK_TIMER_SET_CALLBACK_METHOD == 1)
    
#if (USING_CALLBACK_TIMER_DELETE_METHOD == 1)
	OS_RESULT CallbackTimerDelete(CALLBACK_TIMER *CallbackTimer)
	{
		OS_RESULT Result;

        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(AddressInHeap((OS_WORD)CallbackTimer) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

		EnterCritical();

		// this will remove it from any lists it might be on
		CallbackTimerEnableFromISR(CallbackTimer, FALSE);
        
		// now we can attempt to release the callback timer
		if (ReleaseMemory((void*)CallbackTimer) == TRUE)
			Result = OS_SUCCESS;
		else
			Result = OS_RESOURCE_NOT_IN_OS_HEAP;

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_CALLBACK_TIMER_DELETE_METHOD == 1)

#if (USING_CALLBACK_TIMER_IS_RUNNING_METHOD == 1)
	BOOL CallbackTimerIsRunning(CALLBACK_TIMER *CallbackTimer)
	{
        BOOL Result;
        
        EnterCritical();
        
        Result = CallbackTimerIsRunningFromISR(CallbackTimer);
        
        ExitCritical();
        
        return Result;
	}
#endif // end of #if (USING_CALLBACK_TIMER_IS_RUNNING_METHOD == 1)

#if (USING_CALLBACK_TIMER_RESET_FROM_ISR_METHOD == 1)
	// stops timer and sets the CurrentCount to 0
	OS_RESULT CallbackTimerResetFromISR(CALLBACK_TIMER *CallbackTimer)
	{
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

        // clear the timer
		OS_SoftwareTimerClear(&CallbackTimer->Timer);

        // now disable it
        CallbackTimerEnableFromISR(CallbackTimer, FALSE);

		return OS_SUCCESS;
	}
#endif // end of #if (USING_CALLBACK_TIMER_RESET_METHOD == 1)
    
#if (USING_CALLBACK_TIMER_ENABLE_FROM_ISR_METHOD == 1)
	OS_RESULT CallbackTimerEnableFromISR(CALLBACK_TIMER *CallbackTimer, BOOL Enable)
	{
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

		if (OS_SoftwareTimerIsRunning(&CallbackTimer->Timer) == Enable)
			return OS_SUCCESS;

		if(Enable != OS_SoftwareTimerIsRunning(&CallbackTimer->Timer))
        {
            // they want to enable it and it is disabled, this is valid
            if(Enable == TRUE)
                InsertNodeAtEndOfDoubleLinkedList(&gCallbackTimerList, &CallbackTimer->Node);
            else
                RemoveNodeFromDoubleLinkedList(&gCallbackTimerList, &CallbackTimer->Node);
            
            // clear and then enable the SOFTWARE_TIMER
            OS_SoftwareTimerClear(&(CallbackTimer->Timer));
            
            OS_SoftwareTimerEnable(&(CallbackTimer->Timer), Enable);
        }

		return OS_SUCCESS;
	}
#endif // end of #if (USING_CALLBACK_TIMER_ENABLE_METHOD == 1)
    
#if (USING_CALLBACK_TIMER_RESTART_FROM_ISR_METHOD == 1)
	// puts the current count to 0 and enables the counter if disabled
	OS_RESULT CallbackTimerRestartFromISR(CALLBACK_TIMER *CallbackTimer)
	{
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

		OS_SoftwareTimerClear(&CallbackTimer->Timer);

        // if it was off, turn it on
		if (OS_SoftwareTimerIsRunning(&CallbackTimer->Timer) == FALSE)
			CallbackTimerEnableFromISR(CallbackTimer, TRUE);

		return OS_SUCCESS;
	}
#endif // end of #if (USING_CALLBACK_TIMER_RESTART_FROM_ISR_METHOD == 1)
    
#if (USING_CALLBACK_TIMER_GET_TICKS_REMAINING_FROM_ISR_METHOD == 1)
	// gets how long until the method fires in ticks
	UINT32 CallbackTimerGetTicksRemainingFromISR(CALLBACK_TIMER *CallbackTimer)
	{
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return 0;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

		return CallbackTimer->PeriodicityInTicks - OS_SoftwareTimerGetTicks(&CallbackTimer->Timer);
	}
#endif // end of #if (USING_CALLBACK_TIMER_GET_TICKS_REMAINING_FROM_ISR_METHOD == 1)
    
#if (USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_FROM_ISR_METHOD == 1)
	UINT32 CallbackTimerGetPeriodicityInTicksFromISR(CALLBACK_TIMER *CallbackTimer)
	{
        UINT32 PeriodicityInTicks;
        
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return 0;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

		PeriodicityInTicks = CallbackTimer->PeriodicityInTicks;

		return PeriodicityInTicks;
	}
#endif // end of #if (USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_FROM_ISR_METHOD == 1)
    
#if (USING_CALLBACK_TIMER_SET_PERIODICITY_FROM_ISR_METHOD == 1)
	OS_RESULT CallbackTimerSetPeriodicityFromISR(CALLBACK_TIMER *CallbackTimer, UINT32 PeriodicityInTicks)
	{
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if (PeriodicityInTicks == 0)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

        if(OS_SoftwareTimerGetTicks(&CallbackTimer->Timer) < PeriodicityInTicks)
            CallbackTimer->PeriodicityInTicks = PeriodicityInTicks;
        else
            return OS_INVALID_OBJECT_STATE;

		return OS_SUCCESS;
	}
#endif // end of #if (USING_CALLBACK_TIMER_SET_PERIODICITY_FROM_ISR_METHOD == 1)
    
#if(USING_CALLBACK_TIMER_SET_CALLBACK_FROM_ISR_METHOD == 1)
    OS_RESULT CallbackTimerSetCallbackFromISR(CALLBACK_TIMER *CallbackTimer, CALLBACK_TIMER_CALLBACK CallbackTimerCallback)
	{
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if (ProgramAddressValid((OS_WORD)CallbackTimerCallback) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

		CallbackTimer->CallbackTimerCallback = CallbackTimerCallback;

		return OS_SUCCESS;
	}
#endif // end of #if(USING_CALLBACK_TIMER_SET_CALLBACK_FROM_ISR_METHOD == 1)
    
#if (USING_CALLBACK_TIMER_IS_RUNNING_FROM_ISR_METHOD == 1)
	BOOL CallbackTimerIsRunningFromISR(CALLBACK_TIMER *CallbackTimer)
	{
        #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)CallbackTimer) == FALSE)
                return FALSE;
        #endif // end of #if (USING_CHECK_CALLBACK_TIMER_PARAMETERS == 1)

		return SoftwareTimerIsRunningFromISR(&CallbackTimer->Timer);
	}
#endif // end of #if (USING_CALLBACK_TIMER_IS_RUNNING_FROM_ISR_METHOD == 1)