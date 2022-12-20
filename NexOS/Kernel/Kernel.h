/*
    NexOS Kernel Version v1.02.00
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
	Description:  
		This file is the main file for the NexOS.  This file houses all the method prototypes
        for interacting with the Kernel.  It also has all the methods for modifying the contents
        of the CPU scheduler.
 
    RTOS Design Philosophy
		1. Run Time Safety
		2. Code Maintainability and Modularity
		3. Execution Speed
		4. RAM Usage
		5. User Features
		6. Program Space
*/

#ifndef KERNEL_H
	#define KERNEL_H

#include "GenericTypes.h"
#include "RTOSConfig.h"
#include "Port.h"
#include "TaskObject.h"
#include "OS_Exception.h"

/*
 * TODO:
 *  - The sleep function will not re-enable the OS Tick Timer if it was off and
 *    an interrupt wakes the CPU up which causes a TASK other than the IDLE TASK
 *    to run.  The Timer would not run again until the IDLE TASK runs again.
 * 
 */

#define DO_NOT_WAIT                                                     0
#define WAIT_FOREVER                                                    -1
#define TASK_TIMEOUT_DONE_VALUE											-2

#if HIGHEST_USER_TASK_PRIORITY > 254
	#error "HIGHEST_USER_TASK_PRIORITY cannot exceed 254!"
#endif // end of #if HIGHEST_USER_TASK_PRIORITY > 254

// This represents the priority of the OS tasks
#define OS_HIGHEST_TASK_PRIORITY										(HIGHEST_USER_TASK_PRIORITY + 1)

// This is how many indexes the gCPUScheduler[] has.
#define CPU_SCHEDULER_QUEUE_SIZE                                        (OS_HIGHEST_TASK_PRIORITY + 1)

/*
 * These define the current kernel version.  If KERNEL_TEST_VERSION_NUMBER is
 * anything other than 0x00, the OS is a test release and should not be in
 * a field deployable project.
 */
#define KERNEL_MAJOR_VERSION_NUMBER										0x01
#define KERNEL_MINOR_VERSION_NUMBER										0x02
#define KERNEL_TEST_VERSION_NUMBER                                      0x00

#define INVALID_TASK_PRIORITY											255

typedef enum
{
	OS_SUCCESS = 0,
    OS_FALSE,
	OS_RESOURCE_GET_BLOCKED,
	OS_RESOURCE_GET_FAILED,
	OS_RESOURCE_GET_TIMEOUT,
	OS_RESOURCE_INVALID_ADDRESS,
	OS_RESOURCE_IN_USE,
	OS_RESOURCE_TIMEOUT,
	OS_RESOURCE_INVALID_OWNER,
	OS_RESOURCE_NOT_IN_OS_HEAP,
	OS_INVALID_ARGUMENT_ADDRESS,
	OS_INVALID_ARGUMENT,
	OS_INVALID_OBJECT_USED,
	OS_TASK_TIMEOUT,
	OS_ALLOCATE_MEMORY_FAILED,
	OS_FREE_MEMORY_FAILED,
	OS_INVALID_TASK_STATE,
	OS_RESOURCE_HAS_NO_DATA,
	OS_RESOURCE_INSUFFICIENT_SPACE,
	OS_RESOURCE_INSUFFICIENT_DATA,
	OS_INVALID_EVENT,
	OS_RESOURCE_FULL,
    OS_IO_BUFFER_INVALID_STATE, // the requested operation cannot be completed due to the IO Buffer being in the wrong state
    OS_INVALID_OBJECT_STATE,

	// These are all returned during InitOS()
	OS_HEAP_INITIALIZATION_FAILED,
	OS_CREATE_IDLE_TASK_FAILED,
	OS_CREATE_MAINTENANCE_TASK_FAILED,
    OS_CREATE_IO_BUFFER_TASK_FAILED,
	OS_INITIALIZE_TIMER_LIB_FAILED,
	OS_INITIALIZE_CALLBACK_TIMER_LIB_FAILED,
	OS_INITIALIZE_EVENTS_LIB_FAILED,
	OS_INITIALIZE_EVENT_TIMERS_LIB_FAILED,
	
	NUMBER_OF_OS_RESULTS
}OS_RESULT;

