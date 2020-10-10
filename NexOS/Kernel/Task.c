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

#include <string.h>

#include "Task.h"
#include "../../Generic Libraries/Double Linked List/DoubleLinkedList.h"
#include "Kernel.h"
#include "CriticalSection.h"
#include "Memory.h"

extern TASK * volatile gCurrentTask;
extern DOUBLE_LINKED_LIST_HEAD gCPUScheduler[];

#if (USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_SUSPEND_TASK_METHOD == 1)
	extern DOUBLE_LINKED_LIST_HEAD gSuspendedQueueHead;
#endif // end of USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_SUSPEND_TASK_METHOD == 1

#if (USING_TASK_HIBERNATION == 1 || USING_TASK_SIGNAL == 1)
	extern DOUBLE_LINKED_LIST_HEAD gMiscellaneousBlockedQueueHead;
#endif // end of #if (USING_TASK_HIBERNATION == 1 || USING_TASK_SIGNAL == 1)
    
#if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
    UINT32 gNumberOfTasks = 0;
#endif // end of #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)

#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
	static BOOL OS_SuspendTask(TASK *Task)
	{
		// first check to see if the task can be suspended, if not, return
		if (Task->TaskInfo.bits.Suspendable == FALSE || Task->TaskInfo.bits.State != READY)
		{
			return FALSE;
		}

		OS_PlaceTaskOnBlockedList(Task, &gSuspendedQueueHead, &Task->TaskNodeArray[PRIMARY_TASK_NODE], SUSPENDED, TRUE);

		return TRUE;
	}
#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

static TASK *OS_CreateTask(TASK_ENTRY_POINT StartingAddress,
							UINT32 StackSizeInWords,
							BYTE Priority,

							#if(USING_RESTART_TASK == 1)
								BOOL RestartTask,
							#endif // end of #if(USING_RESTART_TASK == 1)

							#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
								TASK_STATE StartingTaskState,
							#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
							
							#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
								BOOL Suspendable,
							#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

							#if (USING_TASK_NAMES == 1)
								BYTE *TaskName,
							#endif // end of #if (USING_TASK_NAMES == 1)

						   void *Args,

						   #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
							   TASK_EXIT_CALLBACK TaskExit,
						   #endif // end of #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)

						   TASK *PreAllocatedTask)
{
	UINT32 i;
	TASK *NewTask = PreAllocatedTask;

	if(NewTask == (TASK*)NULL)
	{
		NewTask = (TASK*)AllocateMemory(sizeof(TASK));

		if (NewTask == (TASK*)NULL)
		{
			return (TASK*)NULL;
		}
	}
	
	// initialize the task stack and args
	NewTask->TaskStackPointer = OS_InitializeTaskStack(NewTask, StartingAddress, Args, StackSizeInWords);

	if(NewTask->TaskStackPointer == (OS_WORD*)NULL)
	{
		// Only release the TASK if the user didn't supply one
		if(PreAllocatedTask == (TASK*)NULL)
			ReleaseMemory((void*)NewTask);

		return (TASK*)NULL;
	}
    
    NewTask->TaskInfo.Word = 0;

	#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
		NewTask->TaskInfo.bits.State = StartingTaskState;
	#else
		NewTask->TaskInfo.bits.State = READY;
	#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)

	NewTask->TaskInfo.bits.Priority = Priority;

	for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
		OS_InitializeTaskNode(&NewTask->TaskNodeArray[i], (void*)NewTask);

	NewTask->CriticalCount = 0;

	#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
		NewTask->TaskInfo.bits.Suspendable = Suspendable;
	#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		NewTask->DelayInTicks = 0;
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	#if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
		NewTask->TaskExitMethod = TaskExit;
	#endif // end of USING_TASK_EXIT_METHOD_CALLBACK

	#if (USING_TASK_NAMES == 1)
		if (TaskName != (BYTE*)NULL)
		{
			// if it's too big, truncate it
			if (strlen((const char*)TaskName) > TASK_NAME_LENGTH_IN_BYTES)
				TaskName[TASK_NAME_LENGTH_IN_BYTES] = 0;

			strcpy((char*)(NewTask->TaskName), (char*)TaskName);
		}
	#endif // end of USING_TASK_NAMES

    #if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)
        for(i = 0; i < NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS; i++)
            NewTask->TaskLocalThreadStorageArray[i] = (void*)NULL;
    #endif // end of #if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)

	#if(USING_RESTART_TASK == 1)
 		if(RestartTask == TRUE)
		{
			// the user wants us to setup the task for restart
			if((NewTask->RestartInfo = (TASK_RESTART_INFO*)AllocateMemory(sizeof(TASK_RESTART_INFO))) == NULL)
			{
				// if we failed to allocate the structre, abandon the operation.
				// free up the stack space
				ReleaseMemory((void*)NewTask->StartOfTaskStackPointer);
				
				// if the user didn't pass in a TASK, then we allocated it, so free it
				if(PreAllocatedTask == (TASK*)NULL)
					ReleaseMemory((void*)NewTask);

        		return (TASK*)NULL;	
			}
			else
			{
				// assign all needed data for restarting a TASK
				NewTask->RestartInfo->StartingAddress = StartingAddress;
				NewTask->RestartInfo->TaskPriority = Priority;

				#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
					NewTask->RestartInfo->TaskState = StartingTaskState;
				#else
					NewTask->RestartInfo->TaskState = READY;
				#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)

				NewTask->RestartInfo->StartingTaskStackSizeInWords = StackSizeInWords;
			}
		}
		else
		{
			NewTask->RestartInfo = (TASK_RESTART_INFO*)NULL;
		}	
	#endif // end of (USING_RESTART_TASK == 1)

	#if(USING_TASK_CHECK_IN == 1)
		NewTask->TaskCheckIn = (TASK_CHECK_IN*)NULL;
	#endif // end of USING_TASK_CHECK_IN

	#if (USING_TASK_SIGNAL == 1)
		NewTask->TaskSignal.OS_Signals.Word = 0;
		NewTask->TaskSignal.UserSignals.Word = 0;
	#endif // end of #if (USING_TASK_SIGNAL == 1)

    #if(USING_DELETE_TASK == 1 && (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_PIPE_DELETE_FROM_ISR_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1))
        NewTask->DeleteBlockCounter = (UINT32*)NULL;
    #endif // end of #if(USING_DELETE_TASK == 1 && (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_PIPE_DELETE_FROM_ISR_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1))

    #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
        gNumberOfTasks++;
    #endif // end of #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
        
	return NewTask;
}

