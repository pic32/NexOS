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

#include "Mutex.h"
#include "CriticalSection.h"
#include "../../Kernel/Memory.h"
#include "../../Kernel/Task.h"

extern TASK * volatile gCurrentTask;

// This method returns TRUE if it added a TASK with a higher priority than
// the current system priority.
static BOOL OS_ReleaseMutex(MUTEX* Mutex)
{
	OS_RESULT Result;
	BOOL CallNextTask = FALSE;

	// was the owners priority changed during the ownership?
	if (Mutex->OriginalPriority != INVALID_TASK_PRIORITY)
		if (OS_ChangeTaskPriority(OS_BinarySemaphoreGetOwner(&Mutex->BinarySemaphore), Mutex->OriginalPriority) == TRUE)
			CallNextTask = TRUE;

	// now we can release the semaphore
	if (OS_ReleaseBinarySemaphore(&Mutex->BinarySemaphore, &Result) == TRUE)
		CallNextTask = TRUE;

	// reset the priority back to the value which denotes unchanged priority
	Mutex->OriginalPriority = INVALID_TASK_PRIORITY;

	return CallNextTask;
}

MUTEX *CreateMutex(MUTEX *Mutex, BOOL GetMutex)
{
	MUTEX *NewMutex;

    #if (USING_CHECK_MUTEX_PARAMETERS == 1)
        // if it is not NULL, and not in valid RAM, it is an error
        if (Mutex != (MUTEX*)NULL)
            if (RAMAddressValid((OS_WORD)Mutex) == FALSE)
                return (MUTEX*)NULL;
    #endif // end of #if (USING_CHECK_MUTEX_PARAMETERS == 1)

	if (Mutex != (MUTEX*)NULL)
	{
		NewMutex = Mutex;
	}
	else
	{
		if ((NewMutex = (MUTEX*)AllocateMemory(sizeof(MUTEX))) == (MUTEX*)NULL)
		{
			// Wasn't enough room return NULL
			return (MUTEX*)NULL;
		}
	}

	if (OS_CreateBinarySemaphore(&NewMutex->BinarySemaphore, GetMutex) == (BINARY_SEMAPHORE*)NULL)
	{
		// only delete it if we allocated for a new one
		if (Mutex == (MUTEX*)NULL)
			ReleaseMemory((void*)NewMutex);

		NewMutex = (MUTEX*)NULL;
	}
	else
	{
		NewMutex->OriginalPriority = INVALID_TASK_PRIORITY;
	}
	
	return NewMutex;
}

OS_RESULT GetMutex(	MUTEX *Mutex
	
					#if (USING_TASK_DELAY_TICKS_METHOD == 1)
						, INT32 TimeoutInTicks
					#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

						)
{
	OS_RESULT Result;

    #if (USING_CHECK_MUTEX_PARAMETERS == 1)
        if (RAMAddressValid((OS_WORD)Mutex) == FALSE)
            return OS_INVALID_ARGUMENT_ADDRESS;
    #endif // end of #if (USING_CHECK_MUTEX_PARAMETERS == 1)

	EnterCritical();

	// attempt to get the semaphore
	Result = OS_TryGetBinarySemaphore(	&Mutex->BinarySemaphore

										#if (USING_TASK_DELAY_TICKS_METHOD == 1)
											, TimeoutInTicks
										#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

										);
	// did we get the semaphore?
	if (Result == OS_RESOURCE_GET_BLOCKED)
	{
		// we couldn't get the BinarySemaphore
		TASK *CurrentOwner = OS_BinarySemaphoreGetOwner(&Mutex->BinarySemaphore);

		// now this is the unique aspect of the Mutex
		// make the holding task higher priority if we are
		if (CurrentOwner->TaskInfo.bits.Priority < gCurrentTask->TaskInfo.bits.Priority)
		{
			// The TASK that owns the Mutex has a lower priority than the 
			// TASK that tried to get the Mutex.  This means there needs
			// to be some priority adjustments.

			// we will preserve the priority now of the mutex owner, not on mutex get.
			// Why?  After a mutex get, we may change our priority.  This ensures that
			// if we change our priority only when we need to, it is closer to what it 
			// was incase it changed.  We only want to preserve the priority if it wasn't
			// already modified from another TASK on the waiting list.
			if (Mutex->OriginalPriority == INVALID_TASK_PRIORITY)
				Mutex->OriginalPriority = CurrentOwner->TaskInfo.bits.Priority;

			// the owner is of lower priority, make them the same
			// priority as us!
			OS_ChangeTaskPriority(CurrentOwner, gCurrentTask->TaskInfo.bits.Priority);
		}

		// this function will automatically block, so we don't care about
		// the return value of OS_SwapTaskPriority().
		Result = OS_BinarySemaphoreWait(&Mutex->BinarySemaphore);
	}

	ExitCritical();

	return Result;
}		