/*
	OS_RESULT InitOS(void)

	Description: This method initializes the OS and gets everything ready for running.
	It must be called before any other method to the OS is called.  If this method does
	not return OS_SUCCESS there is probably something wrong with the configuration of
	the OS and is unrecoverable.

	Blocking: No

	User Callable: Yes, only one time at start-up.

	Arguments:
		None

	Returns: 
		OS_RESULT - OS_SUCCESS if the OS was successfully initialized, a failure value
		otherwise.

	Notes:
		- This needs to be called before any other OS related method.  It should
          also only be called one time.

	See Also:
		- StartOSScheduler()
*/
OS_RESULT InitOS(void);

/*
	OS_RESULT InitOS(void)

	Description: This method launches the CPU scheduler and will begin to execute
	the TASK with the highest priority.  This method never returns under any 
	normal circumstance.

	Blocking: No

	User Callable: Yes, only one time at start-up.

	Arguments:
		None

	Returns:
		None

	Notes:
		- InitOS() must be called prior to this method.

	See Also:
		- InitOS()
*/
void StartOSScheduler(void);

/*
	UINT32 GetOSTickCount(void)

	Description: This method returns the current OS tick count.  The OS tick count
	will not start incrementing until after StartOSScheduler() is called.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns:
		UINT32 - The current OS tick count.  This value increments by 1 each time
		the OS timer interrupt triggers.

	Notes:
		- The OS tick value can roll over from 0xFFFFFFFF to 0x00000000.

	See Also:
		- StartOSScheduler(), GetOSTickCountFromISR()
*/
UINT32 GetOSTickCount(void);

/*
	UINT32 GetOSTickCountFromISR(void)

	Description: This method returns the current OS tick count.  The OS tick count
	will not start incrementing until after StartOSScheduler() is called.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns:
		UINT32 - The current OS tick count.  This value increments by 1 each time
		the OS timer interrupt triggers.

	Notes:
        - USING_GET_OS_TICK_COUNT_FROM_ISR_METHOD inside of RTOSConfig.h must be 
          defined as 1 to use this method.
		- The OS tick value can roll over from 0xFFFFFFFF to 0x00000000.

	See Also:
		- StartOSScheduler(), GetOSTickCount()
*/
UINT32 GetOSTickCountFromISR(void);

/*
	UINT32 AnaylzeSystemStackUsage(void)

	Description: This method returns how many words have been unused by the system
    stack.  This denotes the high water mark inside the system stack.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns:
		UINT32 - The number of words unused in the system stack.

	Notes:
        - USING_ANALYZE_SYSTEM_STACK_METHOD inside of RTOSConfig.h must be 
          defined as 1 to use this method.
		- PortAnaylzeTaskStackUsage() inside of Port.c must also be implemented for
          use of this function.

	See Also:
		- PortAnaylzeTaskStackUsage()
*/
UINT32 AnaylzeSystemStackUsage(void);

/*
	void TaskRuntimeExecutionListToString(BYTE *ToStringBuffer, BOOL Percentage)

	Description: This method will take the task execution times and to string
    them into a buffer.
	
	Blocking: No

	User Callable: Yes

	Arguments:
        BYTE *ToStringBuffer - The buffer to put the string in.
  
        BOOL Percentage - If TRUE the runtime in percentage will be printed, 
        otherwise the runtime in seconds will be printed.

	Returns:
        None

	Notes:
        - USING_TASK_RUNTIME_EXECUTION_TO_STRING_METHOD inside of RTOSConfig.h 
          must be defined as 1 to use this method.
        - USING_TASK_RUNTIME_EXECUTION_COUNTER inside of RTOSConfig.h must be 
          defined as 1 to use this method.
		- PortGetExecutionTimeInSeconds() inside of Port.c must also be implemented for
          use of this function.

	See Also:
		- PortGetExecutionTimeInSeconds()
*/
void TaskRuntimeExecutionListToString(BYTE *ToStringBuffer, BOOL Percentage);

