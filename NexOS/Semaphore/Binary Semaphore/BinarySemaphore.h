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

/*
	BINARY_SEMAPHORE Description:
		A BINARY_SEMAPHORE can be used to control access to an object or area of code.  Only one TASK at a
		time can own the semaphore.  If TASK 1 tries to get the BINARY_SEMAPHORE when TASK 2 already
		owns it, TASK 1 will block until TASK 2 releases the BINARY_SEMAPHORE.  When a BINARY_SEMAPHORE is
		released it goes to the TASK on the BINARY_SEMAPHORE's blocked list which has the highest priority.

		This can cause starvation of a TASK getting the BINARY_SEMAPHORE.  To prevent a TASK from getting 
		starved of the BINARY_SEMAPHORE the starvation protection mechanism should be enabled.  This is
		done by setting USING_BINARY_SEMAPHORE_STARVATION_PROTECTION in RTOSConfig.h to a 1.  Each time a
		BINARY_SEMAPHORE is released, all the TASKs on the blocked list get their priorites increased by 1.
		Eventually a TASK that would have been starved will get the BINARY_SEMAPHORE.  Once the TASK gets 
		a BINARY_SEMAPHORE while it was on the blocked list, it's priority will return to whatever it was
		when it first called GetBinarySemaphore().
*/

#ifndef BINARY_SEMAPHORE_H
    #define BINARY_SEMAPHORE_H

#include "../../Kernel/TaskObject.h"
#include "../OS_BinarySemaphore.h"

#if (USING_BINARY_SEMAPHORES != 1)
	#error "USING_BINARY_SEMAPHORES must be defined as a 1 in RTOSConfig.h to use Binary Semaphores"
#endif // end of #if (USING_BINARY_SEMAPHORES != 1)

/*
	BINARY_SEMAPHORE *CreateBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, BOOL GetSemaphore)

	Description: This method attempts to create and or initialize a basic BINARY_SEMAPHORE.

	Blocking: No

	User Callable: Yes

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  If
		NULL is passed in this method will create a BINARY_SEMAPHORE object
		in the OS heap.

		BOOL GetBinarySemaphore - If TRUE the calling TASK will
		own the BINARY_SEMAPHORE once created.  Otherwise the BINARY_SEMAPHORE
		is just created and not owned by any TASK.

	Returns: 
		BINARY_SEMAPHORE * - A pointer to a valid BINARY_SEMAPHORE upon success, otherwise
		a (BINARY_SEMAPHORE*)NULL pointer is returned.

	Notes:
		- None

	See Also:
		- DeleteBinarySemaphore()
*/
BINARY_SEMAPHORE *CreateBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, BOOL GetSemaphore);

/*
	OS_RESULT GetBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore, INT32 TimeoutInTicks)

	Description: This method attempts to obtain a basic BINARY_SEMAPHORE.  If
	the BINARY_SEMPAHORE is owned by another TASK, and a Timeout is specified,
	this method will place the current TASK on the Delay Queue.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		CreateBinarySemaphore().

		INT32 TimeoutInTicks - This is a timeout value in ticks to wait
		for the BINARY_SEMAPHORE should it be owned by another TASK.  Below
		are valid values for TimeoutInTicks.

			TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
			Delayed Queue up to the specified number of ticks if
			the BINARY_SEMAPHORE is not obtained from this method.

			TimeoutInTicks = 0: The calling TASK will not be placed on the
			Delay Queue and this method will return OS_RESOURCE_GET_FAILED if
            the BINARY_SEMAPHORE is already owned by another TASK.

			TimeoutInTicks <= -1: The calling TASK will not be placed on the
			Delay Queue and will wait forever if the BINARY_SEMAPHORE is already 
            owned by another TASK.

	Returns:
		OS_SUCCESS - If the BINARY_SEMAPHORE was successfully obtained or
		already owned by the calling TASK.

		OS_RESOURCE_GET_FAILED - If the BINARY_SEMAPHORE was not obtained
		and no delay was specified.

		OS_RESOURCE_GET_BLOCKED - If the BINARY_SEMAPHORE was not obtained
		but a delay was specified.

	Notes:
		- This method can potentially block since the BINARY_SEMAPHORE could be 
		  already owned by another TASK.
   
 		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- ReleaseBinarySemaphore()
*/
OS_RESULT GetBinarySemaphore(	BINARY_SEMAPHORE *BinarySemaphore

								#if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, INT32 TimeoutInTicks
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

								);

