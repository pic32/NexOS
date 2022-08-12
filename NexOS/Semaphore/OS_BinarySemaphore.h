/*
    NexOS Kernel Version v1.01.04
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

#ifndef OSSEMAPHORE_H
	#define OSSEMAPHORE_H

#include "../Kernel/TaskObject.h"
#include "../Kernel/Kernel.h"
#include "../../Generic Libraries/Double Linked List/DoubleLinkedList.h"

/*
	OS_BINARY_SEMAPHORE Description:
		An OS_BINARY_SEMAPHORE is a normal BINARY_SEMAPHORE that is the base object for all other semaphore
		derivatives.  The BINARY_SEMAPHORE, COUNTING_SEMAPHORE, and MUTEX are all based off of the
		OS_BINARY_SEMAPHORE.

		The OS_BINARY_SEMAPHORE is only used by the OS and should not be used by the user.
*/

/*  The structure of a BINARY_SEMAPHORE */
typedef struct
{
	// The below is a variable to denote which TASK currently owns the semaphore.
	// If this variable is (TASK*)NULL no TASK owns the semaphore currently.
	TASK *Owner; 

	// This is the semaphores blocked list.
	DOUBLE_LINKED_LIST_HEAD BlockedListHead;
    
    #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
        UINT32 DeleteBlockCount;
    #endif // end of #if (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1)
    
}BINARY_SEMAPHORE;

/*
	BINARY_SEMAPHORE *OS_CreateBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, BOOL GetBinarySemaphore)

	Description: This method attempts to create and or initialize a 
	basic BINARY_SEMAPHORE.

	Blocking: No

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  If
		NULL is passed in this method will create a BINARY_SEMAPHORE object
		in the OS heap.

		BOOL GetBinarySemaphore - If TRUE the calling TASK will 
		own the BINARY_SEMAPHORE once created.  Otherwise the BINARY_SEMAPHORE 
		is just created and not owned by any TASK.

	Returns: 
		BINARY_SEMAPHORE * - A pointer to the BINARY_SEMAPHORE upon success, otherwise
		a (BINARY_SEMAPHORE*)NULL pointer will be returned.

	Notes:
		- Only the OS calls this method.

	See Also:
		- OS_BinarySemaphoreDelete()
*/
BINARY_SEMAPHORE *OS_CreateBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, BOOL GetBinarySemaphore);

/*
	OS_RESULT OS_TryGetBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, INT32 TimeoutInTicks)

	Description: This method attempts to obtain a basic BINARY_SEMAPHORE.  If 
	the SEMPAHORE is owned by another TASK, and a Timeout is specified,
	this method will place the current TASK on the Delay Queue.  This
	method needs to be called before OS_BinarySemaphoreWait().

	Blocking: No

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This 
		BINARY_SEMAPHORE must have been created/initialized with a call to
		OS_CreateBinarySemaphore().

		INT32 TimeoutInTicks - This is a timeout value in ticks to wait 
		for the BINARY_SEMAPHORE should it be owned by another TASK.  Below 
		are valid values for TimeoutInTicks.

			TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
			Delayed Queue up to the specified number of ticks if
			the BINARY_SEMAPHORE is not obtained from this method.

			TimeoutInTicks = 0: The calling TASK will not be placed on the 
			Delay Queue and this method will return OS_RESOURCE_GET_FAILED.

			TimeoutInTicks <= -1: The calling TASK will not be placed on the
			Delay Queue and will wait forever.

	Returns: 
		OS_SUCCESS - If the BINARY_SEMAPHORE was successfully obtained or
		already owned by the calling TASK.
		
		OS_RESOURCE_GET_FAILED - If the BINARY_SEMAPHORE was not obtained 
		and no delay was specified.

		OS_RESOURCE_GET_BLOCKED - If the BINARY_SEMAPHORE was not obtained 
		but a delay was specified.

	Notes:
		- Only the OS calls this method.

		- This method must be called before OS_BinarySemaphoreWait().
 
 		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- OS_BinarySemaphoreWait()
*/
OS_RESULT OS_TryGetBinarySemaphore(	BINARY_SEMAPHORE *BinarySemaphore

									#if (USING_TASK_DELAY_TICKS_METHOD == 1)
										,INT32 TimeoutInTicks
									#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
										
									);

/*
	OS_RESULT OS_BinarySemaphoreWait(BINARY_SEMAPHORE *BinarySemaphore)

	Description: This method waits for the specified BINARY_SEMAPHORE
	to become available.

	Blocking: Yes

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		OS_CreateBinarySemaphore().

	Returns:
		OS_SUCCESS - If the BINARY_SEMAPHORE was successfully obtained or
		already owned by the calling TASK.

		OS_RESOURCE_GET_FAILED - If the BINARY_SEMAPHORE was not obtained
		and no delay was specified.

		OS_RESOURCE_GET_BLOCKED - If the BINARY_SEMAPHORE was not obtained
		but a delay was specified.

	Notes:
		- Only the OS calls this method.
		- A call to OS_TryGetBinarySemaphore() must be made before this method.

	See Also:
		- OS_TryGetBinarySemaphore()
*/
OS_RESULT OS_BinarySemaphoreWait(BINARY_SEMAPHORE *BinarySemaphore);