/*
	void TaskRuntimeHistoryListToString(BYTE *ToStringBuffer)

	Description: This method will take the runtime history of TASKs and 
    to string the data to a buffer.  The most recently executed TASK will
    be the first one to stringed in the buffer.  The buffer will contain up
    to TASK_RUNTIME_HISTORY_SIZE_IN_TASKS number of TASKs data.
	
	Blocking: No

	User Callable: Yes

	Arguments:
        BYTE *ToStringBuffer - The buffer to put the string in.

        BOOL PrintRuntime - If set to TRUE the TASK runtime will also be printed.
 
	Returns:
        None 

	Notes:
        - USING_TASK_RUNTIME_EXECUTION_COUNTER inside of RTOSConfig.h must be 
          defined as 1 to use this method.

	See Also:
		- TASK_RUNTIME_HISTORY_SIZE_IN_TASKS inside of RTOSConfig.h
*/
void TaskRuntimeHistoryListToString(BYTE *ToStringBuffer, BOOL PrintRuntime);

/*
	void DeviceEnterSleepMode(void)

	Description: This method makes the CPU enter sleep mode.  It is user implemented.
	
	Blocking: Yes

	User Callable: Yes

	Arguments:
		None

	Returns:
        None

	Notes:
        - USING_ENTER_DEVICE_SLEEP_MODE_METHOD inside of RTOSConfig.h must be defined as 1 to use this method.
        - PortStopOSTickTimer(), PortStartOSTickTimer(), and PortEnterSleepMode() inside of Port.c/Port.h
          must be implemented for DeviceEnterSleepMode() to work.

	See Also:
		- USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK, USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK.
*/
void DeviceEnterSleepMode(void);

/*
	UINT32 TicksToMilliseconds(UINT32 Ticks)

	Description: This method converts the number of OS ticks to milliseconds.  If there
	is a fraction of a millisecond for the conversion it is omitted.  That is to say
	if the number of OS ticks produced 4.8 milliseconds, the returned number is 4.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		UINT32 Ticks - The number of ticks to convert to milliseconds.

	Returns:
		UINT32 - The number of milliseconds the ticks represent.

	Notes:
		- USING_TICKS_TO_MILLISECONDS_METHOD in RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		- MillisecondsToTicks()
*/
UINT32 TicksToMilliseconds(UINT32 Ticks);

/*
	UINT32 MillisecondsToTicks(UINT32 Milliseconds)

	Description: This method converts the number of milliseconds to OS ticks.  If there
	is a fraction of an OS ticks for the conversion it is omitted.  That is to say
	if the number of milliseconds produced 4.8 ticks, the returned number is 4.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		UINT32 Milliseconds - The number of milliseconds to convert to ticks.

	Returns:
		UINT32 - The number of ticks the milliseconds represent.

	Notes:
		- USING_MILLISECONDS_TO_TICKS_METHOD in RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		TicksToMilliseconds()
*/
UINT32 MillisecondsToTicks(UINT32 Milliseconds);

/*
	UINT32 TicksToMicroseconds(UINT32 Ticks)

	Description: This method converts the number of ticks to microseconds.  If there
	is a fraction of a microsecond for the conversion it is omitted.  That is to say
	if the number of ticks produced 4.8 microseconds, the returned number is 4.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		UINT32 Ticks - The number of ticks to convert to microseconds.

	Returns:
		UINT32 - The number of microseconds the ticks represent.

	Notes:
		- USING_MICROSECONDS_TO_TICKS_METHOD in RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		- MicrosecondsToTicks()
*/
UINT32 TicksToMicroseconds(UINT32 Ticks);