TASK *CreateTask(TASK_ENTRY_POINT StartingAddress,
				UINT32 StackSizeInWords,
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

				TASK *PreAllocatedTask)
{
	TASK *Task;

    #if (USING_CHECK_TASK_PARAMETERS == 1)
        if(PreAllocatedTask != (TASK*)NULL)
            if(RAMAddressValid((OS_WORD)PreAllocatedTask) == FALSE)
                return (TASK*)NULL;

        if(ProgramAddressValid((OS_WORD)StartingAddress) == FALSE)
            return (TASK*)NULL;

        #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
            if(TaskExit != (TASK_EXIT_CALLBACK)NULL)
            {
                if(ProgramAddressValid((OS_WORD)TaskExit) == FALSE)
                    return (TASK*)NULL;
            }
        #endif // end of #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)

        #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
            if (StartingTaskState == BLOCKED || StartingTaskState == RESTARTING || StartingTaskState == DELETING || StartingTaskState >= NUMBER_OF_TASK_STATES)
                return (TASK*)NULL;
        #endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)

        #if(USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
            // they cannot have a SUSPENDED state and if they are not suspendable.
            if (Suspendable == FALSE && StartingTaskState == SUSPENDED)
                return (TASK*)NULL;
        #endif // end of #if(USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

        if (Priority > HIGHEST_USER_TASK_PRIORITY)
            return (TASK*)NULL;

        if(StackSizeInWords < MINIMUM_STACK_SIZE)
            return (TASK*)NULL;

        #if(USING_TASK_NAMES == 1)
            if(TaskName != (BYTE*)NULL)
            {
                if(RAMAddressValid((OS_WORD)TaskName) == FALSE && ProgramAddressValid((OS_WORD)TaskName) == FALSE)
                    return (TASK*)NULL;
            }
        #endif // end of USING_TASK_NAMES
    #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

	EnterCritical();

	Task = OS_CreateTask(	StartingAddress,
							StackSizeInWords,
							Priority,

							#if (USING_RESTART_TASK == 1)
								RestartTask,
							#endif // end of #if (USING_RESTART_TASK == 1)

							#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
								StartingTaskState,
							#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)

							#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
								Suspendable,
							#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

							#if(USING_TASK_NAMES == 1)
								TaskName,
							#endif // end of #if(USING_TASK_NAMES == 1)

							Args,

							#if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
								TaskExit,
							#endif // end of #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)

							PreAllocatedTask);

	// if we got a valid TASK back, we can add it to whatever list it should be on.
	if(Task != (TASK*)NULL)
	{
		#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
			switch (StartingTaskState)
			{
				case READY:
				{
					if (OS_AddTaskToReadyQueue(Task) == TRUE)
						SurrenderCPU();

					break;
				}

				#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
					case SUSPENDED:
					{
						OS_PlaceTaskOnBlockedList(Task, &gSuspendedQueueHead, &Task->TaskNodeArray[PRIMARY_TASK_NODE], SUSPENDED, FALSE);

						break;
					}
				#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

				#if (USING_TASK_HIBERNATION == 1)
					case HIBERNATING:
					{
						OS_PlaceTaskOnBlockedList(Task, &gMiscellaneousBlockedQueueHead, &Task->TaskNodeArray[PRIMARY_TASK_NODE], HIBERNATING, FALSE);

						break;
					}
				#endif // end of #if (USING_TASK_HIBERNATION == 1)

				default:
				{
					// something has gone horrifically wrong.

					// clean up any memory allocated to the TASK
					ReleaseMemory((void*)Task->TaskStackPointer);

					#if (USING_RESTART_TASK == 1)
						if (Task->RestartInfo != (TASK_RESTART_INFO*)NULL)
							ReleaseMemory((void*)Task->RestartInfo);
					#endif // end of #if (USING_RESTART_TASK == 1)

					// now we can release the TASK if they did not supply one
					if (PreAllocatedTask == (TASK*)NULL)
						ReleaseMemory((void*)Task);

					Task = (TASK*)NULL;

					break;
				}
			}
		#else
			if (OS_AddTaskToReadyQueue(Task) == TRUE)
				SurrenderCPU();
		#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
	}

	ExitCritical();

	return Task;
}

