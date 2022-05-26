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

/*
	COUNTING_SEMAPHORE Description:
		A COUNTING_SEMAPHORE is identical to a BINARY_SEMAPHORE with one exception.  The COUNTING_SEMAPHORE can
		be aquired multiple times by the same TASK.  Each time it is acquired a counter is incremented.  To
		release the COUNTING_SEMAPHORE an equal number of release calls need to be made, and on the last
		release call the COUNTING SEMAPHORE is finally released by the owning TASK.
	
		Only one TASK at a time can own the semaphore.  If TASK 1 tries to get the COUNTING_SEMAPHORE when 
		TASK 2 already owns it, TASK 1 will block until TASK 2 releases the COUNTING_SEMAPHORE.  When a 
		COUNTING_SEMAPHORE is released it goes to the TASK on the COUNTING_SEMAPHORE's blocked list which 
		has the highest priority.

		This can cause starvation of a TASK getting the COUNTING_SEMAPHORE.  To prevent a TASK from getting
		starved of the COUNTING_SEMAPHORE the starvation protection mechanism should be enabled.  This is
		done by setting USING_BINARY_SEMAPHORE_STARVATION_PROTECTION in RTOSConfig.h to a 1.  Each time a
		COUNTING_SEMAPHORE is released, all the TASKs on the blocked list get their priorites increased by 1.
		Eventually a TASK that would have been starved will get the COUNTING_SEMAPHORE.  Once the TASK gets
		a COUNTING_SEMAPHORE while it was on the blocked list, it's priority will return to whatever it was
		when it first called GetCountingSemaphore().
*/

#ifndef COUNTING_SEMAPHORE_H
	#define COUNTING_SEMAPHORE_H

#include "../Binary Semaphore/BinarySemaphore.h"

typedef struct
{
	// this is the count used to keep track of how many times GetCountingSemaphore() was called.
	UINT32 Count;

	// this is the BINARY_SEMAPHORE object which is used for semaphore like properties.
	BINARY_SEMAPHORE BinarySemaphore;
}COUNTING_SEMAPHORE;

/*
	COUNTING_SEMAPHORE *CreateCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore, BOOL GetCountingSemaphore)

	Description: This method attempts to create and or initialize a basic COUNTING_SEMAPHORE.

	Blocking: No

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.  If
		NULL is passed in this method will create a COUNTING_SEMAPHORE object
		in the OS heap.

		BOOL GetCountingSemaphore - If TRUE the calling TASK will
		own the COUNTING_SEMAPHORE once created.  Otherwise the COUNTING_SEMAPHORE
		is just created and not owned by any TASK.

	Returns:
		COUNTING_SEMAPHORE * - A pointer to a valid COUNTING_SEMAPHORE upon success, otherwise
		a (COUNTING_SEMAPHORE*)NULL pointer is returned.

	Notes:
		- None

	See Also:
		- DeleteCountingSemaphore()
*/
COUNTING_SEMAPHORE *CreateCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore, BOOL GetCountingSemaphore);

/*
	OS_RESULT GetCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore, INT32 TimeoutInTicks)

	Description: This method attempts to obtain a basic COUNTING_SEMAPHORE.  If
	the COUNTING_SEMAPHORE is owned by another TASK, and a Timeout is specified,
	this method will place the current TASK on the Delay Queue.  If the calling TASK 
	already owns the COUNTING_SEMAPHORE, the COUNTING_SEMAPHORE count is increased by one.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.  
		This COUNTING_SEMAPHORE must have been created/initialized with a call to
		CreateCountingSemaphore().

		INT32 TimeoutInTicks - This is a timeout value in ticks to wait
		for the COUNTING_SEMAPHORE should it be owned by another TASK.  Below
		are valid values for TimeoutInTicks.

			TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
			Delayed Queue up to the specified number of ticks if
			the COUNTING_SEMAPHORE is not obtained from this method.

			TimeoutInTicks = 0: The calling TASK will not be placed on the
			Delay Queue and this method will return OS_RESOURCE_GET_FAILED.

			TimeoutInTicks <= -1: The calling TASK will not be placed on the
			Delay Queue and will wait forever.

	Returns:
		OS_SUCCESS - If the COUNTING_SEMAPHORE was successfully obtained or
		already owned by the calling TASK.

		OS_RESOURCE_GET_FAILED - If the COUNTING_SEMAPHORE was not obtained
		and no delay was specified.

		OS_RESOURCE_GET_BLOCKED - If the COUNTING_SEMAPHORE was not obtained
		but a delay was specified.

	Notes:
		- This method can potentially block since the COUNTING_SEMAPHORE could be
		  already owned by another TASK.
 
 		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- ReleaseCountingSemaphore()
*/
OS_RESULT GetCountingSemaphore(	COUNTING_SEMAPHORE *CountingSemaphore

								#if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, INT32 TimeoutInTicks
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

								);

/*
	OS_RESULT ReleaseCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore)

	Description: This method decreases the count of the COUNTING_SEMAPHORE by 1.  Once the
	count reaches zero, the COUNTING_SEMAPHORE will be released.

	Blocking: No

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.  This
		COUNTING_SEMAPHORE must have been created/initialized with a call to
		CreateCountingSemaphore().  This COUNTING_SEMAPHORE must have already been
		obtained by the calling TASK with a valid call to GetCountingSemaphore().

	Returns:
		OS_RESULT - This will be set to OS_RESOURCE_INVALID_OWNER if the current 
		TASK does not own the specified COUNTING_SEMAPHORE.  Otherwise the 
		COUNTING_SEMAPHORE count wwas decreased and potentially released 
		and this will return OS_SUCCESS.

	Notes:
		- The COUNTING_SEMAPHORE must have already been successfully obtained
		  with a call to either GetCountingSemaphore(), or CreateCountingSemaphore().
		- Only the owner of the COUNTING_SEMAPHORE can call this method.

	See Also:
		- GetCountingSemaphore()
*/
OS_RESULT ReleaseCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore);

