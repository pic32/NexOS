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

#ifndef OS_CALLBACKS_H
	#define OS_CALLBACKS_H

#include "GenericTypeDefs.h"
#include "TaskObject.h"
#include "Kernel.h"

/*
 * Special Notes:
 * 
 * - No callback should ever call a method that may block. 
 * - A callback is executed in a critical section, so be quick.
 * - A callback is executed only when the Kernel or Kernel TASK has control.
 * 
 */

// Kernel.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
/*
	BOOL TaskCheckInUserCallback(TASK *Task)

	Description: This method is called when a TASK exceeds it's check in time.
    The return value of this method determines if the TASK is restarted, deleted,
    or nothing happens.

	Blocking: No

	User Callable: No

	Arguments:
		TASK *Task - The TASK which exceeded its check in value.

	Returns:
        BOOL - If this method returns TRUE the TASK will be restarted if RestartTask was
        set to TRUE when the TASK was created with CreateTask().  Otherwise the TASK will
        be deleted.  If this method returns FALSE nothing happens to the offending TASK.

	Notes:
		- USING_TASK_CHECK_IN, USING_RESTART_TASK, USING_DELETE_TASK, and
          USING_TASK_CHECK_IN_USER_CALLBACK inside of RTOSConfig.h must be 
          defined as 1 to use this method.

	See Also:
		- SetupTaskCheckIn(), TaskCheckIn()
*/
BOOL TaskCheckInUserCallback(TASK *Task);

/*
	void ContextSwitchUserCallback(void)

	Description: This method is called each time the OS goes to potentially change
    the currently executing TASK.  This method is called each time the OS timer 
    overflows, the Core Interrupt is triggered, or an EVENT/IO_BUFFER area is executed.

	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns:
        None

	Notes:
        - USING_CONTEXT_SWITCH_USER_CALLBACK inside of RTOSConfig.h must be defined 
          as a 1 to use this callback.
		- Just because this method is called doesn't mean that the currently executing
          TASK is swapped for another.

	See Also:
		- None
*/
void ContextSwitchUserCallback(void);

/*
	void OSTickUpdateUserCallback(UINT32 CurrentTickCount)

	Description: This method is called each time the OS tick timer overflows.

	Blocking: No

	User Callable: No

	Arguments:
        UINT32 CurrentTickCount - The current number of OS timer ticks that have
        elapsed since the call to StartOSScheduler().

	Returns:
        None 

	Notes:
		- USING_OS_TICK_UPDATE_USER_CALLBACK inside of RTOSConfig.h must be 
          defined as a 1 to use this callback.

	See Also:
		- None
*/
void OSTickUpdateUserCallback(UINT32 CurrentTickCount);

/*
	void EnterDeviceSleepModeUserCallback(void)

	Description: This method is called right before the CPU enters sleep mode.

	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns:
        None
 
	Notes:
		- USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK inside of RTOSConfig.h must 
          be defined as a 1 to use this callback.
        - PortStopOSTickTimer(), PortStartOSTickTimer(), and PortEnterSleepMode() inside of 
          Port.c/Port.h must be implemented to use void EnterDeviceSleepModeUserCallback(void).

	See Also:
		- ExitDeviceSleepModeUserCallback()
*/
void EnterDeviceSleepModeUserCallback(void);

/*
	void ExitDeviceSleepModeUserCallback(void)

	Description: This method is called once the CPU exits sleep mode.

	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns:
        None 

	Notes:
		- USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK inside of RTOSConfig.h must 
          be defined as a 1 to use this callback.
        - PortStopOSTickTimer(), PortStartOSTickTimer(), and PortEnterSleepMode() inside of 
          Port.c/Port.h must be implemented to use void ExitDeviceSleepModeUserCallback(void).

	See Also:
		- EnterDeviceSleepModeUserCallback()
*/
void ExitDeviceSleepModeUserCallback(void);