#if (USING_SUSPEND_TASK_METHOD == 1)
	OS_RESULT SuspendTask(TASK *Task)
	{
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if(Task != (TASK*)NULL)
            {
                if(RAMAddressValid((OS_WORD)Task) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            }
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		if (Task == (TASK*)NULL)
		{
			// the TASK is NULL, so make it the current task
			Task = gCurrentTask;
		}
            
        EnterCritical();

		if (OS_SuspendTask(Task) == FALSE)
		{
			ExitCritical();

			return OS_INVALID_OBJECT_USED;
		}
		
		// If TaskToSuspend is the currently executing Task or if NULL was passed in, we will Suspend ourselves.
		if(Task == gCurrentTask)
			SurrenderCPU();
	
	    ExitCritical();
	    
		return OS_SUCCESS;
	}
#endif // end of USING_SUSPENDED_QUEUE_METHOD

#if (USING_SUSPEND_ALL_TASKS_METHOD == 1)
	static void OS_SuspendTaskList(DOUBLE_LINKED_LIST_HEAD *TaskList)
	{
		DOUBLE_LINKED_LIST_NODE *TaskNode;
		TASK *TempTask;
		BOOL TaskSuspended = FALSE;

		do
		{
			// if the list is NULL, just return
			if (TaskList->Beginning == (DOUBLE_LINKED_LIST_NODE*)NULL)
				return;

			TaskNode = TaskList->Beginning;

			TempTask = (TASK*)TaskNode->Data;

			do
			{
				if (TempTask->TaskInfo.bits.Suspendable == TRUE)
				{
					OS_SuspendTask(TempTask);

					TaskNode = TaskList->Beginning;

					TaskSuspended = TRUE;

					break;
				}

				// iterate to the next Task
				TaskNode = TaskNode->NextNode;

				TempTask = (TASK*)TaskNode->Data;
			} 
			while (TaskNode != TaskList->Beginning);

			if (TaskSuspended == FALSE)
				return;

			TaskSuspended = FALSE;
		} 
		while (1);
	}

	void SuspendAllTasks(BOOL SuspendCurrentTask)
	{
		BYTE i;
		BOOL PriorSuspendProperty;

	    EnterCritical();

		// get this for use later on
		PriorSuspendProperty = gCurrentTask->TaskInfo.bits.Suspendable;

		if (SuspendCurrentTask == FALSE)
			gCurrentTask->TaskInfo.bits.Suspendable = FALSE;

		// go one priority to the next and disable each task if possible
		for (i = 0; i < HIGHEST_USER_TASK_PRIORITY; i++)
			OS_SuspendTaskList(&gCPUScheduler[i]);

		// now unconditionally set the suspendable property back
		gCurrentTask->TaskInfo.bits.Suspendable = PriorSuspendProperty;

		// give control back to the scheduler to run the next potential task.
		// worst case is the same task runs and this was uneeded, but oh well.
		SurrenderCPU();

	    ExitCritical();
	}
#endif // end of USING_SUSPEND_ALL_TASKS_METHOD

#if (USING_RESUME_TASK_METHOD == 1)
	OS_RESULT ResumeTask(TASK *TaskToResume)
	{
		OS_RESULT ReturnValue;

        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if(RAMAddressValid((OS_WORD)TaskToResume) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

	    EnterCritical();
	
		// If the Tasks state is SUSPENDED continue along, otherwise its either already READY, or BLOCKED, so just exit.
		if(TaskToResume->TaskInfo.bits.State == SUSPENDED)
		{
			OS_RemoveTaskFromList(&TaskToResume->TaskNodeArray[PRIMARY_TASK_NODE]);

			// Add the specified Task to the Ready Queue.
			if (OS_AddTaskToReadyQueue(TaskToResume))
			{
				// the task we added has a higher priority than us
				SurrenderCPU();
			}
			
			ReturnValue = OS_SUCCESS;
		}
		else
		{
			ReturnValue = OS_INVALID_TASK_STATE;
		}

	    ExitCritical();
	
		return ReturnValue;
	}
#endif // end of USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_SUSPEND_TASK_METHOD == 1

#if (USING_RESUME_ALL_TASKS_METHOD == 1)
	void ResumeAllTasks(void)
	{
		EnterCritical();
		
		if (OS_AddTaskListToReadyQueue(&gSuspendedQueueHead))
			SurrenderCPU();
		
		ExitCritical();
	}
#endif // end of USING_RESUME_ALL_TASKS_METHOD

#if (USING_DELETE_TASK == 1)
	OS_RESULT DeleteTask(TASK *Task)
	{
		OS_RESULT ReturnValue;

		if(Task == (TASK*)NULL)
            Task = gCurrentTask;
        
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (AddressInHeap((OS_WORD)Task) == FALSE)
            {
                return OS_RESOURCE_NOT_IN_OS_HEAP;
            }
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		if(OS_DeleteTask(Task) == TRUE)
		{
			if(Task == gCurrentTask)
				SurrenderCPU();
	
			ReturnValue = OS_SUCCESS;
		}
		else
		{
			ReturnValue = OS_INVALID_OBJECT_USED;
		}

		ExitCritical();

		return ReturnValue;
	}
#endif // end of USING_DELETE_TASK

#if (USING_TASK_DELAY_TICKS_METHOD == 1)
	OS_RESULT TaskDelayTicks(UINT32 TicksToDelay)
	{
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (TicksToDelay == 0 || TicksToDelay >= INVALID_TIMEOUT_TICKS_VALUE)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], TicksToDelay, TRUE);

		SurrenderCPU();

		// clear out the tick counter before leaving
		gCurrentTask->DelayInTicks = 0;

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of USING_TASK_DELAY_TICKS_METHOD

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------
	Nothing below this line has been tested as of yet!!! February 16, 2018.
-----------------------------------------------------------------------------------------------------------------------------------------------------
*/

#if (USING_TASK_CHECK_IN == 1)
	extern DOUBLE_LINKED_LIST_HEAD gTaskCheckInList;

	OS_RESULT SetupTaskCheckIn(TASK_CHECK_IN *TaskCheckIn, UINT32 TaskCheckInPeriodicityInTicks)
	{
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (TaskCheckInPeriodicityInTicks <= 1)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)
	
		EnterCritical();

		if (gCurrentTask->TaskCheckIn != (TASK_CHECK_IN*)NULL)
		{
			ExitCritical();

			return OS_INVALID_OBJECT_USED;
		}

        // check to see if they passed in a TASK_CHECK_IN
        if(TaskCheckIn == (TASK_CHECK_IN*)NULL)
        {
            // they didn't pass in a TASK_CHECK_IN, so we will make one in the OS Heap.
            gCurrentTask->TaskCheckIn = (TASK_CHECK_IN*)AllocateMemory(sizeof(TASK_CHECK_IN));

            if(gCurrentTask->TaskCheckIn == (TASK_CHECK_IN*)NULL)
            {
                ExitCritical();

                return OS_ALLOCATE_MEMORY_FAILED;
            }
        }
        else
        {
            // just use the one they passed in
            gCurrentTask->TaskCheckIn = TaskCheckIn;
        }
	
		gCurrentTask->TaskCheckIn->Timeout = TaskCheckInPeriodicityInTicks;
		gCurrentTask->TaskCheckIn->CurrentCount = 0;

		OS_InitializeTaskNode(&(gCurrentTask->TaskCheckIn->TaskCheckInNode), (void*)gCurrentTask);
		
		// now add it to the check in list
		OS_AddTaskToList(&gTaskCheckInList, &(gCurrentTask->TaskCheckIn->TaskCheckInNode));

		ExitCritical();
	
		return OS_SUCCESS;
	}

	OS_RESULT TaskCheckIn(void)
	{
		EnterCritical();

		if (gCurrentTask->TaskCheckIn == (TASK_CHECK_IN*)NULL)
		{
			ExitCritical();

			return OS_INVALID_OBJECT_USED;
		}

		gCurrentTask->TaskCheckIn->CurrentCount = 0;

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of USING_TASK_CHECK_IN

#if (USING_TASK_HIBERNATION == 1)
	OS_RESULT HibernateTask(TASK *Task)
	{
		if (Task == (TASK*)NULL)
		{
            Task = gCurrentTask;
		}
		else
		{
            #if (USING_CHECK_TASK_PARAMETERS == 1)
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)
		}

		EnterCritical();

		if (Task->TaskInfo.bits.State != READY)
		{
			ExitCritical();

			return OS_INVALID_TASK_STATE;
		}

		OS_PlaceTaskOnBlockedList(Task, &gMiscellaneousBlockedQueueHead, &Task->TaskNodeArray[PRIMARY_TASK_NODE], HIBERNATING, TRUE);

		ExitCritical();

		return OS_SUCCESS;
	}

	OS_RESULT WakeTask(TASK *Task)
	{
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)Task) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		if (Task->TaskInfo.bits.State != HIBERNATING)
		{
			ExitCritical();

			return OS_INVALID_TASK_STATE;
		}

		// remove the task from the hibernation list
		OS_RemoveTaskFromList(&Task->TaskNodeArray[PRIMARY_TASK_NODE]);

		// Remove the Task from the Suspended List.
		if (OS_AddTaskToReadyQueue(Task))
			SurrenderCPU();

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of USING_TASK_HIBERNATION

