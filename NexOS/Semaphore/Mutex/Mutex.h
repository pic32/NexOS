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
	MUTEX Description: 
		A MUTEX is similar to a BINARY_SEMAPHORE with one exception.  The owner of a MUTEX will temporarily
		have its priority increased if a TASK with a higher priority goes on the blocked list of the MUTEX.
		For example, If TASK 1 currently owns a MUTEX, and TASK 2 goes on the blocked list for the MUTEX, 
		TASK 1 will have it's priority elevated to TASK 2's should TASK 2's priority be higher than 
		TASK 1's priority.  

		Once a TASK releases a MUTEX its priority will return to whatever it was when it called GetMutex()
		should it have been modified in the interm.

		Problems or complications with ChangeTaskPriority() come into play too.  If TASK 1 in the above
		example has its priority changed with a call to ChangeTaskPriority() while it owns the MUTEX, the
		priority of TASK 1 will still return to whatever it was when it called GetMutex().  Thus the call
		to ChangeTaskPriority() only had a short term effect.

		Unlike a BINARY_SEMAPHORE or a COUNTING_SEMAPHORE, a MUTEX does not have any starvation protection
		mechanism built in.
*/

#ifndef MUTEX_H
	#define MUTEX_H

#include "../Binary Semaphore/BinarySemaphore.h"

#if (USING_MUTEXES != 1)
	#error "USING_MUTEXES must be defined as a 1 in RTOSConfig.h to use Mutexes"
#endif // end of #if USING_MUTEXES != 1)

/*  The structure of a MUTEX */
typedef struct
{
	// This is the BINARY_SEMAPHORE object which is used for semaphore like properties.
	BINARY_SEMAPHORE BinarySemaphore;

	// This is the original priority of the owning TASK.
	// This gets recorded once another TASK of higher priority
	// calls GetMutex() and then goes on the blocked list of the MUTEX.  
	// The recorded priority is of the current owning TASK.
	BYTE OriginalPriority;
}MUTEX;

/*
	MUTEX *CreateMutex(MUTEX *Mutex, BOOL GetMutex)

	Description: This method attempts to create and or initialize a basic MUTEX.

	Blocking: No

	User Callable: Yes

	Arguments:
		MUTEX *Mutex - A pointer to a valid MUTEX.  If NULL is passed in this method will 
		create a MUTEX object in the OS heap.

		BOOL GetMutex - If TRUE the calling TASK will own the MUTEX once created.  
		Otherwise the MUTEX is just created and not owned by any TASK.

	Returns:
		MUTEX * - A pointer to a valid MUTEX upon success, otherwise
		a (MUTEX*)NULL pointer is returned.

	Notes:
		- None

	See Also:
		- DeleteMutex()
*/
MUTEX *CreateMutex(MUTEX *Mutex, BOOL GetMutex);

/*
	OS_RESULT GetMutex(MUTEX *Mutex, INT32 TimeoutInTicks)

	Description: This method attempts to obtain a basic MUTEX.  If the MUTEX is owned 
	by another TASK, and a Timeout is specified, this method will place the current 
	TASK on the blocked	queue of the MUTEX.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		MUTEX *Mutex - A pointer to a valid MUTEX.  This
		MUTEX must have been created/initialized with a call to
		CreateMutex().

		INT32 TimeoutInTicks - This is a timeout value in ticks to wait
		for the MUTEX should it be owned by another TASK.  Below
		are valid values for TimeoutInTicks.

            TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
            Delayed Queue up to the specified number of ticks if
            the MUTEX is not obtained from this method.

            TimeoutInTicks = 0: The calling TASK will not be placed on the
            Delayed Queue and this method will return OS_RESOURCE_GET_FAILED.

            TimeoutInTicks <= -1: The calling TASK will not be placed on the
            Delay Queue and will wait forever.

	Returns:
		OS_SUCCESS - If the MUTEX was successfully obtained or
		already owned by the calling TASK.

		OS_RESOURCE_GET_FAILED - If the MUTEX was not obtained
		and no delay was specified.

		OS_RESOURCE_GET_BLOCKED - If the MUTEX was not obtained
		but a delay was specified.

	Notes:
		- This method can potentially block since the MUTEX could be already owned
		  by another TASK.
 
 		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- ReleaseMutex()
*/
OS_RESULT GetMutex(	MUTEX *Mutex

					#if (USING_TASK_DELAY_TICKS_METHOD == 1)
						, INT32 TimeoutInTicks
					#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

					);