/*
	OS_RESULT ReleaseUnconditionallyCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore)

	Description: This method sets the count of the COUNTING_SEMAPHORE to 0 and releases it.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.  This
		COUNTING_SEMAPHORE must have been created/initialized with a call to
		CreateCountingSemaphore().  This COUNTING_SEMAPHORE must have already been
		obtained by the calling TASK with a valid call to GetCountingSemaphore().

	Returns:
		OS_RESULT - This will be set to OS_RESOURCE_INVALID_OWNER if the current 
		TASK does not own the specified COUNTING_SEMAPHORE.  Otherwise the
		COUNTING_SEMAPHORE will be successfully released and this will return OS_SUCCESS.

	Notes:
		- The COUNTING_SEMAPHORE must have already been successfully obtained
		  with a call to either GetCountingSemaphore(), or CreateCountingSemaphore().
		- Only the owner of the COUNTING_SEMAPHORE can call this method.
		- This method is blocking potentially because a TASK with a higher priority
		  could be on the blocked list of the COUNTING_SEMAPHORE.  If so, once the
		  owning TASK calls this method, it will block.
		- USING_RELEASE_UNCONDITIONALLY_COUNTING_SEMAPHORE_METHOD inside of RTOSConfig.h
		  must be defined as a 1 to use this method.

	See Also:
		- GetCountingSemaphore()
*/
OS_RESULT ReleaseUnconditionallyCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore);

/*
	OS_RESULT DeleteCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore)

	Description: This method attempts to delete the specified COUNTING_SEMAPHORE.

	Blocking: No

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.  This
		COUNTING_SEMAPHORE must have been created/initialized with a call to
		CreateCountingSemaphore().

	Returns:
		OS_RESULT - This method will return one of three values based
		on the outcome of the requested delete operation.

		OS_SUCCESS - If the COUNTING_SEMAPHORE was successfully deleted.

		OS_RESOURCE_NOT_IN_OS_HEAP - If the COUNTING_SEMAPHORE was not
		located within the OS heap.  The COUNTING_SEMAPHORE will not have
		been deleted.

		OS_RESOURCE_IN_USE - If the COUNTING_SEMAPHORE is currently owned
		by a TASK.  The COUNTING_SEMAPHORE will not have been deleted.

	Notes:
		- A COUNTING_SEMAPHORE must not be currently owned by a TASK in order
		  to be deleted.  Any TASK can delete a COUNTING_SEMAPHORE.  That is to
		  say that the COUNTING_SEMAPHORE does not need to be deleted by the
		  TASK which created it.
		- The COUNTING_SEMAPHORE must have been allocated in the heap to call
		  this method.
		- USING_DELETE_COUNTING_SEMAPHORE_METHOD inside of RTOSConfig.h must
		  be defined as a 1 to use this method.

	See Also:
		- CreateCountingSemaphore()
*/
OS_RESULT DeleteCountingSemaphore(COUNTING_SEMAPHORE *CountingSemaphore);

/*
	TASK *GetCountingSemaphoreOwner(COUNTING_SEMAPHORE *CountingSemaphore)

	Description: This method gets the current TASK owner of the COUNTING_SEMAPHORE.

	Blocking: No

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.  This
		COUNTING_SEMAPHORE must have been created/initialized with a call to
		CreateCountingSemaphore().

	Returns:
        TASK * - The current owner of the COUNTING_SEMAPHORE, or (TASK*)NULL if unsuccessful.
	
	Notes:
		- USING_GET_COUNTING_SEMAPHORE_OWNER_METHOD in RTOSConfig.h must be set
		  to a 1 to use this method.

	See Also:
		- None
*/
TASK *GetCountingSemaphoreOwner(COUNTING_SEMAPHORE *CountingSemaphore);

/*
	BOOL GetCountingSemaphoreTaken(COUNTING_SEMAPHORE *CountingSemaphore)

	Description: This method gets if the COUNTING_SEMAPHORE is currently owned
	by another TASK.

	Blocking: No

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.  
		This COUNTING_SEMAPHORE must have been created/initialized with a call to
		CreateCountingSemaphore().

	Returns:
        BOOL - This will be set to TRUE if the COUNTING_SEMAPHORE is taken, FALSE otherwise.

	Notes:
		- USING_GET_COUNTING_SEMAPHORE_TAKEN_METHOD in RTOSConfig.h must be set
		  to a 1 to use this method.

	See Also:
		- None
*/
BOOL GetCountingSemaphoreTaken(COUNTING_SEMAPHORE *CountingSemaphore);

/*
	UINT32 GetCountingSemaphoreCount(COUNTING_SEMAPHORE *CountingSemaphore)

	Description: This method writes the current count of the COUNTING_SEMAPHORE to a pointer
	passed in.

	Blocking: No

	User Callable: Yes

	Arguments:
		COUNTING_SEMAPHORE *CountingSemaphore - A pointer to a valid COUNTING_SEMAPHORE.
		This COUNTING_SEMAPHORE must have been created/initialized with a call to
		CreateCountingSemaphore().

	Returns:
		UINT32 - The number of times the COUNTING_SEMAPHORE was acquired.

	Notes:
		- USING_GET_COUNTING_SEMAPHORE_COUNT_METHOD in RTOSConfig.h must be set
		  to a 1 to use this method.

	See Also:
		- None
*/
UINT32 GetCountingSemaphoreCount(COUNTING_SEMAPHORE *CountingSemaphore);

#endif // end of #ifndef COUNTING_SEMAPHORE_H