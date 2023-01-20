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

#include "CriticalSection.h"
#include "../../Kernel/Memory.h"
#include "../../Kernel/Task.h"
#include "BinarySemaphore.h"

extern TASK * volatile gCurrentTask;

BINARY_SEMAPHORE *CreateBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, BOOL GetBinarySemaphore)
{
    #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)
        // if it is not NULL, and not in valid RAM, it is an error
        if (BinarySemaphore != (BINARY_SEMAPHORE*)NULL)
            if (RAMAddressValid((OS_WORD)BinarySemaphore) == FALSE)
                return (BINARY_SEMAPHORE*)NULL;
    #endif // end of #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)

	BinarySemaphore = OS_CreateBinarySemaphore(BinarySemaphore, GetBinarySemaphore);
	
	return BinarySemaphore;
}

OS_RESULT GetBinarySemaphore(	BINARY_SEMAPHORE *BinarySemaphore

								#if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, INT32 TimeoutInTicks
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

								)
{
	#if (USING_BINARY_SEMAPHORE_STARVATION_PROTECTION == 1)
		BYTE OriginalTaskPriority;
	#endif // end of USING_BINARY_SEMAPHORE_STARVATION_PROTECTION

	OS_RESULT Result;

    #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)
        if (AddressInHeap((OS_WORD)BinarySemaphore) == FALSE)
            return OS_INVALID_ARGUMENT_ADDRESS;
    #endif // end of #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)

	EnterCritical();

	Result = OS_TryGetBinarySemaphore(	BinarySemaphore

										#if (USING_TASK_DELAY_TICKS_METHOD == 1)
											, TimeoutInTicks
										#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
		
										);

	if (Result == OS_RESOURCE_GET_BLOCKED)
	{
		#if (USING_BINARY_SEMAPHORE_STARVATION_PROTECTION == 1)
			OriginalTaskPriority = gCurrentTask->TaskInfo.bits.Priority;
		#endif // end of USING_BINARY_SEMAPHORE_STARVATION_PROTECTION

        // this method is blocking until we get the BINARY_SEMAPHORE
		Result = OS_BinarySemaphoreWait(BinarySemaphore);

		#if (USING_BINARY_SEMAPHORE_STARVATION_PROTECTION == 1)
			// Did the TASK's priority get increased while blocked?
			if (gCurrentTask->TaskInfo.bits.Priority != OriginalTaskPriority)
			{
				// The current task had it's priority increased while on the blocked list.
				// Put the priority back to normal.
				if (OS_ChangeTaskPriority(gCurrentTask, OriginalTaskPriority) == TRUE)
					SurrenderCPU();
			}
		#endif // end of USING_BINARY_SEMAPHORE_STARVATION_PROTECTION
	}

	ExitCritical();

	return Result;
}		

OS_RESULT ReleaseBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore)
{
	OS_RESULT Result;

    #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)
        if (AddressInHeap((OS_WORD)BinarySemaphore) == FALSE)
            return OS_INVALID_ARGUMENT_ADDRESS;
    #endif // end of #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)

	EnterCritical();

	// If the gCurrentTask isn't the owner of the BinarySemaphore then just return
	if (OS_ReleaseBinarySemaphore(BinarySemaphore, &Result) == TRUE)
	{
		// There is a list on the BinarySemaphore Blocked List, increment their priorities by 1.
		#if (USING_BINARY_SEMAPHORE_STARVATION_PROTECTION == 1)
			OS_IncrementTaskListPriorities(&BinarySemaphore->BlockedListHead);
		#endif // end of USING_BINARY_SEMAPHORE_STARVATION_PROTECTION

		SurrenderCPU();
	}

	ExitCritical();

	return Result;
}

#if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
	OS_RESULT DeleteBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore)
	{
		OS_RESULT Result;

        #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)BinarySemaphore) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)

		EnterCritical();
	
		Result = OS_BinarySemaphoreDelete(BinarySemaphore);
	
		ExitCritical();
	
		return Result;
	}
#endif // end of USING_DELETE_BINARY_SEMAPHORE_METHOD

#if (USING_GET_BINARY_SEMAPHORE_OWNER_METHOD == 1)
	TASK *GetBinarySemaphoreOwner(BINARY_SEMAPHORE *BinarySemaphore)
	{
        TASK *Owner;
        
        #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)BinarySemaphore) == FALSE)
                return (TASK*)NULL;
        #endif // end of #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)

		EnterCritical();
	
		Owner = OS_BinarySemaphoreGetOwner(BinarySemaphore);
	
		ExitCritical();
	
		return Owner;
	}
#endif // end of USING_GET_BINARY_SEMAPHORE_OWNER_METHOD

#if (USING_GET_BINARY_SEMAPHORE_TAKEN_METHOD == 1)
	BOOL GetBinarySemaphoreTaken(BINARY_SEMAPHORE *BinarySemaphore)
	{
        BOOL Taken;
        
        #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)BinarySemaphore) == FALSE)
                return FALSE;
        #endif // end of #if (USING_CHECK_BINARY_SEMAPHORE_PARAMETERS == 1)

		EnterCritical();

		Taken = OS_BinarySemaphoreTaken(BinarySemaphore);

		ExitCritical();

		return Taken;
	}
#endif // end of USING_GET_BINARY_SEMAPHORE_TAKEN_METHOD