/*
	UINT32 MicrosecondsToTicks(UINT32 Microseconds)

	Description: This method converts the number of microseconds to ticks.  If there
	is a fraction of a ticks for the conversion it is omitted.  That is to say
	if the number of microseconds produced 4.8 ticks, the returned number is 4.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		UINT32 Microseconds - The number of microseconds to convert to ticks.

	Returns:
		UINT32 - The number of ticks the microseconds represent.

	Notes:
		- USING_MICROSECONDS_TO_TICKS_METHOD in RTOSConfig.h must be defined 
          as a 1 to use this method.

	See Also:
		TicksToMicroseconds()
*/
UINT32 MicrosecondsToTicks(UINT32 Microseconds);

/*
	const BYTE *ExceptionToString(OS_EXCEPTION_CODE EPCCode)

	Description: This method will return a pointer to a string which
    is the to-stringed conversion of the exception.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		OS_EXCEPTION_CODE EPCCode - A valid OS_EXCEPTION_CODE.  This was more
        than likely generated from an argument in the method GeneralExceptionHandler().

	Returns:
		const BYTE * - A pointer to a human readable string describing the exception.

	Notes:
		- USING_OS_GENERAL_EXCEPTION_TO_STRING and USING_OS_GENERAL_EXCEPTION_HANDLER
          must be set to a 1 to use this method.

	See Also:
		- GeneralExceptionHandler(), GeneralExceptionHandlerUserCallback()
*/
const BYTE *ExceptionToString(OS_EXCEPTION_CODE EPCCode);

/*
	BYTE *KernelVersionToString(BYTE *ToStringBuffer)

	Description: This method will to string the kernel version into a user buffer.
	
	Blocking: No

	User Callable: Yes

	Arguments:
		BYTE * - A pointer to a user buffer that will hold the kernel version in ASCII.

	Returns:
		BYTE * - A pointer to the kernel version in ASCII.

	Notes:
        - USING_KERNEL_VERSION_TO_STRING in RTOSConfig.h must be set to a 1 
          to use this method.
		- The size of the buffer should be at least 5 bytes.

	See Also:
		- None
*/
BYTE *KernelVersionToString(BYTE *ToStringBuffer);

/*
	double GetNumberOfTicksPerMillisecond(void)

	Description: This method will return the Number of OS Ticks that would occur 
    in one millisecond.
	
	Blocking: No

	User Callable: Yes

	Arguments:
        None

	Returns: 
        double - The number of OS ticks per millisecond.

	Notes:
		- None

	See Also:
		- GetNumberOfTicksPerMicrosecond()
*/
#define GetNumberOfTicksPerMillisecond()								(double)((double)OS_TICK_RATE_IN_HZ / (double)1000)

/*
	double GetNumberOfTicksPerMicrosecond(void)

	Description: This method will return the Number of OS Ticks that would occur 
    in one microsecond.
	
	Blocking: No

	User Callable: Yes

	Arguments:
        None

	Returns: 
        double - The number of OS ticks per microsecond.

	Notes:
		- None

	See Also:
		- GetNumberOfTicksPerMillisecond()
*/
#define GetNumberOfTicksPerMicrosecond()								(double)((double)OS_TICK_RATE_IN_HZ / (double)1000000)

/*
	UINT32 GetKernelMajorVersion(void)

	Description: This method will return the major version number of the Kernel.
	
	Blocking: No

	User Callable: Yes

	Arguments:
        None

	Returns: 
        UINT32 - The major version number of the Kernel.

	Notes:
		- None

	See Also:
		- GetKernelMinorVersion(), GetKernelTestVersion()
*/
#define GetKernelMajorVersion()											KERNEL_MAJOR_VERSION_NUMBER

