/*
    NexOS Kernel Version v1.00.00
    Copyright (c) 2020 brodie

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
	CALLBACK_TIMER Description:
	A CALLBACK_TIMER can be used to periodically execute a callback function.  The
	accuracy of these timers is +/- 1 OS tick.  A CALLBACK_TIMER is started and stopped
	only by a TASK.  All method calls done inside of the callback method of a CALLBACK_TIMER
	must be guaranteed to be non blocking, and cannot call any method which enters a critical
	section.

	The method callback is called at a configurable period which can be changed at any time.
*/

#ifndef CALLBACK_TIMERS_H
	#define CALLBACK_TIMERS_H

#include "GenericTypeDefs.h"
#include "../../../Generic Libraries/Double Linked List/DoubleLinkedList.h"
#include "../SoftwareTimer.h"

#if (USING_CALLBACK_TIMERS != 1)
	#error "USING_CALLBACK_TIMERS must be defined as a 1 in RTOSConfig.h to use Callback Timers"
#endif // end of #if USING_CALLBACK_TIMERS != 1)

/*
 * This method is what the OS calls for the callback of a CALLBACK_TIMER.
 * The UINT32 CurrentOSTickCount is the current OS tick count.
 */
typedef void(*CALLBACK_TIMER_CALLBACK)(UINT32 CurrentOSTickCount);

typedef struct
{
	SOFTWARE_TIMER Timer;
	CALLBACK_TIMER_CALLBACK CallbackTimerCallback; // this is the method called when Periodicity = Periodicity
	UINT32 PeriodicityInTicks;
	DOUBLE_LINKED_LIST_NODE Node;
}CALLBACK_TIMER;

/*
	void OS_UpdateCallbackTimers(UINT32 CurrentOSTickCount)

	Description:
		This method should be called every OS tick.  It updates the 
		CALLBACK_TIMERs that are active and calls their callback
		methods if required.

	Blocking: No

	User Callable: No

	Arguments:
		UINT32 CurrentOSTickCount - The current OS tick count.

	Returns:
		None

	Notes:
		- Only the OS calls this method.

	See Also:
		- None
*/
void OS_UpdateCallbackTimers(UINT32 CurrentOSTickCount);

/*
	BOOL OS_InitCallbackTimersLib(void)

	Description:
		This method initializes the CALLBACK_TIMER library.  It is
		called only by the OS.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns:
		BOOL - TRUE if the initialization was successful, FALSE otherwise.

	Notes:
		- Only the OS calls this method.

	See Also:
		- None
*/
BOOL OS_InitCallbackTimersLib(void);

/*
	CALLBACK_TIMER *CreateCallbackTimer(CALLBACK_TIMER *CallbackTimer, UINT32 PeriodicityInTicks, CALLBACK_TIMER_CALLBACK CallbackTimerCallback, BOOL Enable)

	Description:
		This method attempts to create a new CALLBACK_TIMER and return a pointer to it.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER.  If this is (CALLBACK_TIMER*)NULL
		this method will allocate a new CALLBACK_TIMER in the OS heap.
		
		UINT32 PeriodicityInTicks - The period in OS ticks to call the CallbackTimerCallback method.
		
		CALLBACK_TIMER_CALLBACK CallbackTimerCallback - The method to call when PeriodicityInTicks have elapsed.
		This method must not call any blocking function, or function which enters a critical section.

		BOOL Enable - If TRUE the CALLBACK_TIMER will be enabled and start counting after creation.  If FALSE 
		the CALLBACK_TIMER will not start counting after creation.  A call to CallbackTimerEnable() must be 
		made to start the CALLBACK_TIMER.

	Returns:
		CALLBACK_TIMER * - A pointer to a valid CALLBACK_TIMER if successful, (CALLBACK_TIMER*)NULL otherwise.

	Notes:
		- This method must be called before the CALLBACK_TIMER can be used in any other method.

	See Also:
		- CallbackTimerDelete()
*/
CALLBACK_TIMER *CreateCallbackTimer(CALLBACK_TIMER *CallbackTimer, UINT32 PeriodicityInTicks, CALLBACK_TIMER_CALLBACK CallbackTimerCallback, BOOL Enable);

/*
	OS_RESULT CallbackTimerEnable(CALLBACK_TIMER *CallbackTimer, BOOL Enable)

	Description:
		This method attempts to enable or disable a CALLBACK_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

		BOOL Enable - If TRUE the CALLBACK_TIMER will be enabled and start counting.  If FALSE the CALLBACK_TIMER
		will be disabled and cease to count.  The current tick count of the CALLBACK_TIMER is unchanged after this
		method is called.

	Returns:
		OS_RESULT - OS_SUCCESS if the operation was successful, or another value otherwise.

	Notes:
		- If the CALLBACK_TIMER is already in the requested enabled state, this method does nothing and returns
		OS_SUCCESS.

	See Also:
		- CreateCallbackTimer()
*/
OS_RESULT CallbackTimerEnable(CALLBACK_TIMER *CallbackTimer, BOOL Enable);