/*
	BOOL OS_ReleaseBinarySemaphore(BINARY_SEMAPHORE* BinarySemaphore, OS_RESULT *Outcome)

	Description: This method attempts to release the specified BINARY_SEMAPHORE.

	Blocking: No

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		OS_CreateBinarySemaphore().  This BINARY_SEMAPHORE must have already been 
		obtained by the calling task with a valid call to either 
		OS_TryGetBinarySemaphore() or OS_BinarySemaphoreWait().

		OS_RESULT Result - A pointer to a valid OS_RESULT.  This will
		be set to OS_RESOURCE_INVALID_OWNER if the current TASK does
		not own the specified BINARY_SEMAPHORE.  Otherwise the BINARY_SEMAPHORE will
		be successfully released and this will be se to OS_SUCCESS.

	Returns:
		TRUE - If BINARY_SEMAPHORE was released and a TASK with a higher
		priority than the current TASK was added to the Ready Queue.
		In which case a call to SurrenderCPU() should be made after
		this method.

		FALSE - If the BINARY_SEMAPHORE was not released, or if it was released
		and a TASK with a lower priority than the current TASK was added
		the the Ready Queue.

	Notes:
		- The BINARY_SEMAPHORE must have already been successfully obtained
		  with a call to either OS_TryGetBinarySemaphore() or OS_BinarySemaphoreWait().
		- Only the OS calls this method.

	See Also:
		- OS_TryGetBinarySemaphore()
*/
BOOL OS_ReleaseBinarySemaphore(BINARY_SEMAPHORE* BinarySemaphore, OS_RESULT *Result);

/*
	TASK *OS_BinarySemaphoreGetOwner(BINARY_SEMAPHORE *BinarySemaphore)

	Description: This method returns the current TASK owner of the BINARY_SEMAPHORE.

	Blocking: No

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		OS_CreateBinarySemaphore().

	Returns:
		TASK* - A pointer to the TASK which currently owns the BINARY_SEMAPHORE.
		If no TASK owns the BINARY_SEMAPHORE a (TASK*)NULL value is returned.

	Notes:
		- Only the OS calls this method.

	See Also:
		- None
*/
TASK *OS_BinarySemaphoreGetOwner(BINARY_SEMAPHORE *BinarySemaphore);

/*
	OS_RESULT OS_BinarySemaphoreDelete(BINARY_SEMAPHORE *BinarySemaphore)

	Description: This method attempts to delete the specified BINARY_SEMAPHORE.

	Blocking: No

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		OS_CreateBinarySemaphore().

	Returns:
		OS_RESULT - This method will return one of three values based
		on the outcome of the requested delete operation.
			
			OS_SUCCESS - If the BINARY_SEMAPHORE was successfully deleted.

			OS_RESOURCE_NOT_IN_OS_HEAP - If the BINARY_SEMAPHORE was not
			located within the OS heap.  The BINARY_SEMAPHORE will not have
			been deleted.

			OS_RESOURCE_IN_USE - If the BINARY_SEMAPHORE is currently owned
			by a TASK.  The BINARY_SEMAPHORE will not have been deleted.

	Notes:
		A BINARY_SEMAPHORE must not be currently owned by a TASK in order
		to be deleted.  Any TASK can delete a BINARY_SEMAPHORE.  That is to
		say that the BINARY_SEMAPHORE does not need to be deleted by the 
		TASK which created it.

	Notes:
		- Only the OS calls this method.

	See Also:
		- OS_CreateBinarySemaphore()
*/
OS_RESULT OS_BinarySemaphoreDelete(BINARY_SEMAPHORE *BinarySemaphore);

/*
	BOOL OS_BinarySemaphoreTaken(BINARY_SEMAPHORE *BinarySemaphore)

	Description: This method returns if the BINARY_SEMAPHORE is currently owned
	by another TASK.

	Blocking: No

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		OS_CreateBinarySemaphore().

	Returns:
		BOOL - TRUE if the BINARY_SEMAPHORE is taken, FALSE otherwise.

	Notes:
		- Only the OS calls this method.

	See Also:
		- OS_ReleaseBinarySemaphore(), OS_TryGetBinarySemaphore()
*/
BOOL OS_BinarySemaphoreTaken(BINARY_SEMAPHORE *BinarySemaphore);

/*
	UINT32 OS_BinarySempahoreGetDeleteCount(BINARY_SEMAPHORE *BinarySemaphore)

	Description: This method will return the DeleteBlockCount of the BINARY_SEMAPHORE.
    A value other than zero means a TASK is using the BINARY_SEMAPHORE and it cannot
    be deleted at this time.

	Blocking: No

	User Callable: No

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		OS_CreateBinarySemaphore().

	Returns:
		UINT32 - The DeleteBlockCount of the BINARY_SEMAPHORE. 

	Notes:
		- Only the OS calls this method.

	See Also:
		- OS_BinarySemaphoreDelete()
*/
UINT32 OS_BinarySempahoreGetDeleteCount(BINARY_SEMAPHORE *BinarySemaphore);

#endif // end of #ifndef OSSEMAPHORE_H