OS_RESULT ReleaseMutex(MUTEX *Mutex)
{
	OS_RESULT Result;

    #if (USING_CHECK_MUTEX_PARAMETERS == 1)
        if (RAMAddressValid((OS_WORD)Mutex) == FALSE)
            return OS_INVALID_ARGUMENT_ADDRESS;
    #endif // end of #if (USING_CHECK_MUTEX_PARAMETERS == 1)

	EnterCritical();

	// If the gCurrentTask isn't the owner of the Mutex then just return
	if (gCurrentTask != OS_BinarySemaphoreGetOwner(&Mutex->BinarySemaphore))
	{
		Result = OS_RESOURCE_INVALID_OWNER;
	}
	else
	{
		// release the mutex and give it to the next highest
		// task waiting on it (if there)
		if (OS_ReleaseMutex(Mutex) == TRUE)
			SurrenderCPU();

		Result = OS_SUCCESS;
	}

	ExitCritical();

	return Result;
}

#if (USING_DELETE_MUTEX_METHOD == 1)
	OS_RESULT DeleteMutex(MUTEX *Mutex)
	{
		OS_RESULT Result;

        #if (USING_CHECK_MUTEX_PARAMETERS == 1)
            if (AddressInHeap((OS_WORD)Mutex) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
        #endif // end of #if (USING_CHECK_MUTEX_PARAMETERS == 1)

		EnterCritical();
        
        if(OS_BinarySempahoreGetDeleteCount(&Mutex->BinarySemaphore) != 0)
        {
            ExitCritical();
            
            return OS_RESOURCE_IN_USE;
        }

		if (OS_BinarySemaphoreTaken(&Mutex->BinarySemaphore) == FALSE)
		{
            if (ReleaseMemory((void*)Mutex) == TRUE)
                Result = OS_SUCCESS;
            else
                Result = OS_RESOURCE_NOT_IN_OS_HEAP;
		}
		else
		{
			Result = OS_RESOURCE_IN_USE;
		}
	
		ExitCritical();
	
		return Result;
	}
#endif // end of USING_DELETE_MUTEX_METHOD

#if (USING_GET_MUTEX_OWNER_METHOD == 1)
    TASK *OS_GetMutexOwner(MUTEX *Mutex)
    {
        return OS_BinarySemaphoreGetOwner(&Mutex->BinarySemaphore);
    }
    
	TASK *GetMutexOwner(MUTEX *Mutex)
	{
        TASK *Owner;
        
        #if (USING_CHECK_MUTEX_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)Mutex) == FALSE)
                return (TASK*)NULL;
        #endif // end of #if (USING_CHECK_MUTEX_PARAMETERS == 1)

		EnterCritical();

		// even if NULL, this will be returned
		Owner = OS_GetMutexOwner(Mutex);

		ExitCritical();
	
		return Owner;
	}
#endif // end of USING_GET_MUTEX_OWNER_METHOD

#if (USING_GET_MUTEX_TAKEN_METHOD == 1)
	BOOL GetMutexTaken(MUTEX *Mutex)
	{
        BOOL Taken;
        
        #if (USING_CHECK_MUTEX_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)Mutex) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_MUTEX_PARAMETERS == 1)

		EnterCritical();

		Taken = OS_BinarySemaphoreTaken(&Mutex->BinarySemaphore);

		ExitCritical();

		return Taken;
	}
#endif // end of USING_GET_MUTEX_TAKEN_METHOD