/*
	UINT32 GetKernelMinorVersion(void)

	Description: This method will return the minor version number of the Kernel.
	
	Blocking: No

	User Callable: Yes

	Arguments:
        None

	Returns: 
        UINT32 - The minor version number of the Kernel.

	Notes:
		- None

	See Also:
		- GetKernelMajorVersion(), GetKernelTestVersion()
*/
#define GetKernelMinorVersion()											KERNEL_MINOR_VERSION_NUMBER

/*
	UINT32 GetKernelTestVersion(void)

	Description: This method will return the test version number of the Kernel.  If other
    than zero, the Kernel version is a test release and should not be deployed in the field.
	
	Blocking: No

	User Callable: Yes

	Arguments:
        None

	Returns: 
        UINT32 - The test version number of the Kernel.

	Notes:
		- None

	See Also:
		- GetKernelMajorVersion(), GetKernelMinorVersion()
*/
#define GetKernelTestVersion()											KERNEL_TEST_VERSION_NUMBER






//----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
// End user callable section.  Any method below this point should NEVER be called by the user.                                                                          |
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------|




/*  RTOS Applcation Level  */
	
/*  Highest Level Scheduler Methods */
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
	The following 3 methods are the ONLY 3 methods the RTOS should ever call to add a Task or Tasks to
	the Ready Queue, and are the highest level methods.
*/

/*
	This method will add all the Tasks from the specified Event List, to the Ready Queue.
*/
BOOL OS_AddTaskListToReadyQueue(DOUBLE_LINKED_LIST_HEAD *ListHead);

/*
	This method will add the specified Task to the Ready Queue.
*/
BOOL OS_AddTaskToReadyQueue(TASK *TaskToAddToReadyQueue);

/*
	The following  method is the ONLY method the RTOS should ever call to remove a Task from
	the Ready Queue, and is the highest level methods.
*/

BOOL OS_RemoveTaskFromReadyQueue(TASK *TaskToRemoveFromReadyQueue);

//----------------------------------------------------------------------------------------------------------------------------------

/*
	This is called when the OS performs a Context Switch.  This method gets the next TASK the CPU
	should run and returns a pointer to the Task's Stack.
*/
OS_WORD *OS_NextTask(OS_WORD *CurrentTaskStackPointer);

/*
 * This is just a prototype for the method implemented in ContextSwitch.S
 */
void OS_StartFirstTask(OS_WORD *FirstTaskStackPointer);

// This is eventually called by CreateTask().
OS_WORD *OS_InitializeTaskStack(TASK *Task, TASK_ENTRY_POINT StartingAddress, void *Args, UINT32 StackSizeInWords);

BOOL OS_PlaceTaskOnBlockedList(TASK *Task, DOUBLE_LINKED_LIST_HEAD *BlockedListHead, TASK_NODE *TaskNode, TASK_STATE NewTaskState, BOOL RemoveTaskFromReadyQueue);

DOUBLE_LINKED_LIST_NODE *OS_PriorityRemoveDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head);

void OS_IncrementTaskListPriorities(DOUBLE_LINKED_LIST_HEAD *ListHead);

BOOL OS_ChangeTaskPriority(TASK *Task, BYTE Priority);

void OS_AddTaskToDelayQueue(TASK *Task, TASK_NODE *Node, INT32 TicksToDelay, BOOL RemoveTaskFromReadyQueue);

BOOL OS_AddTaskToRuntimeExecutionList(TASK *Task);

void OS_InitializeTaskNode(TASK_NODE *TaskNode, void *Data);

TASK *OS_RemoveTaskFromList(TASK_NODE *TaskNode);

void OS_AddTaskToList(DOUBLE_LINKED_LIST_HEAD *Head, TASK_NODE *TaskNode);

BOOL OS_PrepTaskForRemoval(TASK *Task);

BOOL OS_DeleteTask(TASK *Task);

BOOL OS_RestartTask(TASK *Task);

#endif // end of Kernel_H