#if (USING_TASK_NAMES == 1)
	BYTE *GetTaskName(TASK *Task)
	{
		if (Task == (TASK*)NULL)
		{
            Task = gCurrentTask;
		}
		else
		{
            #if (USING_CHECK_TASK_PARAMETERS == 1)
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                    return (BYTE*)NULL;
            #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)
		}

		return Task->TaskName;
	}
#endif // end of USING_TASK_NAMES

#if (USING_CHANGE_TASK_PRIORITY_METHOD == 1)
	OS_RESULT ChangeTaskPriority(TASK *Task, BYTE Priority)
	{
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (Priority > HIGHEST_USER_TASK_PRIORITY)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		if (Task == (TASK*)NULL)
		{
            Task = gCurrentTask;
		}
		else
		{
			#if (USING_CHECK_TASK_PARAMETERS == 1)
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                {
                    return OS_INVALID_ARGUMENT_ADDRESS;
                }
            #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)
		}

		EnterCritical();

		if (OS_ChangeTaskPriority(Task, Priority) == TRUE)
			SurrenderCPU();

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of USING_CHANGE_TASK_PRIORITY_METHOD

#if (USING_RESTART_TASK == 1)
	OS_RESULT RestartTask(TASK *Task)
	{
		OS_RESULT ReturnValue;

		if (Task == (TASK*)NULL)
		{
            Task = gCurrentTask;
		}
		else
		{
            #if (USING_CHECK_TASK_PARAMETERS == 1)
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)
		}

		EnterCritical();

        // do they have a valid restart info block?
		if (RAMAddressValid((OS_WORD)(Task->RestartInfo)) == FALSE)
		{
            // they did not create the TASK with the intent of restarting it
            // and the restart info block is invalid
			ReturnValue = OS_INVALID_OBJECT_USED;
		}
		else
		{
            // the restart info block is valid
            
			if (OS_RestartTask(Task) == TRUE)
			{
                // if the TASK to restart was us, release the CPU
				if (Task == gCurrentTask)
					SurrenderCPU();

				ReturnValue = OS_SUCCESS;
			}
			else
			{
				ReturnValue = OS_INVALID_OBJECT_USED;
			}
		}

		ExitCritical();

		return ReturnValue;
	}
