/*
    NexOS Kernel Version v1.01.05
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
	TASK Description:
		A TASK is a single process which is scheduled by the Kernel (in the CPU scheduler) to be executed.  Each TASK 
		has a priority and a state.  The CPU scheduler will execute the TASK with the highest priority with a READY state.  
		If there are multiple TASKs with the highest priority the CPU scheduler will execute them in a round robin fashion.

		The OS only executes TASKs as there is no other data structure used (in the case of a thread, process, co-routines, etc.).
		TASKs can be created, deleted, restarted, suspended, and resumed.  The user should create at least one TASK prior to 
		starting the OS_Scheduler() method.  Otherwise the kernel could only be used for timers, callbacks and events.
*/

#ifndef TASK_H
	#define TASK_H

#include "Kernel.h"
#include "GenericTypes.h"

/*
	TASK *CreateTask(TASK_ENTRY_POINT StartingAddress, UINT32 StackSizeInWords, BYTE Priority, void *Args, BOOL RestartTask, TASK_STATE StartingTaskState, BOOL Suspendable, BYTE *TaskName, TASK_EXIT_CALLBACK TaskExit, TASK *PreAllocatedTask)

	Description: This method attempts to create the specified TASK.  Once a TASK is created it
		is eligible to execute pending its priority and state.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		TASK_ENTRY_POINT StartingAddress - A pointer to a valid TASK_ENTRY_POINT method.  This is where
		the created TASK will begin execution from.  This parameter is required.

		UINT32 StackSizeInBytes - This is the number of bytes to allocate to the stack
		of the TASK.  Insufficient stack space can cause unpredictable erratic behavior
		by the TASK.  The stack space is allocated in the heap of the OS.  This parameter
		is required.
		
		BYTE Priority - This is the starting priority of the TASK.  This value must be
		between 0 and HIGHEST_USER_TASK_PRIORITY.  If the calling TASK is of lower priority
		than the TASK created, execution will immediately switch to the created TASK once
		this method finishes.  The only time this is not the case is when the OS scheduler 
		has yet to be started.  During this period the system will create a TASK but never 
		switch to it.  This parameter is required.

		void *Args - This is an optional argument to pass to the TASK when it starts execution.
		This argument is visible by the method declaration void(*StartingAddress)(void *Arg).
		
		BOOL RestartTask - Set to TRUE if the created TASK will be eligible for restarting.
		To restart a TASK the USING_RESTART_TASK in RTOSConfig.h must be set to a 1, otherwise
		this parameter has no meaning.
		
		TASK_STATE StartingTaskState - This is the starting state of the TASK.  This can be
		READY, SUSPENDED, or HIBERNATING.  To use the SUSPENDED state USING_SUSPEND_TASK_METHOD
		or USING_SUSPEND_ALL_TASKS_METHOD in RTOSConfig.h must be defined as 1.  To use the 
		HIBERNATING state USING_TASK_HIBERNATION inside of RTOSConfig.h must be defined as 1.
		
		BOOL Suspendable - If this is set to TRUE the TASK can be suspended, otherwise the TASK
		can never be suspended.  Once this parameter is set it cannot be changed and always takes 
		priority over any other suspend method.  For this parameter to have any meaning 
		USING_SUSPEND_TASK_METHOD or USING_SUSPEND_ALL_TASKS_METHOD in RTOSConfig.h must be 
		defined as 1.
		
		BYTE *TaskName - This is a pointer to the name of the TASK.  This is useful for debugging
		purposes.  The name must not exceed TASK_NAME_LENGTH_IN_BYTES with a call to strlen().
		The name passed in is copied to the TASK itself and as such will not be a reference.
		USING_TASK_NAMES must be defined as a 1 in RTOSConfig.h for this parameter to have meaning.
		
		TASK_EXIT_CALLBACK TaskExit - This is an optional argument to pass in.  If a TASK is 
		deleted this method will be called by the Maintenance TASK.  This method must be guranteed
		to not block.  USING_TASK_EXIT_METHOD_CALLBACK inside of RTOSConfig.h must be defined as
		a 1 to use this parameter and feature.
		
		TASK *PreAllocatedTask - An optional pointer to a TASK.  If this is not supplied a TASK
		inside of the OS heap will be allocated.  To have the OS allocate a TASK pass in (TASK*)NULL.
		A TASK cannot be deleted if this parameter is not (TASK*)NULL.

	Returns:
		TASK * - A valid pointer to a TASK on success, (TASK*)NULL otherwise.

	Notes:
		- This method can block if the calling TASK is of lower priority than the TASK being created.
		- InitOS() must be called prior to this method.

	See Also:
		- DeleteTask(), InitOS()
*/
TASK *CreateTask(	TASK_ENTRY_POINT StartingAddress,
					UINT32 StackSizeInBytes,
					BYTE Priority,
					void *Args,

					#if (USING_RESTART_TASK == 1)
						BOOL RestartTask,
					#endif // end of #if (USING_RESTART_TASK == 1)

					#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
						TASK_STATE StartingTaskState,
					#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)

					#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
						BOOL Suspendable,
					#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

					#if(USING_TASK_NAMES == 1)
						BYTE *TaskName,
					#endif // end of #if(USING_TASK_NAMES == 1)

					#if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
						TASK_EXIT_CALLBACK TaskExit,
					#endif // end of #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)

					TASK *PreAllocatedTask);