/*
	OS_RESULT ReleaseBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore)

	Description: This method attempts to release the specified BINARY_SEMAPHORE.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		CreateBinarySemaphore().  This BINARY_SEMAPHORE must have already been
		obtained by the calling TASK with a valid call to GetBinarySemaphore().

	Returns:
		OS_RESULT - This will be set to OS_RESOURCE_INVALID_OWNER if the current 
		TASK does not own the specified BINARY_SEMAPHORE.  Otherwise the 
		BINARY_SEMAPHORE will be successfully released and this will be set to 
		OS_SUCCESS.

	Notes:
		- The BINARY_SEMAPHORE must have already been successfully obtained
		  with a call to either GetBinarySemaphore(), or CreateBinarySemaphore()
		- This method is blocking potentially because a TASK with a higher priority
		  could be on the blocked list of the BINARY_SEMAPHORE.  If so, once the
		  owning TASK calls this method, it will block.

	See Also:
		- GetBinarySemaphore()
*/
OS_RESULT ReleaseBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore);

/*
	OS_RESULT DeleteBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore)

	Description: This method attempts to delete the specified BINARY_SEMAPHORE.

	Blocking: No

	User Callable: Yes

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		CreateBinarySemaphore().

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
		- A BINARY_SEMAPHORE must not be currently owned by a TASK in order
		  to be deleted.  Any TASK can delete a BINARY_SEMAPHORE.  That is to
		  say that the BINARY_SEMAPHORE does not need to be deleted by the
		  TASK which created it.  
		- The BINARY_SEMAPHORE must have been allocated in the heap to call
		  this method.
		- USING_DELETE_BINARY_SEMAPHORE_METHOD inside of RTOSConfig.h must
		  be defined as a 1 to use this method.

	See Also:
		- CreateBinarySemaphore()
*/
OS_RESULT DeleteBinarySemaphore(BINARY_SEMAPHORE *BinarySemaphore);

/*
	OS_RESULT BinarySemaphoreGetOwner(BINARY_SEMAPHORE *BinarySemaphore, TASK **Owner)

	Description: This method gets the current TASK owner of the BINARY_SEMAPHORE.

	Blocking: No

	User Callable: Yes

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		CreateBinarySemaphore().

	Returns:
		TASK * - A pointer to the TASK owner upon success, or (TASK*)NULL otherwise.

	Notes:
		- USING_GET_BINARY_SEMAPHORE_OWNER_METHOD in RTOSConfig.h must be set
		  to a 1 to use this method.

	See Also:
		- None
*/
TASK *GetBinarySemaphoreOwner(BINARY_SEMAPHORE *BinarySemaphore);

/*
	OS_RESULT GetBinarySemaphoreTaken(BINARY_SEMAPHORE *BinarySemaphore, BOOL *Taken)

	Description: This method gets if the BINARY_SEMAPHORE is currently owned
	by another TASK.

	Blocking: No

	User Callable: Yes

	Arguments:
		BINARY_SEMAPHORE *BinarySemaphore - A pointer to a valid BINARY_SEMAPHORE.  This
		BINARY_SEMAPHORE must have been created/initialized with a call to
		CreateBinarySemaphore().

	Returns:
        BOOL - TRUE if the SEMAPHORE is taken, FALSE otherwise.

	Notes: 
		- USING_GET_BINARY_SEMAPHORE_TAKEN_METHOD in RTOSConfig.h must be set
		  to a 1 to use this method.

	See Also:
		- None
*/
BOOL GetBinarySemaphoreTaken(BINARY_SEMAPHORE *BinarySemaphore);

#endif // end of #ifndef BINARY_SEMAPHORE_H