#endif // end of USING_RESTART_TASK

#if (USING_GET_TASK_PRIORITY_METHOD == 1)
	BYTE GetTaskPriority(TASK *Task)
	{
		BYTE TaskPriority;

        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (Task != (TASK*)NULL)
            {
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                    return INVALID_TASK_PRIORITY;
            }
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		if (Task == (TASK*)NULL)
			Task = gCurrentTask;

		TaskPriority = Task->TaskInfo.bits.Priority;

		ExitCritical();

		return TaskPriority;
	}
#endif // end of USING_GET_TASK_PRIORITY_METHOD

#if (USING_TASK_SIGNAL == 1)
	void OS_WaitOnSignals(SIGNALS *TaskSignals, UINT16 SignalsToWaitOn)
	{
		TaskSignals->SignalFlags.WaitOnSignals.Word = SignalsToWaitOn;

		TaskSignals->SignalFlags.CurrentlySetSignals.Word = 0;
	}

	void OS_BlockOnSignals(TASK *Task, BYTE TaskNodeIndex, BOOL RemoveFromReadyQueue)
	{
		OS_PlaceTaskOnBlockedList(Task, &gMiscellaneousBlockedQueueHead, &Task->TaskNodeArray[TaskNodeIndex], BLOCKED, RemoveFromReadyQueue);
	}

	BOOL OS_SetTaskSignals(TASK *Task, SIGNALS *TaskSignals, UINT16 SignalsToSet)
	{
		// It is possible that the TASK was set to the READY state if a timeout occurred.
		// So we must check that it is in the BLOCKED state prior to mucking with the signal bits.
		if (Task->TaskInfo.bits.State == BLOCKED)
		{
			// set the new signals
			TaskSignals->SignalFlags.CurrentlySetSignals.Word |= SignalsToSet;

			// if they all match, wake the TASK and leave
			if ((TaskSignals->SignalFlags.WaitOnSignals.Word & TaskSignals->SignalFlags.CurrentlySetSignals.Word) == TaskSignals->SignalFlags.WaitOnSignals.Word)
			{
				UINT32 i;

				// remove all the nodes from any lists they were on
				for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
					OS_RemoveTaskFromList(&Task->TaskNodeArray[i]);

				// clear out the wait on signals to clear the resource
				TaskSignals->SignalFlags.WaitOnSignals.Word = 0;

				return OS_AddTaskToReadyQueue(Task);
			}
		}

		return FALSE;
	}

	OS_RESULT TaskWaitOnSignals(UINT16 SignalsToWaitOn

								#if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, UINT32 TicksToDelay
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
		
								)
	{
		OS_RESULT Result = OS_SUCCESS;

        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (SignalsToWaitOn == 0)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		// set the signals bits
		OS_WaitOnSignals(&gCurrentTask->TaskSignal.UserSignals, SignalsToWaitOn);

		// now go to blocked state and wait for them to be set
		OS_BlockOnSignals(gCurrentTask, PRIMARY_TASK_NODE, TRUE);

		#if (USING_TASK_DELAY_TICKS_METHOD == 1)
			if (TicksToDelay > 0)
				OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], TicksToDelay, FALSE);
		#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

		SurrenderCPU();

		#if (USING_TASK_DELAY_TICKS_METHOD == 1)
			// we must clear the wait signals since we could have been awoken by the 
			// timeout expiring and not our signals being set.
			gCurrentTask->TaskSignal.UserSignals.SignalFlags.WaitOnSignals.Word = 0;

			// check to see if we timed out before all the signals were set
			if (gCurrentTask->DelayInTicks == TASK_TIMEOUT_DONE_VALUE)
			{
				// we timed out
				Result = OS_TASK_TIMEOUT;
			}

			// clear out the tick counter before leaving
			gCurrentTask->DelayInTicks = 0;
		#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

		ExitCritical();

		return Result;
	}

	UINT16 TaskGetSetSignals(TASK *Task)
	{
        UINT16 SignalsSet;
        
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (Task != (TASK*)NULL)
            {
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                    return 0;
            }
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		if (Task == (TASK*)NULL)
			Task = gCurrentTask;

		SignalsSet = Task->TaskSignal.UserSignals.SignalFlags.CurrentlySetSignals.Word;

		ExitCritical();

		return SignalsSet;
	}

	OS_RESULT SignalTask(TASK *Task, UINT16 SignalsToSet)
	{
		OS_RESULT Result;

        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)Task) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		// if the OS is using signals, the user signals will be ignored
		if (Task->TaskSignal.OS_Signals.SignalFlags.WaitOnSignals.Word == 0)
		{
			if (Task->TaskSignal.UserSignals.SignalFlags.WaitOnSignals.Word == 0 || Task->TaskInfo.bits.State != BLOCKED)
			{
				// the TASK is not currently waiting for any signals, do not set them
				Result = OS_INVALID_TASK_STATE;
			}
			else
			{
				// proceed to set the requested signals
				if (OS_SetTaskSignals(Task, &Task->TaskSignal.UserSignals, SignalsToSet) == TRUE)
					SurrenderCPU();

				Result = OS_SUCCESS;
			}
		}
		else
		{
			// the OS was using signals
			Result = OS_RESOURCE_IN_USE;
		}

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_TASK_SIGNAL == 1)