/*
	BOOL SuspendTask(TASK *Task)

	Description: This method attempts to suspend the specified TASK.  Suspended TASKs will never
    execute until a call to ResumeTask() or ResumeAllTasks() is made.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		TASK *Task - A pointer to a TASK to suspend.  A suspended TASK will never be executed.
		If (TASK*)NULL is passed in for this parameter, the current TASK will be suspended.

	Returns:
		OS_RESULT - OS_SUCCESS if the specified TASK was suspended, a failure value otherwise.

	Notes:
		- TASK *Task must be a valid TASK returned from CreateTask(), or (TASK*)NULL for current TASK.
		- USING_SUSPEND_TASK_METHOD in RTOSConfig.h must be defined as a 1 to use this method.
		- A TASK can suspend itself, hence the Blocking = Potentially.
		- To get a suspended TASK back into the CPU Ready Queue a call to ResumeTask() must be made.
		- A TASK can only be suspended if Suspendable = TRUE when CreateTask() was called.

	See Also:
		- ResumeTask(), ResumeAllTasks()
*/
OS_RESULT SuspendTask(TASK *Task);

/*
	void SuspendAllTasks(BOOL SuspendCurrentTask)

	Description: This method suspends all eligible TASKs in the CPU Ready Queue.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		BOOL SuspendCurrentTask - If TRUE the current TASK will be suspended if suspendable.

	Returns:
		None

	Notes:
		- USING_SUSPEND_ALL_TASKS_METHOD in RTOSConfig.h must be defined as a 1 to use this method.
		- To get a suspended TASK back into the CPU Ready Queue a call to ResumeTask() must be made.
		- A TASK can only be suspended if Suspendable = TRUE when CreateTask() was called.
		- This method is potentially blocking because SuspendCurrentTask can be set to TRUE.

	See Also:
		- ResumeTask(), ResumeAllTasks()
*/
void SuspendAllTasks(BOOL SuspendCurrentTask);

/*
	BOOL ResumeTask(TASK *Task)

	Description: This method resumes a TASK which was previously suspended.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to be resumed.  A resumed TASK will be placed into the CPU Ready Queue.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was resumed, a failure value otherwise.

	Notes:
		- The specified TASK must have previously been put onto the Suspended Queue by a call to 
		  SuspendAllTasks() or SuspendTask().
		- USING_RESUME_TASK_METHOD in RTOSConfig.h must be defined as a 1 to use this method.
		- This method is potentially blocking because the resumed TASK could have a higher priority
		  than the currently executing TASK.
		- TASK *Task must be a valid TASK returned from CreateTask().

	See Also:
		- SuspendAllTasks(), SuspendTask()
*/
OS_RESULT ResumeTask(TASK *Task);

