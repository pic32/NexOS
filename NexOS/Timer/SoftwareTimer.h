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
	SOFTWARE_TIMERS Description:
		A SOFTWARE_TIMER can be used to keep track of time in OS ticks resolution.  The 
		accuracy of these timers is +/- 1 OS tick.  A SOFTWARE_TIMER is started and stopped
		by a TASK and the time elapsed can be read out.

		The SOFTWARE_TIMER is also the base object for the EVENT_TIMER and CALLBACK_TIMER.
*/

#ifndef SOFTWARE_TIMERS_H
	#define SOFTWARE_TIMERS_H

#include "GenericTypeDefs.h"
#include "RTOSConfig.h"
#include "../../Generic Libraries/Double Linked List/DoubleLinkedList.h"
#include "../Kernel/Kernel.h"

#if (USING_SOFTWARE_TIMERS != 1)
	#error "USING_SOFTWARE_TIMERS must be defined as a 1 in RTOSConfig.h to use Software Timers"
#endif // end of #if USING_SOFTWARE_TIMERS != 1)

typedef struct
{
	UINT32 Ticks;
	DOUBLE_LINKED_LIST_NODE TimerListNode;
	BOOL Active;
}SOFTWARE_TIMER;

// Use this define to know if a valid number of ticks are returned with timer get methods
#define INVALID_TIMER_TICKS_VALUE								0xFFFFFFFF

/*
	BOOL OS_InitSoftwareTimerLib(void)

	Description: 
		This method attempts to initialize the Software Timer library.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns:
		BOOL - TRUE if the method succeeded, FALSE otherwise.

	Notes:
		- Only the OS calls this method.

	See Also:
		- None
*/
BOOL OS_InitSoftwareTimerLib(void);

/*
	void OS_UpdateSoftwareTimers(void)

	Description:
		This method updates all active timers.  This method is called at each
		call of UpdateOSTick().

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns:
		None

	Notes:
		- Only the OS calls this method.

	See Also:
		- UpdateOSTick()
*/
void OS_UpdateSoftwareTimers(void);

/*
	void OS_SoftwareTimerClear(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method clears the tick counter of the SOFTWARE_TIMER.

	Blocking: No

	User Callable: No

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER.

	Returns:
		None

	Notes:
		- Only the OS calls this method.
		- This method assumes the SOFTWARE_TIMER is valid.

	See Also:
		- None
*/
void OS_SoftwareTimerClear(SOFTWARE_TIMER *SoftwareTimer);

/*
	BOOL OS_SoftwareTimerIsRunning(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method returns the enabled state of the SOFTWARE_TIMER.

	Blocking: No

	User Callable: No

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER.

	Returns:
		None

	Notes:
		- Only the OS calls this method.
		- This method assumes the SOFTWARE_TIMER is valid.

	See Also:
		- None
*/
BOOL OS_SoftwareTimerIsRunning(SOFTWARE_TIMER *SoftwareTimer);

/*
	void OS_SoftwareTimerEnable(SOFTWARE_TIMER *SoftwareTimer, BOOL Enable)

	Description:
		This method sets the enabled state of the SOFTWARE_TIMER.

	Blocking: No

	User Callable: No

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER.
		BOOL Enable - If TRUE the SOFTWARE_TIMER will be enabled, FALSE the
		SOFTWARE_TIMER will be disabled.

	Returns:
		None

	Notes:
		- Only the OS calls this method.
		- This method assumes the SOFTWARE_TIMER is valid.

	See Also:
		- None
*/
void OS_SoftwareTimerEnable(SOFTWARE_TIMER *SoftwareTimer, BOOL Enable);

/*
	UINT32 OS_SoftwareTimerGetTicks(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method gets the tick counter of the SOFTWARE_TIMER.

	Blocking: No

	User Callable: No

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER.

	Returns:
		UINT32 - The current ticks in the SOFTWARE_TIMER.

	Notes:
		- Only the OS calls this method.
		- This method assumes the SOFTWARE_TIMER is valid.

	See Also:
		- None
*/
UINT32 OS_SoftwareTimerGetTicks(SOFTWARE_TIMER *SoftwareTimer);

/*
	SOFTWARE_TIMER *CreateSoftwareTimer(SOFTWARE_TIMER *Timer)

	Description:
		This method creates and/or initializes a SOFTWARE_TIMER.  Once done a pointer to
		the SOFTWARE_TIMER can be used as an argument for other methods in this library.
		This method must be called before any other methods use the returned SOFTWARE_TIMER *.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER or (SOFTWARE_TIMER*)NULL.
		If (SOFTWARE_TIMER*)NULL is passed in the method will attempt to allocate a 
		SOFTWARE_TIMER in the OS heap.

		BOOL Enable - If TRUE the SOFTWARE_TIMER will be enabled when created.  If FALSE the
		SOFTWARE_TIMER will be disabled when created.

	Returns:
		SOFTWARE_TIMER * - A pointer to a valid SOFTWARE_TIMER upon success, or
		(SOFTWARE_TIMER*)NULL upon failure.

	Notes:
		- None

	See Also:
		- SoftwareTimerDelete()
*/
SOFTWARE_TIMER *CreateSoftwareTimer(SOFTWARE_TIMER *Timer, BOOL Enable);

