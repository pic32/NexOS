/*
    NexOS Kernel Version v1.01.02
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

#include "../Kernel/Kernel.h"
#include "../Kernel/Memory.h"
#include "../Kernel/Task.h"
#include "OS_BinarySemaphore.h"

extern TASK * volatile gCurrentTask;

BINARY_SEMAPHORE *OS_CreateBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, BOOL GetBinarySemaphore)
{
	BINARY_SEMAPHORE *NewBinarySemaphore;

	if (BinarySemaphore)
	{
		NewBinarySemaphore = BinarySemaphore;
	}
	else
	{
		if ((NewBinarySemaphore = (BINARY_SEMAPHORE*)AllocateMemory(sizeof(BINARY_SEMAPHORE))) == (BINARY_SEMAPHORE*)NULL)
		{
			// Wasn't enough room return NULL

			return (BINARY_SEMAPHORE*)NULL;
		}
	}

	// Initialize NewBinarySemaphore with default values
	if (GetBinarySemaphore == TRUE)
		NewBinarySemaphore->Owner = gCurrentTask;
	else
		NewBinarySemaphore->Owner = (TASK*)NULL;
    
    #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
        NewBinarySemaphore->DeleteBlockCount = 0;
    #endif // end of #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)

	InitializeDoubleLinkedListHead(&NewBinarySemaphore->BlockedListHead);

	return NewBinarySemaphore;
}

OS_RESULT OS_TryGetBinarySemaphore(	BINARY_SEMAPHORE *BinarySemaphore 
	
									#if (USING_TASK_DELAY_TICKS_METHOD == 1)
										,INT32 TimeoutInTicks
									#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
	
									)
{
	// Check to see if we already have the BinarySemaphore
	if (gCurrentTask != BinarySemaphore->Owner)
	{
		// Check to see if the BinarySemaphore isn't taken
		if (BinarySemaphore->Owner == (TASK*)NULL)
		{
			// The BinarySemaphore isn't taken, so just take it
			BinarySemaphore->Owner = gCurrentTask;

			// now the calling method knows not to block
			return OS_SUCCESS;
		}
		else
		{
			#if (USING_TASK_DELAY_TICKS_METHOD == 1)
				// if the user didn't want to block then leave since we couldn't get the semaphore
				if (TimeoutInTicks == 0)
				{
					return OS_RESOURCE_GET_FAILED;
				}
				else
			#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
			{
				// The BinarySemaphore is taken, so make the current task state blocked
				OS_PlaceTaskOnBlockedList(gCurrentTask, &(BinarySemaphore->BlockedListHead), &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], BLOCKED, TRUE);

				#if (USING_TASK_DELAY_TICKS_METHOD == 1)
					if (TimeoutInTicks > (INT32)0)
					{
						// place on timer list
						OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], (UINT32)TimeoutInTicks, FALSE);
					}
				#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
			}

			// the calling method knows to block
			return OS_RESOURCE_GET_BLOCKED;
		}
	}

	// now the calling method knows not to block
	return OS_SUCCESS;
}

OS_RESULT OS_BinarySemaphoreWait(BINARY_SEMAPHORE *BinarySemaphore)
{
    #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
        BinarySemaphore->DeleteBlockCount++;
        
        #if(USING_DELETE_TASK == 1)
            gCurrentTask->DeleteBlockCounter = &BinarySemaphore->DeleteBlockCount;
        #endif // end of #if(USING_DELETE_TASK == 1)
    #endif // end of #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
    
	SurrenderCPU();
    
    #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
        BinarySemaphore->DeleteBlockCount--;
        
        #if(USING_DELETE_TASK == 1)
            gCurrentTask->DeleteBlockCounter = (UINT32*)NULL;
        #endif // end of #if(USING_DELETE_TASK == 1)
    #endif // end of #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)

	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		gCurrentTask->DelayInTicks = 0;

		if (BinarySemaphore->Owner != gCurrentTask)
		{
			// must have timed out
			return OS_RESOURCE_GET_TIMEOUT;
		}
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	return OS_SUCCESS;
}

// This method returns TRUE if it added a TASK with a higher priority than
// the current system priority.
BOOL OS_ReleaseBinarySemaphore(BINARY_SEMAPHORE* BinarySemaphore, OS_RESULT *Result)
{
	TASK *TaskHandle;
	DOUBLE_LINKED_LIST_NODE *Node;

	if(gCurrentTask != BinarySemaphore->Owner)
    {
		*Result = OS_RESOURCE_INVALID_OWNER;

		return FALSE;
    }

	*Result = OS_SUCCESS;

	// If the BinarySemaphore Blocked List is NULL, just release the BinarySemaphore
	if (BinarySemaphore->BlockedListHead.Beginning == (DOUBLE_LINKED_LIST_NODE*)NULL)
	{
		BinarySemaphore->Owner = (TASK*)NULL;

		return FALSE;
	}

	/*
	If we made it to this point in the code then there was a Blocked List
	for the BinarySemaphore.  So we have to get the Task with the highest priority
	out of the BinarySemaphores Blocked List, and give that Task the BinarySemaphore.
	*/

	// This removes the SystemQueueTaskNode from the BinarySemaphore Blocked List
	Node = OS_PriorityRemoveDoubleLinkedList(&BinarySemaphore->BlockedListHead);

	TaskHandle = (TASK*)Node->Data;

	// The new owner of the BinarySemaphore is the Task that we found on the BinarySemaphores Blocked List with the highest Priority
	BinarySemaphore->Owner = TaskHandle;

	return OS_AddTaskToReadyQueue((TASK*)Node->Data);
}

TASK *OS_BinarySemaphoreGetOwner(BINARY_SEMAPHORE *BinarySemaphore)
{
	return BinarySemaphore->Owner;
}

#if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
    OS_RESULT OS_BinarySemaphoreDelete(BINARY_SEMAPHORE *BinarySemaphore)
    {
        #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
            if(BinarySemaphore->DeleteBlockCount != 0)
                return OS_RESOURCE_IN_USE;
        #endif // end of #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)

        if (BinarySemaphore->Owner == (TASK*)NULL)
        {
            // first check that it is in the heap prior to clearing it
            if (AddressInHeap((OS_WORD)BinarySemaphore) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
            
            if (ReleaseMemory((void*)BinarySemaphore) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;

            return OS_SUCCESS;
        }
        else
        {
            return OS_RESOURCE_IN_USE;
        }
    }
#endif // end of #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)

BOOL OS_BinarySemaphoreTaken(BINARY_SEMAPHORE *BinarySemaphore)
{
	return (BOOL)(BinarySemaphore->Owner != (TASK*)NULL);
}

#if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1)
    UINT32 OS_BinarySempahoreGetDeleteCount(BINARY_SEMAPHORE *BinarySemaphore)
    {
        return BinarySemaphore->DeleteBlockCount;
    }
#endif // end of #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1)