/*
	void ResumeAllTasks(void)

	Description: This method resumes all TASKs on the Suspended Queue.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		None

	Returns:
		None

	Notes:
		- USING_RESUME_ALL_TASKS_METHOD in RTOSConfig.h must be defined as a 1 to use this method.
		- This method is potentially blocking because the resumed TASKs could have a higher priority
		  than the currently executing TASK.

	See Also:
		- SuspendAllTasks(), SuspendTask()
*/
void ResumeAllTasks(void);

/*
	OS_RESULT HibernateTask(TASK *Task)

	Description: This method puts the specified TASK into HIBERNATION mode.  This can be thought of
		as a secondary suspend queue.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		TASK *Task - A pointer to a TASK to put into hibernation.  A hibernating TASK will never 
		be executed.  If (TASK*)NULL is passed in for this parameter, the current TASK will be
		placed into hibernation.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was placed into hibernation, a failure value otherwise.

	Notes:
		- USING_TASK_HIBERNATION in RTOSConfig.h must be defined as a 1 to use this method.
		- This method is potentially blocking because calling TASK can put itself into hibernation.
		- A TASK can only be taken out of hibernation by a call to WakeTask().
		- TASK *Task must be a valid TASK returned from CreateTask(), or (TASK*)NULL for current TASK.

	See Also:
		- WakeTask()
*/
OS_RESULT HibernateTask(TASK *Task);

/*
	OS_RESULT HibernateTask(TASK *Task)

	Description: This method awakens the specified TASK from HIBERNATION mode.  Once awoken the TASK
		is eligible for execution.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		TASK *Task - A pointer to a TASK to wake from hibernation.  A TASK can only be awoken
		it it was put into hibernation by a call to HibernateTask().  Once awoken a TASK is placed
		into the CPU scheduler and eligible for execution.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was woken out of hibernation, a failure value otherwise.

	Notes:
		- USING_TASK_HIBERNATION in RTOSConfig.h must be defined as a 1 to use this method.
		- This method is potentially blocking because calling TASK can put itself into hibernation.
		- A TASK can only be taken out of hibernation by a call to WakeTask().
		- TASK *Task must be a valid TASK returned from CreateTask().

	See Also:
		- HibernateTask()
*/
OS_RESULT WakeTask(TASK *Task);

/*
	BOOL TaskDelayTicks(INT32 TicksToDelay)

	Description: This method causes the current TASK to be taken out of the CPU scheduler for
		the specified number of OS ticks.

	Blocking: Yes

	User Callable: Yes

	Arguments:
		INT32 TicksToDelay - The number of OS ticks to delay.  Once this number has been 
		reached the TASK will be placed back into the CPU scheduler and eligible for 
		execution.  Valid values are from 1 to (2 ^ 31) - 1.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was delayed for the specified number of OS ticks, a failure
		value otherwise.

	Notes:
		- USING_TASK_DELAY_TICKS_METHOD in RTOSConfig.h must be defined as a 1 to use this method.

	See Also:
		TaskDelayMilliseconds(), TaskDelayMicroseconds()
*/
OS_RESULT TaskDelayTicks(INT32 TicksToDelay);

/*
	BOOL TaskDelayMilliseconds(UINT32 MillisecondsToDelay)

	Description: This method causes the current TASK to be taken out of the CPU scheduler for
		the specified number of milliseconds.

	Blocking: Yes

	User Callable: Yes

	Arguments:
		UINT32 MillisecondsToDelay - The number of milliseconds to delay.  Once this number has been
		reached the TASK will be placed back into the CPU scheduler and eligible for execution.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was delayed for the specified number of milliseconds, a failure 
		value otherwise.

	Notes:
		- None

	See Also:
		TaskDelayTicks(), TaskDelayMicroseconds()
*/
#define TaskDelayMilliseconds(MillisecondsToDelay)                         TaskDelayTicks((INT32)MillisecondsToTicks(MillisecondsToDelay))