/*
	BOOL GeneralExceptionHandlerUserCallback(TASK *CurrentTask, OS_EXCEPTION_CODE ExceptionCode, UINT32 ExceptionAddress, BOOL UserCodeCurrentlyExecuting)

	Description: This method is called when a CPU exception occurs.

	Blocking: No

	User Callable: No

	Arguments:
        TASK *CurrentTask - A pointer to the currently executing TASK when the exception occurred.

        OS_EXCEPTION_CODE ExceptionCode - The type of exception that occurred.
 
        UINT32 ExceptionAddress - The address the PC had when the exception occurred.
 
        BOOL UserCodeCurrentlyExecuting - TRUE if a user TASK was executing, FALSE otherwise.
 
	Returns:
        BOOL - Return TRUE to restart or delete the currently running TASK.  This is
        only valid if user code is executing.  Depending on the configuration of the
        the Kernel the return value for this method does the following:
 
        If USING_DELETE_TASK = 0 and USING_RESTART_TASK = 0 the return value of 
        GeneralExceptionHandlerUserCallback() means nothing.  The task will swap out
        and things are corrupted only to fail again.
 
        If USING_DELETE_TASK = 0 and USING_RESTART_TASK = 1 means that returning TRUE
        will try to restart the TASK.  If the TASK did not set RestartTask to TRUE
        in CreateTask() the TASK will not be restarted even if GeneralExceptionHandlerUserCallback()
        returns TRUE.  If FALSE is returned from GeneralExceptionHandlerUserCallback()
        then control is returned to the method which called GeneralExceptionHandler().
 
        If USING_DELETE_TASK = 1 and USING_RESTART_TASK = 0 means that returning TRUE
        will delete the current TASK.  The TASK with the next highest priority is then
        chosen to run.  If FALSE is returned from GeneralExceptionHandlerUserCallback()
        then control is returned to the method which called GeneralExceptionHandler().
 
        If USING_DELETE_TASK = 1 and USING_RESTART_TASK = 1 means that returning TRUE
        will attempt to restart the current TASK.  If the TASK did not set RestartTask to TRUE
        in CreateTask() the TASK will not be restarted even if GeneralExceptionHandlerUserCallback()
        returns TRUE.  If this is the case then the currently executing TASK will be deleted.
        The TASK with the next highest priority is then chosen to run.  If FALSE is returned by
        GeneralExceptionHandlerUserCallback() the currently executing TASK is deleted and
        the TASK with the next highest priority is then chosen to run.
 
	Notes:
		- USING_OS_GENERAL_EXCEPTION_HANDLER inside of RTOSConfig.h must be defined
          as a 1 to use this callback.
        - USING_CPU_EXCEPTION_RAISED_USER_CALLBACK inside of RTOSConfig.h must be defined
          as a 1 to use this callback.
        - USING_RESTART_TASK inside of RTOSConfig.h must be defined as a 1 for returning
          TRUE to have any meaning.
        - USING_DELETE_TASK inside of RTOSConfig.h must be defined as a 1 for returning
          TRUE to have any meaning.

	See Also:
		- RestartTask(), DeleteTask()
*/
BOOL GeneralExceptionHandlerUserCallback(TASK *CurrentTask, OS_EXCEPTION_CODE ExceptionCode, UINT32 ExceptionAddress, BOOL UserCodeCurrentlyExecuting);
//----------------------------------------------------------------------------------------------------------------------------------------------------------


// Memory.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
/*
	void MemoryWarningUserCallback(UINT32 HeapUsageInBytes)

	Description: This method is called once the heap usage crosses a user defined
    threshold.  For example, if the user had OS_HEAP_SIZE_IN_BYTES defined as 50,000
    and a MEMORY_WARNING_LEVEL_IN_BYTES 45,000, if the heap usage went above 45,000
    this MemoryWarningUserCallback() would be called. 

	Blocking: No

	User Callable: No

	Arguments:
        UINT32 HeapUsageInBytes - The current number of bytes used in the heap.

	Returns:
        None

	Notes:
		- USING_MEMORY_WARNING_USER_CALLBACK inside of RTOSConfig.h must be defined
          as a 1 to use this callback.
        - MEMORY_WARNING_LEVEL_IN_BYTES inside of RTOSConfig.h must have a valid value.

	See Also:
		- MemoryWarningClearUserCallback()
*/
void MemoryWarningUserCallback(UINT32 HeapUsageInBytes);

/*
	void MemoryWarningClearUserCallback(void)

	Description: This method is called when the heap usage previously crossed 
    MEMORY_WARNING_LEVEL_IN_BYTES when increasing, and then crossed 
    MEMORY_WARNING_LEVEL_IN_BYTES while decreasing.
 
	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns:
        None 

	Notes:
		- USING_MEMORY_WARNING_CLEAR_USER_CALLBACK inside of RTOSConfig.h must be defined
          as a 1 to use this callback.
        - MEMORY_WARNING_LEVEL_IN_BYTES inside of RTOSConfig.h must have a valid value.

	See Also:
		- MemoryWarningUserCallback()
*/
void MemoryWarningClearUserCallback(void);
//----------------------------------------------------------------------------------------------------------------------------------------------------------


// KernelTasks.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
/*
	void IdleTaskUserCallback(void *Args)

	Description: This method is called forever by the Idle TASK.

	Blocking: No

	User Callable: No

	Arguments:
        void *Args - The arguments passed with the macro IDLE_TASK_ARGS.

	Returns:
        None 

	Notes:
		- USING_IDLE_TASK_USER_CALLBACK inside of RTOSConfig.h must be defined
          as a 1 to use this callback.
		- This method does not have to return, but it still cannot block.

	See Also:
		- None
*/
void IdleTaskUserCallback(void *Args);

/*
	void MaintenanceTaskUserCallback(void *Args)

	Description: This method is called each time the Maintenance TASK executes.
    The Maintenance TASK only executes if USING_RESTART_TASK = 1 or if 
    USING_DELETE_TASK = 1.

	Blocking: No

	User Callable: No

	Arguments:
        void *Args - The arguments passed with the macro MAINTENANCE_TASK_ARGS.

	Returns:
        None 

	Notes:
		- USING_MAINTENANCE_TASK_USER_CALLBACK inside of RTOSConfig.h must be defined
          as a 1 to use this callback.

	See Also:
		- None
*/
void MaintenanceTaskUserCallback(void *Args);
//----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // end of #ifndef OS_CALLBACKS_H