/*
	OS_RESULT ReleaseMutex(MUTEX *Mutex)

	Description: This method attempts to release the specified MUTEX.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		MUTEX *Mutex - A pointer to a valid MUTEX.  This
		MUTEX must have been created/initialized with a call to
		CreateMutex().  This MUTEX must have already been
		obtained by the calling TASK with a valid call to  GetMutex().

	Returns:
		OS_RESULT - This will be set to OS_RESOURCE_INVALID_OWNER 
		if the current TASK does not own the specified MUTEX.  
		Otherwise the MUTEX will be successfully released and this 
		will be set to OS_SUCCESS.

	Notes:
		- The MUTEX must have already been successfully obtained
		  with a call to either GetMutex() or CreateMutex().
		- This method can be blocking if the owner of the MUTEX had its
		  priority increased by a higher priority TASK on the blocked list.
		  Once the owner of the MUTEX releases the MUTEX it will block
		  because its priority will go down to whatever it was when the
		  higher priority TASK called GetMutex().
		
	See Also:
		- GetMutex()
*/
OS_RESULT ReleaseMutex(MUTEX *Mutex);

/*
	OS_RESULT DeleteMutex(MUTEX *Mutex)

	Description: This method attempts to delete the specified MUTEX.

	Blocking: No

	User Callable: Yes

	Arguments:
		MUTEX *Mutex - A pointer to a valid MUTEX.  This
		MUTEX must have been created/initialized with a call to
		CreateMutex().

	Returns:
		OS_RESULT - This method will return one of three values based
		on the outcome of the requested delete operation.

		OS_SUCCESS - If the MUTEX was successfully deleted.

		OS_RESOURCE_NOT_IN_OS_HEAP - If the MUTEX was not
		located within the OS heap.  The MUTEX will not have
		been deleted.

		OS_RESOURCE_IN_USE - If the MUTEX is currently owned
		by a TASK.  The MUTEX will not have been deleted.

	Notes:
		- A MUTEX must not be currently owned by a TASK in order
		  to be deleted.  Any TASK can delete a MUTEX.  That is to
		  say that the MUTEX does not need to be deleted by the
		  TASK which created it.
		- The MUTEX must have been allocated in the heap to call
		  this method.
		- USING_DELETE_MUTEX_METHOD inside of RTOSConfig.h must
		  be defined as a 1 to use this method.

	See Also:
		- CreateMutex()
*/
OS_RESULT DeleteMutex(MUTEX *Mutex);

/*
	OS_RESULT GetMutexOwner(MUTEX *Mutex, TASK **Owner)

	Description: This method gets the current TASK owner of the MUTEX.

	Blocking: No

	User Callable: Yes

	Arguments:
		MUTEX *Mutex - A pointer to a valid MUTEX.  This
		MUTEX must have been created/initialized with a call to
		CreateMutex().

	Returns:
        TASK * - The TASK owner, or (TASK*)NULL if not successful.

	Notes:
		- USING_GET_MUTEX_OWNER_METHOD inside of RTOSConfig.h must be set
		  to a 1 to use this method.

	See Also:
		- None
*/
TASK *GetMutexOwner(MUTEX *Mutex);
TASK *OS_GetMutexOwner(MUTEX *Mutex);

/*
	BOOL GetMutexTaken(MUTEX *Mutex)

	Description: This method gets if the MUTEX is currently owned
	by another TASK.

	Blocking: No

	User Callable: Yes

	Arguments:
		MUTEX *Mutex - A pointer to a valid MUTEX.  This
		MUTEX must have been created/initialized with a call to
		CreateMutex().

	Returns:
        BOOL - This will be set to TRUE if the MUTEX is taken, FALSE otherwise.

	Notes:
		- USING_GET_MUTEX_TAKEN_METHOD in RTOSConfig.h must be set
		  to a 1 to use this method.

	See Also:
		- None
*/
BOOL GetMutexTaken(MUTEX *Mutex);

#endif // end of #ifndef MUTEX_H