/*
	BOOL TaskDelayMicroseconds(UINT32 MicrosecondsToDelay)

	Description: This method causes the current TASK to be taken out of the CPU scheduler for
		the specified number of microseconds.

	Blocking: Yes

	User Callable: Yes

	Arguments:
		UINT32 MicrosecondsToDelay - The number of microseconds to delay.  Once this number has been
		reached the TASK will be placed back into the CPU scheduler and eligible for execution.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was delayed for the specified number of microseconds, a failure 
		value otherwise.

	Notes:
		- None

	See Also:
		TaskDelayTicks(), TaskDelayMilliseconds()
*/
#define TaskDelayMicroseconds(MicrosecondsToDelay)                         TaskDelayTicks((INT32)MicrosecondsToTicks(MicrosecondsToDelay))

/*
	BOOL RestartTask(TASK *Task)

	Description: This method attempts to restart the specified TASK.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to be restarted.  Passing in (TASK*)NULL will cause the current TASK
		to restart.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was queued for restarting, a failure value otherwise.

	Notes:
		- USING_RESTART_TASK in RTOSConfig.h must be defined as a 1 to use this method.
		- TASK *Task must be a valid TASK returned from CreateTask(), or (TASK*)NULL for current TASK.
		- RestartTask in CreateTask() must be set to TRUE for a TASK to be eligible for restart.
		- A TASK is not immediately restarted.  It is placed on the Restart Queue and the
		  Maintenance Task will restart it once the Maintenance Task is scheduled to run.

	See Also:
		CreateTask()
*/
OS_RESULT RestartTask(TASK *Task);

/*
	OS_RESULT DeleteTask(TASK *Task)

	Description: This method attempts to delete the specified TASK.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to be deleted.  Passing in (TASK*)NULL will cause the current TASK
		to be deleted.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK was queued for deletion, a failure value otherwise.

	Notes:
		- USING_DELETE_TASK in RTOSConfig.h must be defined as a 1 to use this method.
		- TASK *Task must be a valid TASK returned from CreateTask(), or (TASK*)NULL for current TASK.
		- TASK *PreAllocatedTask must be set to (TASK*)NULL when the TASK was created.  Otherwise
		  the TASK cannot be deleted.
		- A TASK is not immediately deleted.  It is placed on the Delete Queue and the
		  Maintenance Task will delete it once the Maintenance Task is scheduled to run.
		- The delete callback for a TASK is called by the Maintenance Task when it goes
		  to delete the TASK.

	See Also:
		CreateTask()
*/
OS_RESULT DeleteTask(TASK *Task);

/*
	BYTE GetTaskPriority(TASK *Task)

	Description: This method attempts to get the priority of the specified TASK.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to get the priority of.  If (TASK*)NULL is passed in then this
		method will get the priority of the current TASK.

	Returns:
		BYTE - The TASK priority upon success, of INVALID_TASK_PRIORITY upon failure.

	Notes:
		- USING_GET_TASK_PRIORITY_METHOD in RTOSConfig.h must be defined as a 1 to use this method.
		- TASK *Task must be a valid TASK returned from CreateTask(), or (TASK*)NULL for current TASK.

	See Also:
		ChangeTaskPriority()
*/
BYTE GetTaskPriority(TASK *Task);

