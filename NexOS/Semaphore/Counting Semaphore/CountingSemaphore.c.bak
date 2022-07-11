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

#include "CriticalSection.h"
#include "../../Kernel/Memory.h"
#include "../../Kernel/Task.h"
#include "CountingSemaphore.h"

extern TASK * volatile gCurrentTask;

COUNTING_SEMAPHORE *CreateCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore, BOOL GetCountingSemaphore)
{
	COUNTING_SEMAPHORE *NewCountingSemaphore;

    #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
        // if it is not NULL, and not in valid RAM, it is an error
        if (CountingSemaphore != (COUNTING_SEMAPHORE*)NULL)
            if (RAMAddressValid((OS_WORD)CountingSemaphore) == FALSE)
                return (COUNTING_SEMAPHORE*)NULL;
    #endif // end of #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)

	if (CountingSemaphore != (COUNTING_SEMAPHORE*)NULL)
	{
		NewCountingSemaphore = CountingSemaphore;
	}
	else
	{
		if ((NewCountingSemaphore = (COUNTING_SEMAPHORE*)AllocateMemory(sizeof(COUNTING_SEMAPHORE))) == (COUNTING_SEMAPHORE*)NULL)
		{
			return (COUNTING_SEMAPHORE*)NULL;
		}
	}

	if (CreateBinarySemaphore(&NewCountingSemaphore->BinarySemaphore, GetCountingSemaphore) == (BINARY_SEMAPHORE*)NULL)
	{
		// only delete it if we allocated for a new one
		if (CountingSemaphore == (COUNTING_SEMAPHORE*)NULL)
			ReleaseMemory((void*)NewCountingSemaphore);

		NewCountingSemaphore = (COUNTING_SEMAPHORE*)NULL;
	}
	else
	{
		// if they wanted to get it, then set the count to 1
		if (GetCountingSemaphore == TRUE)
			NewCountingSemaphore->Count = 1;
		else
			NewCountingSemaphore->Count = 0;
	}

	return NewCountingSemaphore;
}

OS_RESULT GetCountingSemaphore(	COUNTING_SEMAPHORE *CountingSemaphore

								#if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, INT32 TimeoutInTicks
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

								)
{
	OS_RESULT Result;

	EnterCritical();

	Result = GetBinarySemaphore(&CountingSemaphore->BinarySemaphore

								#if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, TimeoutInTicks
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
		
								);

	if (Result == OS_SUCCESS)
		CountingSemaphore->Count++;

	ExitCritical();

	return Result;
}

OS_RESULT ReleaseCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore)
{
    TASK *Owner;
	OS_RESULT Result = OS_INVALID_ARGUMENT;

    #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
        if (RAMAddressValid((OS_WORD)CountingSemaphore) == FALSE)
            return OS_INVALID_ARGUMENT_ADDRESS;
    #endif // end of #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
    
	EnterCritical();
    
	Owner = OS_BinarySemaphoreGetOwner(&CountingSemaphore->BinarySemaphore);

	if (Owner != (TASK*)NULL)
	{
		// we can only release if we are the owner
		if (gCurrentTask == Owner)
		{
			CountingSemaphore->Count--;

			// check to see if we are back to zero
			if (CountingSemaphore->Count == 0)
				ReleaseBinarySemaphore(&CountingSemaphore->BinarySemaphore);

			Result = OS_SUCCESS;
		}
		else
		{
			Result = OS_RESOURCE_INVALID_OWNER;
		}
	}

	ExitCritical();

	return Result;
}

#if (USING_RELEASE_UNCONDITIONALLY_COUNTING_SEMAPHORE_METHOD == 1)
	OS_RESULT ReleaseUnconditionallyCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore)
	{
        TASK *Owner;
		OS_RESULT Result = OS_INVALID_ARGUMENT;
        
        #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)CountingSemaphore) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
        
		EnterCritical();

		Owner = GetBinarySemaphoreOwner(&CountingSemaphore->BinarySemaphore);

		if (Owner != (TASK*)NULL)
		{
			// we can only release if we are the owner
			if (gCurrentTask == Owner)
			{
				// just clear the count and release the counting semaphore
				CountingSemaphore->Count = 0;

				ReleaseBinarySemaphore(&CountingSemaphore->BinarySemaphore);

				Result = OS_SUCCESS;
			}
			else
			{
				Result = OS_RESOURCE_INVALID_OWNER;
			}
		}

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_RELEASE_UNCONDITIONALLY_COUNTING_SEMAPHORE_METHOD == 1)

#if (USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1)
	OS_RESULT DeleteCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore)
	{
		OS_RESULT Result;

        #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
            if (AddressInHeap((OS_WORD)CountingSemaphore) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
        #endif // end of #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)

		EnterCritical();
        
        // is someone using it?
        if(OS_BinarySempahoreGetDeleteCount(&CountingSemaphore->BinarySemaphore) != 0)
        {
            ExitCritical();
            
            return OS_RESOURCE_IN_USE;
        }

        if (GetBinarySemaphoreTaken(&CountingSemaphore->BinarySemaphore) == FALSE)
        {
            // now delete it
            if (ReleaseMemory((void*)CountingSemaphore) == TRUE)
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
#endif // end of USING_DELETE_COUNTING_SEMAPHORE_METHOD

#if (USING_GET_COUNTING_SEMAPHORE_OWNER_METHOD == 1)
	TASK *GetCountingSemaphoreOwner(COUNTING_SEMAPHORE *CountingSemaphore)
	{
        #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)CountingSemaphore) == FALSE)
                return (TASK*)NULL;
        #endif // end of #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)

		return GetBinarySemaphoreOwner(&CountingSemaphore->BinarySemaphore);
	}
#endif // end of USING_GET_COUNTING_SEMAPHORE_OWNER_METHOD

#if (USING_GET_COUNTING_SEMAPHORE_TAKEN_METHOD == 1)
	BOOL GetCountingSemaphoreTaken(COUNTING_SEMAPHORE *CountingSemaphore)
	{
        #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)CountingSemaphore) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)

		return GetBinarySemaphoreTaken(&CountingSemaphore->BinarySemaphore);
	}
#endif // end of USING_GET_COUNTING_SEMAPHORE_TAKEN_METHOD

#if (USING_GET_COUNTING_SEMAPHORE_COUNT_METHOD == 1)
	UINT32 GetCountingSemaphoreCount(COUNTING_SEMAPHORE *CountingSemaphore)
	{
        UINT32 Count;
        
        #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)CountingSemaphore) == FALSE)
                return 0;
        #endif // end of #if (USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS == 1)

		EnterCritical();

		Count = CountingSemaphore->Count;

		ExitCritical();

		return Count;
	}
#endif // end of