/*
	OS_RESULT CallbackTimerReset(CALLBACK_TIMER *CallbackTimer)

	Description:
		This method attempts to disable and reset the tick count of the specified CALLBACK_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation was successful, or another value otherwise.

	Notes:
		- USING_CALLBACK_TIMER_RESET_METHOD inside of RTOSConfig.h must be defined as a 1 to use this method.
        - USING_CALLBACK_TIMER_ENABLE_METHOD inside of RTOSConfig.h must be defined as a 1 to use this method.

	See Also:
		- CallbackTimerEnable()
*/
OS_RESULT CallbackTimerReset(CALLBACK_TIMER *CallbackTimer);

/*
	OS_RESULT CallbackTimerRestart(CALLBACK_TIMER *CallbackTimer)

	Description:
		This method attempts to clear the current tick count of the CALLBACK_TIMER to zero and then enables
		the CALLBACK_TIMER if disabled.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation was successful, or another value otherwise.

	Notes:
		- USING_CALLBACK_TIMER_RESTART_METHOD inside of RTOSConfig.h must be defined as a 1 to use this method.
        - USING_CALLBACK_TIMER_ENABLE_METHOD inside of RTOSConfig.h must be defined as a 1 to use this method.

	See Also:
		None
*/
OS_RESULT CallbackTimerRestart(CALLBACK_TIMER *CallbackTimer);

/*
	OS_RESULT CallbackTimerGetTicksRemaining(CALLBACK_TIMER *CallbackTimer, UINT32 *TicksRemaining)

	Description:
		This method attempts to get the remaining OS ticks needed to pass until the callback method of the
		CALLBACK_TIMER is executed.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

	Returns:
        UINT32 - The remaining OS ticks until the CALLBACK_TIMER calls the user callback method.

	Notes:
		- USING_CALLBACK_TIMER_GET_TICKS_REMAINING_METHOD inside of RTOSConfig.h must be defined as a 1 to use this method.

	See Also:
		CallbackTimerGetPeriodicityInTicks(), CallbackTimerSetPeriodicity()
*/
UINT32 CallbackTimerGetTicksRemaining(CALLBACK_TIMER *CallbackTimer);

/*
	OS_RESULT CallbackTimerGetPeriodicityInTicks(CALLBACK_TIMER *CallbackTimer, UINT32 *PeriodicityInTicks)

	Description:
		This method attempts to get the number of OS ticks needed to elapse each time for the callback 
		method of the CALLBACK_TIMER to be executed.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

	Returns:
        UINT32 - The CALLBACK_TIMER periodicity in OS ticks.

	Notes:
		- USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_METHOD inside of RTOSConfig.h must be defined as a 1 to use 
		  this method.

	See Also:
		CallbackTimerGetTicksRemaining(), CallbackTimerSetPeriodicity()
*/
UINT32 CallbackTimerGetPeriodicityInTicks(CALLBACK_TIMER *CallbackTimer);

/*
	OS_RESULT CallbackTimerSetPeriodicity(CALLBACK_TIMER *CallbackTimer, UINT32 PeriodicityInTicks)

	Description:
		This method attempts to set a new periodicity for the CALLBACK_TIMER callback method.  This method also
		clears the current tick count of the CALLBACK_TIMER after changing the periodicity.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

		UINT32 PeriodicityInTicks - The new CALLBACK_TIMER periodicity.  This value cannot be zero.

	Returns:
		OS_RESULT - OS_SUCCESS if the operation was successful, or another value otherwise.

	Notes:
		- USING_CALLBACK_TIMER_SET_PERIODICITY_METHOD inside of RTOSConfig.h must be defined as a 1 to use
		  this method.

	See Also:
		- CallbackTimerGetTicksRemaining(), CallbackTimerGetPeriodicityInTicks()
*/
OS_RESULT CallbackTimerSetPeriodicity(CALLBACK_TIMER *CallbackTimer, UINT32 PeriodicityInTicks);

/*
	OS_RESULT CallbackTimerDelete(CALLBACK_TIMER *CallbackTimer)

	Description:
		This method attempts to delete a CALLBACK_TIMER which was created with a call to CreateCallbackTimer().
		If successful all memory allocated for the CALLBACK_TIMER will be returned to the OS heap.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation was successful, or another value otherwise.

	Notes:
		- USING_CALLBACK_TIMER_DELETE_METHOD inside of RTOSConfig.h must be defined as a 1 to use
		  this method.

	See Also:
		- CreateCallbackTimer()
*/
OS_RESULT CallbackTimerDelete(CALLBACK_TIMER *CallbackTimer);

/*
	BOOL CallbackTimerIsRunning(CALLBACK_TIMER *CallbackTimer)

	Description:
		This method attempts to get the enabled status of the CALLBACK_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		CALLBACK_TIMER *CallbackTimer - A pointer to a valid CALLBACK_TIMER returned from CreateCallbackTimer().

	Returns:
		OS_RESULT - TRUE if the CALLBACK_TIMER is running, FALSE otherwise.

	Notes:
		- USING_CALLBACK_TIMER_IS_RUNNING_METHOD inside of RTOSConfig.h must be defined as a 1 to use
		  this method.

	See Also:
		- CallbackTimerEnable()
*/
BOOL CallbackTimerIsRunning(CALLBACK_TIMER *CallbackTimer);

#endif // end of #ifndef CALLBACK_TIMERS_H