/*
	BYTE ChangeTaskPriority(TASK *Task, BYTE Priority)

	Description: This method attempts to change the priority of the specified TASK.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to change the priority of.  If (TASK*)NULL is passed in then this
		method will change the priority of the current TASK.

	Returns:
		OS_RESULT - OS_SUCCESS if the TASKs priority was changed, a failure value otherwise.

	Notes:
		- USING_CHANGE_TASK_PRIORITY_METHOD in RTOSConfig.h must be defined as a 1 to use this method.
		- TASK *Task must be a valid TASK returned from CreateTask(), or (TASK*)NULL for current TASK.
		- This method is potentially blocking becuase the current TASK could lower its priority and
		  be preempted.  Or it could elevate the priority of another TASK beyond itself and then be
		  preempted.
		- This method is superseded by starvation protection or a MUTEX. Take the following example.
		  TASK A gets a MUTEX.  TASK B tries to get the same MUTEX but fails to get it since TASK A
		  has it.  TASK B has a higher priority so now lets say TASK A has a + 2 priority (that of B's).
		  Then ChangeTaskPriority(TASK A, Current Priority + 10) is called.  Now TASK A has a + 12 
		  priority from the beginning of this example.  It then gives up the MUTEX.  It's priority was
		  changed by ChangeTaskPriority(), however, once the MUTEX is given up the priority will return
		  to what it was when TASK B tried to get the MUTEX, thus washing away the effects of the call
		  to ChangeTaskPriority().  ChangeTaskPriority() is a dicey method, and may be taken out in
		  subsequent versions of the OS.

	See Also:
		GetTaskPriority()
*/
OS_RESULT ChangeTaskPriority(TASK *Task, BYTE Priority);

/*
	BYTE *GetTaskName(TASK *Task)

	Description: This method attempts to return a pointer to a TASK's name.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to get the name pointer of.  If (TASK*)NULL is passed in then this
		method will return a pointer to the current TASK's name.

	Returns:
		BYTE * - A pointer to the TASK's name upon success, a (BYTE*)NULL pointer otherwise.

	Notes:
		- USING_TASK_NAMES in RTOSConfig.h must be defined as a 1 to use this method.
		- TASK *Task must be a valid TASK returned from CreateTask(), or (TASK*)NULL for current TASK.

	See Also:
		None
*/
BYTE *GetTaskName(TASK *Task);

/*
	OS_RESULT SetupTaskCheckIn(UINT32 TaskCheckInPeriodicityInTicks)

	Description: This method attempts to register the current TASK's check in feature.  Once a
		TASK registers for the check in feature, it must call TaskCheckIn() repeatively in tick
		intervals of TaskCheckInPeriodicityInTicks.  This can be thought of as a software watch
		dog timer.  Once a TASK registers its check in period, it cannot change it, and remains
		in effect until the TASK is either deleted or restarted.

	Blocking: No

	User Callable: Yes

	Arguments:
        TASK_CHECK_IN TaskCheckIn - This is an optional data structure to pass in.  If passed in
        this method will not create a TASK_CHECK_IN in the OS Heap for the current TASK but use the
        one passed in instead.  This saves memory and space in the OS Heap.
 
		UINT32 TaskCheckInPeriodicityInTicks - This is how often in OS ticks the TASK must call
		TaskCheckIn().  This value must be at least a value of 2 or greater.  If the TASK does 
		not call TaskCheckIn() in time, the TASK can be deleted or restarted depending on the 
		return value of TaskCheckInUserCallback().

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK successfully registered for the check in feature, a 
		failure value otherwise.

	Notes:
		- USING_TASK_CHECK_IN in RTOSConfig.h must be defined as a 1 to use this method.
		- This method always applies to the current TASK when called.
		- This method can only be called once per TASK, unless the TASK is restarted.

	See Also:
		TaskCheckIn()
*/
OS_RESULT SetupTaskCheckIn(TASK_CHECK_IN *TaskCheckIn, UINT32 TaskCheckInPeriodicityInTicks);