#if (USING_TASK_GET_STATE_METHOD == 1)
	TASK_STATE TaskGetState(TASK *Task)
	{
        TASK_STATE TaskState;
                
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)Task) == FALSE)
                return NUMBER_OF_TASK_STATES;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

		EnterCritical();

		TaskState = Task->TaskInfo.bits.State;

		ExitCritical();

		return TaskState;
	}
#endif // end of #if (USING_TASK_GET_STATE_METHOD == 1)

#if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)
    void *TaskGetLocalStoragePointer(TASK *Task, UINT32 Index)
    {
        void *ReturnData;
        
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (Task != (TASK*)NULL)
            {
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                    return (void*)NULL;
            }

            if(Index >= NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS)
                return (void*)NULL;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

        EnterCritical();

        if (Task == (TASK*)NULL)
            Task = gCurrentTask;

        ReturnData = Task->TaskLocalThreadStorageArray[Index];

        ExitCritical();

        return ReturnData;
    }
    
    OS_RESULT TaskSetLocalStoragePointer(TASK *Task, UINT32 Index, void *Value)
    {
        #if (USING_CHECK_TASK_PARAMETERS == 1)
            if (Task != (TASK*)NULL)
            {
                if (RAMAddressValid((OS_WORD)Task) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            }

            if(Index >= NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_TASK_PARAMETERS == 1)

        EnterCritical();

        if (Task == (TASK*)NULL)
            Task = gCurrentTask;

        // set the value
        Task->TaskLocalThreadStorageArray[Index] = Value;

        ExitCritical();

        return OS_SUCCESS;
    }
#endif // end of #if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)
    
#if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
    UINT32 GetNumberOfTasks(void)
    {
        UINT32 NumberOfTasks;
        
        EnterCritical();
        
        NumberOfTasks = gNumberOfTasks;
        
        ExitCritical();
        
        return NumberOfTasks;
    }
#endif // end of #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