/*
	BOOL SoftwareTimerEnable(SOFTWARE_TIMER *Timer, BOOL Enable)

	Description:
		This method enables or disables the incrementing of the SOFTWARE_TIMER.  A timer 
		which is enabled will have it tick counter incremented with the OS tick rate.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

		BOOL Enable - If TRUE the specified SOFTWARE_TIMER will be enabled and the timer
		will increment with the OS tick rate.  If FALSE the specified SOFTWARE_TIMER will
		stop being incremented with the OS tick rate.

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
		- USING_SOFTWARE_TIMER_ENABLE_METHOD inside of RTOSConfig.h must be defined as a 1
		  to use this method.
		- This method will not alter the SOFTWARE_TIMER tick counter when called.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerGetTicks().
*/
OS_RESULT SoftwareTimerEnable(SOFTWARE_TIMER *Timer, BOOL Enable);

/*
	BOOL SoftwareTimerReset(SOFTWARE_TIMER *Timer)

	Description:
		This method disables the specified SOFTWARE_TIMER and sets the internal tick count 
		to zero.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
		- USING_SOFTWARE_TIMER_RESET_METHOD inside of RTOSConfig.h must be defined as a 1
		  to use this method.
        - USING_SOFTWARE_TIMER_RESET_FROM_ISR_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerEnable(), SoftwareTimerResetFromISR()
*/
OS_RESULT SoftwareTimerReset(SOFTWARE_TIMER *Timer);

/*
	BOOL SoftwareTimerRestart(SOFTWARE_TIMER *Timer)

	Description:
		This method clears the internal tick count to zero and enables the SOFTWARE_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
		- USING_SOFTWARE_TIMER_RESTART_METHOD inside of RTOSConfig.h must be defined as a 1
		  to use this method.
        - USING_SOFTWARE_TIMER_RESTART_FROM_ISR_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerRestartFromISR()
*/
OS_RESULT SoftwareTimerRestart(SOFTWARE_TIMER *Timer);

/*
	BOOL SoftwareTimerClear(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method clears the internal tick count to zero of the SOFTWARE_TIMER.  It does not
		matter if the SOFTWARE_TIMER is enabled or not.  The enabled state also stays the same.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
		- USING_SOFTWARE_TIMER_CLEAR_METHOD inside of RTOSConfig.h must be defined as a 1
		  to use this method.
   		- USING_SOFTWARE_TIMER_CLEAR_FROM_ISR_METHOD inside of RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerEnable(), SoftwareTimerClearFromISR()
*/
OS_RESULT SoftwareTimerClear(SOFTWARE_TIMER *SoftwareTimer);

/*
	UINT32 SoftwareTimerGetTicks(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method returns the current tick count of the specified SOFTWARE_TIMER.  Each
		count in this value represents a single OS tick.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		UINT32 - The current tick count of the SOFTWARE_TIMER upon success.  Otherwise the
		value INVALID_TIMER_TICKS_VALUE is returned.

	Notes:
		- USING_SOFTWARE_TIMER_GET_TICKS_METHOD inside of RTOSConfig.h must be defined as a 1
		  to use this method.
   		- USING_SOFTWARE_TIMER_GET_TICKS_FROM_ISR_METHOD inside of RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerGetMilliseconds(), SoftwareTimerGetTicksFromISR()
*/
UINT32 SoftwareTimerGetTicks(SOFTWARE_TIMER *SoftwareTimer);

/*
	UINT32 SoftwareTimerGetMilliseconds(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method gets the current tick count of the SOFTWARE_TIMER and converts
		that to milliseconds.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
        UINT32 - The number of milliseconds the SOFTWARE_TIMER has been running.

	Notes:
		- USING_SOFTWARE_TIMER_GET_MILLISECONDS_METHOD inside of RTOSConfig.h must be defined 
		  as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerGetTicks().
*/
UINT32 SoftwareTimerGetMilliseconds(SOFTWARE_TIMER *SoftwareTimer);

/*
	BOOL SoftwareTimerIsRunning(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method gets the enabled state of the SOFTWARE_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		BOOL - TRUE if the SOFTWARE_TIMER is running, FALSE otherwise.
		
	Notes:
		- USING_SOFTWARE_TIMER_IS_RUNNING_METHOD inside of RTOSConfig.h must be defined
		  as a 1 to use this method.
        - USING_SOFTWARE_TIMER_IS_RUNNING_FROM_ISR_METHOD inside of RTOSConfig.h must 
          be defined as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerIsRunningFromISR()
*/
BOOL SoftwareTimerIsRunning(SOFTWARE_TIMER *SoftwareTimer);

/*
	BOOL SoftwareTimerDelete(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method deletes the specified SOFTWARE_TIMER.  The space used by the SOFTWARE_TIMER
		in the OS heap is released back to the heap.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().  A (SOFTWARE_TIMER*)NULL value must be passed
		to CreateSoftwareTimer() to be able to delete it with a call to SoftwareTimerDelete().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
		- USING_SOFTWARE_TIMER_DELETE_METHOD inside of RTOSConfig.h must be defined
		  as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer()
*/
OS_RESULT SoftwareTimerDelete(SOFTWARE_TIMER *SoftwareTimer);