/*
	OS_SUCCESS TaskCheckIn(void)

	Description: This method attempts to check in for the current TASK if the task check in
		feature is enabled for the current TASK.  TaskCheckIn() must be called in intervals of 
		at least TaskCheckInPeriodicityInTicks from SetupTaskCheckIn() for the currently executing
		TASK.  If not the currently executed TASK can be deleted or restarted automatically.

	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns:
		OS_RESULT - OS_SUCCESS if the TASK successfully checked in, a error value otherwise.

	Notes:
		- USING_TASK_CHECK_IN in RTOSConfig.h must be defined as a 1 to use this method.
		- This method always applies to the current TASK when called.

	See Also:
		SetupTaskCheckIn()
*/
OS_RESULT TaskCheckIn(void);

/*
	void OS_WaitOnSignals(SIGNALS *TaskSignals, UINT16 SignalsToWaitOn)

	Description: This method sets the bits specified in SignalsToWaitOn to the WaitOnSignals of
		TaskSignals.  This only preps the TASK owner of TaskSignals for waiting on the signals.
		This method does not know if the OS or User signals are being set.  This is determined
		by which one is passed in for TaskSignals.  This method also clears any signals that
		are currently set in TaskSignals. 

	Blocking: No

	User Callable: No

	Arguments:
		SIGNALS *TaskSignals - A pointer to a valid SIGNALS data structure.  The WaitOnSignals 
		member of TaskSignals will be set to SignalsToWaitOn.  Once all these bits are set in
		the CurrentlySetSignals the TASK will resume.

		UINT16 SignalsToWaitOn - The signals that the TASK will wait on.  For the TASK to wake,
		CurrentlySetSignals bitwise ANDed with SignalsToWaitOn must equal CurrentlySetSignals.

	Returns:
		None

	Notes:
		- USING_TASK_SIGNAL in RTOSConfig.h must be defined as a 1 to use this method.
		- This method should be called before OS_BlockOnSignals() as it preps the TASK owner of 
		  TaskSignals for blocking.

	See Also:
		OS_BlockOnSignals(), OS_SetTaskSignals()
*/
void OS_WaitOnSignals(SIGNALS *TaskSignals, UINT16 SignalsToWaitOn);

/*
	void OS_BlockOnSignals(TASK *Task, BYTE TaskNodeIndex, BOOL RemoveFromReadyQueue)

	Description: This method potentially removes the specified TASK from the CPU Scheduler 
		and places it on the Signals wait list using TASK_NODE specified by TaskNodeIndex.

	Blocking: Yes

	User Callable: No

	Arguments:
		TASK *Task - The TASK to put on the Signals wait list.

		BYTE TaskNodeIndex - The index used in the TaskNodeArray to specify which TASK_NODE to
		use for the Signals wait list.

		BOOL RemoveFromReadyQueue - If TRUE the specified TASK will be removed from the ready queue.

	Returns:
		None

	Notes:
		- USING_TASK_SIGNAL in RTOSConfig.h must be defined as a 1 to use this method.
		- After calling this method SurrenderCPU() should be called before returning control to the user.

	See Also:
		OS_WaitOnSignals(), OS_SetTaskSignals()
*/
void OS_BlockOnSignals(TASK *Task, BYTE TaskNodeIndex, BOOL RemoveFromReadyQueue);

/*
	BOOL OS_SetTaskSignals(TASK *Task, SIGNALS *TaskSignals, UINT16 SignalsToSet)

	Description: This method sets the signals of the specified TASK.  The signals could
		be either the OS or User signals.  This method will also add the specified TASK
		to the CPU scheduler if it is awoken due to the signals being set.

	Blocking: No

	User Callable: No

	Arguments:
		TASK *Task - The TASK to set the signals of.

		SIGNALS *TaskSignals - A pointer to the TASK's signals to set, either OS or User.

		UINT16 SignalsToSet - The signals to set in the TaskSignals.

	Returns:
		BOOL - TRUE if the TASK was added to the CPU Scheduler and is of higher priority 
		than the currently running TASK.

	Notes:
		- USING_TASK_SIGNAL in RTOSConfig.h must be defined as a 1 to use this method.
		- If this method return TRUE, SurrenderCPU() should be called before returning control to 
		  the user.

	See Also:
		OS_BlockOnSignals(), OS_WaitOnSignals()
*/
BOOL OS_SetTaskSignals(TASK *Task, SIGNALS *TaskSignals, UINT16 SignalsToSet);

/*
	OS_RESULT TaskWaitOnSignals(UINT16 SignalsToWaitOn, INT32 TimeoutInTicks)

	Description: This method attempts to wait for the specified signals to be
		set.  This method always pertains to the currently executing TASK.

	Blocking: Yes

	User Callable: Yes

	Arguments:
		UINT16 SignalsToWaitOn - These are all the signals to wait to be set before TASK
		execution resumes.

		INT32 TimeoutInTicks - This is a timeout value in ticks to wait
		for the signal.  Below are valid values for TimeoutInTicks.

            TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
            Delayed Queue up to the specified number of ticks if
            the signals do not occur.

            TimeoutInTicks = 0: The calling TASK will not be placed on the
            Delayed Queue and this method will return OS_INVALID_ARGUMENT.

            TimeoutInTicks <= -1: The calling TASK will not be placed on the
            Delay Queue and will wait forever for the signals.

	Returns:
		OS_RESULT - Returns OS_SUCCESS if the signals were set within the specified time.  A
		failure value otherwise.

	Notes:
		- USING_TASK_SIGNAL in RTOSConfig.h must be defined as a 1 to use this method.

		- USING_TASK_DELAY_TICKS_METHOD in RTOSConfig.h must be defined as a 1 to use
		  the timeout feature of this method.

	See Also:
		SignalTask()
*/
OS_RESULT TaskWaitOnSignals(UINT16 SignalsToWaitOn

							#if (USING_TASK_DELAY_TICKS_METHOD == 1)
								, INT32 TimeoutInTicks
							#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

							);

/*
	OS_RESULT TaskGetSetSignals(TASK *Task, UINT16 *SignalsSet)

	Description: This method attempts to get the currently set signals of a TASK.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to get the signals of.

		UINT16 *SignalsSet - A pointer to a UINT16 to place the currently set signals of the
		TASK into.

	Returns:
		OS_RESULT - Returns OS_SUCCESS if the signals were retrieved.  A failure value otherwise.

	Notes:
		- USING_TASK_SIGNAL in RTOSConfig.h must be defined as a 1 to use this method.

		- This method always returns the User signals currently set.

	See Also:
		SignalTask()
*/
UINT16 TaskGetSetSignals(TASK *Task);

/*
	OS_RESULT SignalTask(TASK *Task, UINT16 SignalsToSet)

	Description: This method attempts to set the specified signals of a TASK.  If a TASK
		is not waiting for signals, this method will not set the signals and fail.  For a
		TASK to be waiting on signals it must have called TaskWaitOnSignals().

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to set the signals of.

		UINT16 SignalsToSet - The signals to set.  These are bitwise OR'ed with the currently
		set signals fo the TASK.

	Returns:
		OS_RESULT - Returns OS_SUCCESS if the signals were set.  A failure value otherwise.

	Notes:
		- USING_TASK_SIGNAL in RTOSConfig.h must be defined as a 1 to use this method.
		- A TASK must be currently waiting for signals to allow the signals to be set.
		- This method is potentially blocking because the calling TASK could make a higher
		  priority TASK ready.

	See Also:
		TaskWaitOnSignals()
*/
OS_RESULT SignalTask(TASK *Task, UINT16 SignalsToSet);