/*
	BOOL SoftwareTimerEnableFromISR(SOFTWARE_TIMER *Timer, BOOL Enable)

	Description:
		This method enables or disables the incrementing of the SOFTWARE_TIMER.  A timer 
		which is enabled will have it tick counter incremented with the OS tick rate.
        This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

		BOOL Enable - If TRUE the specified SOFTWARE_TIMER will be enabled and the timer
		will increment with the OS tick rate.  If FALSE the specified SOFTWARE_TIMER will
		stop being incremented with the OS tick rate.

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
		- USING_SOFTWARE_TIMER_ENABLE_FROM_ISR_METHOD inside of RTOSConfig.h must be defined 
          as a 1 to use this method.
		- This method will not alter the SOFTWARE_TIMER tick counter when called.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerGetTicksFromISR().
*/
OS_RESULT SoftwareTimerEnableFromISR(SOFTWARE_TIMER *SoftwareTimer, BOOL Enable);

/*
	BOOL SoftwareTimerResetFromISR(SOFTWARE_TIMER *Timer)

	Description:
		This method disables the specified SOFTWARE_TIMER and sets the internal tick count 
		to zero.  This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
        - USING_SOFTWARE_TIMER_RESET_FROM_ISR_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerEnable(), SoftwareTimerReset()
*/
OS_RESULT SoftwareTimerResetFromISR(SOFTWARE_TIMER *Timer);

/*
	BOOL SoftwareTimerRestartFromISR(SOFTWARE_TIMER *Timer)

	Description:
		This method clears the internal tick count to zero and enables the SOFTWARE_TIMER.
        This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
        - USING_SOFTWARE_TIMER_RESTART_FROM_ISR_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerRestart()
*/
OS_RESULT SoftwareTimerRestartFromISR(SOFTWARE_TIMER *Timer);

/*
	BOOL SoftwareTimerClearFromISR(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method clears the internal tick count to zero of the SOFTWARE_TIMER.  It does not
		matter if the SOFTWARE_TIMER is enabled or not.  The enabled state also stays the same.
        This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		OS_RESULT - OS_SUCCESS if the operation passed, otherwise the operation failed.

	Notes:
   		- USING_SOFTWARE_TIMER_CLEAR_FROM_ISR_METHOD inside of RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerEnableFromISR(), SoftwareTimerClear()
*/
OS_RESULT SoftwareTimerClearFromISR(SOFTWARE_TIMER *SoftwareTimer);

/*
	UINT32 SoftwareTimerGetTicksFromISR(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method returns the current tick count of the specified SOFTWARE_TIMER.  Each
		count in this value represents a single OS tick.  This method can be called from
        an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		UINT32 - The current tick count of the SOFTWARE_TIMER upon success.  Otherwise the
		value INVALID_TIMER_TICKS_VALUE is returned.

	Notes:
   		- USING_SOFTWARE_TIMER_GET_TICKS_FROM_ISR_METHOD inside of RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerGetMilliseconds(), SoftwareTimerGetTicks()
*/
UINT32 SoftwareTimerGetTicksFromISR(SOFTWARE_TIMER *SoftwareTimer);

/*
	UINT32 SoftwareTimerGetMillisecondsFromISR(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method gets the current tick count of the SOFTWARE_TIMER and converts
		that to milliseconds.  This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
        UINT32 - The number of milliseconds the SOFTWARE_TIMER has been running.

	Notes:
		- USING_SOFTWARE_TIMER_GET_MILLISECONDS_FROM_ISR_METHOD inside of RTOSConfig.h 
          must be defined as a 1 to use this method.
        - USING_SOFTWARE_TIMER_GET_TICKS_FROM_ISR_METHOD inside of RTOSConfig.h 
          must be defined as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerGetTicksFromISR().
*/
UINT32 SoftwareTimerGetMillisecondsFromISR(SOFTWARE_TIMER *SoftwareTimer);

/*
	BOOL SoftwareTimerIsRunningFromISR(SOFTWARE_TIMER *SoftwareTimer)

	Description:
		This method gets the enabled state of the SOFTWARE_TIMER.  This
        method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		SOFTWARE_TIMER *Timer - A pointer to a valid SOFTWARE_TIMER that was initialized
		by a call to CreateSoftwareTimer().

	Returns:
		BOOL - TRUE if the SOFTWARE_TIMER is running, FALSE otherwise.
		
	Notes:
        - USING_SOFTWARE_TIMER_IS_RUNNING_FROM_ISR_METHOD inside of RTOSConfig.h must 
          be defined as a 1 to use this method.

	See Also:
		- CreateSoftwareTimer(), SoftwareTimerIsRunning()
*/
BOOL SoftwareTimerIsRunningFromISR(SOFTWARE_TIMER *SoftwareTimer);

#endif // end of SOFTWARE_TIMERS_H