/*
	OS_RESULT TaskGetState(TASK *Task, TASK_STATE *TaskState)

	Description: This method attempts to get the specified TASK's TASK_STATE.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - The TASK to get the TASK_STATE of.

		TASK_STATE *TaskState - A pointer to a valid TASK_STATE.  The TASK_STATE of the
		specified TASK will be placed to where the pointer is pointing to.

	Returns:
		OS_RESULT - Returns OS_SUCCESS if the TASK_STATE was successfully got.  A failure
		value otherwise.

	Notes:
		- USING_TASK_GET_STATE_METHOD in RTOSConfig.h must be defined as a 1 to use this method.

	See Also:
		None
*/
TASK_STATE TaskGetState(TASK *Task);

/*
	void *TaskGetLocalStoragePointer(TASK *Task, UINT32 Index)

	Description: This method returns the value at the index of the TASK's local storage array.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - A valid TASK returned from CreateTask(), or (TASK*)NULL for the current TASK.

		UINT32 Index - The index in the array to get the data from.  This index cannot be 
        greater than or equal to NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS.

	Returns:
		void * - The data located at Index of the TASK's local storage array.

	Notes:
		- USING_TASK_LOCAL_STORAGE_ACCESS in RTOSConfig.h must be defined as a 1 to use this method.
        - NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS in RTOSConfig.h must be defined as a value > 0.

	See Also:
		TaskSetLocalStoragePointer()
*/
void *TaskGetLocalStoragePointer(TASK *Task, UINT32 Index);

/*
	BOOL TaskSetLocalStoragePointer(TASK *Task, UINT32 Index, void *Value)

	Description: This method sets the value at the index of the TASK's local storage array.

	Blocking: No

	User Callable: Yes

	Arguments:
		TASK *Task - A valid TASK returned from CreateTask(), or (TASK*)NULL for the current TASK.

		UINT32 Index - The index in the array to set the data of.  This index cannot be 
        greater than or equal to NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS.
 
        void *Value - The value to write into the TASK's local storage array.

	Returns:
        OS_RESULT - Returns OS_SUCCESS if the value was written.  A failure value otherwise.

	Notes:
		- USING_TASK_LOCAL_STORAGE_ACCESS in RTOSConfig.h must be defined as a 1 to use this method.
        - NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS in RTOSConfig.h must be defined as a value > 0.

	See Also:
		TaskGetLocalStoragePointer()
*/
OS_RESULT TaskSetLocalStoragePointer(TASK *Task, UINT32 Index, void *Value);

/*
	UINT32 GetNumberOfTasks(void)

	Description: This method returns the number of TASKS which are currently managed by the Kernel.

	Blocking: No

	User Callable: Yes

	Arguments:
        None

	Returns:
        UINT32 - The number of TASK's currently managed by the Kernel.  THis number includes TASK's
        of any stated (READY, RESTARTING, SUSPENDED, BLOCKED, etc.) This number also includes TASK's
        which were marked for deletion, but have yet to be deleted.

	Notes:
		- USING_GET_NUMBER_OF_TASKS_METHOD in RTOSConfig.h must be defined as a 1 to use this method.

	See Also:
        - None
*/
UINT32 GetNumberOfTasks(void);

/*
	UINT32 AnalyzeTaskStack(TASK *Task)

	Description: This method returns the number of unused words in a TASK stack.  This denotes
    the high water mark inside the TASK stack.

	Blocking: No

	User Callable: Yes

	Arguments:
        TASK *Task - A valid TASK returned from CreateTask(), or (TASK*)NULL for the current TASK.

	Returns:
        UINT32 - The number of bytes unused in the TASK stack.

	Notes:
		- ANALYZE_TASK_STACK_USAGE in RTOSConfig.h must be defined as a 1 to use this method.
        - A unique value for TASK_STACK_FILL_VALUE inside of RTOSConfig.h should be chosen.
        - PortAnaylzeTaskStackUsage() inside of Port.c must also be implemented for
          use of this function.

	See Also:
        - AnaylzeSystemStackUsage()
*/
UINT32 AnalyzeTaskStack(TASK *Task);

#endif // end of TASK_H
