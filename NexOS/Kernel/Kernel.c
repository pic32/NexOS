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

#include <stdlib.h>
#include <string.h>

#include "Kernel.h"
#include "CriticalSection.h"
#include "Task.h"
#include "KernelTasks.h"
#include "Port.h"
#include "Memory.h"
#include "OS_Callback.h"

#if (USING_SOFTWARE_TIMERS == 1)
	#include "../Timer/SoftwareTimer.h"
#endif // end of USING_SOFTWARE_TIMERS

#if (USING_CALLBACK_TIMERS == 1)
	#include "../Timer/Callback Timer/CallbackTimer.h"
#endif // end of USING_CALLBACK_TIMERS

#if (USING_EVENT_TIMERS == 1)
	#include "../Event/Event Timer/EventTimer.h"
#endif // end of USING_EVENT_TIMERS

#if (USING_EVENTS == 1)
	#include "../Event/Event.h"
#endif // end of USING_EVENTS

#if (RTOS_CONFIG_H_VERSION != 0x00000007)
    #error "Wrong RTOSConfig.h file version being used!"
#endif // end of #if (RTOS_CONFIG_H_VERSION != 0xXXXXXXXX)

// This is the critical count counter.  Each time a TASK calls a EnterCritical() 
// this variable is increased.  Each time a TASK called ExitCritical() this
// variable is decreased.  Once it gets back to zero, interrupts are enabled.
extern volatile OS_WORD gCurrentCriticalCount;

OS_WORD gSystemStack[SYSTEM_STACK_SIZE_IN_WORDS];
OS_WORD *gSystemStackPointer;
volatile UINT32 gOSTickCount;
BOOL gCPUSchedulerRunning;
TASK gIdleTask;						// This is the Idle TASK data structure
volatile DOUBLE_LINKED_LIST_NODE *gCurrentNode;// A pointer to the DOUBLE_LINKED_LIST_NODE of the currently executing Task.
BYTE gCurrentSystemPriority; 		// This is the priority of the currently executing Task, and is referenced to see
										 	// if there was a Task added to the Ready Queue that has a higher priority.
TASK * volatile gCurrentTask;			// Points to the currently executing Task.
DOUBLE_LINKED_LIST_HEAD gCPUScheduler[CPU_SCHEDULER_QUEUE_SIZE]; // The main guts of the Ready Queue.  This holds all the TASKs that are executing

#if (USING_TASK_DELAY_TICKS_METHOD == 1)
	DOUBLE_LINKED_LIST_HEAD gDelayQueue;
#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

#if (USING_TASK_HIBERNATION == 1 || USING_TASK_SIGNAL == 1 || USING_IO_BUFFERS == 1)
	DOUBLE_LINKED_LIST_HEAD gMiscellaneousBlockedQueueHead;
#endif // end of #if (USING_TASK_HIBERNATION == 1 || USING_TASK_SIGNAL == 1 || USING_IO_BUFFERS == 1)

#if (USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_SUSPEND_TASK_METHOD == 1)
	DOUBLE_LINKED_LIST_HEAD gSuspendedQueueHead; // This is the Head of the Suspended Queue.  It will only be used if any Suspended methods in Task.c are used.
#endif // end of USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_SUSPEND_TASK_METHOD == 1

#if (USING_TASK_CHECK_IN == 1)
	DOUBLE_LINKED_LIST_HEAD gTaskCheckInList;
#endif // end of USING_TASK_CHECK_IN

#if (USING_DELETE_TASK == 1)
	DOUBLE_LINKED_LIST_HEAD gDeleteTaskList;
#endif // end of #if (USING_DELETE_TASK == 1)

#if (USING_RESTART_TASK == 1)
	DOUBLE_LINKED_LIST_HEAD gRestartTaskList;
#endif // end of #if (USING_RESTART_TASK == 1)
    
#if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)
    DOUBLE_LINKED_LIST_HEAD gRuntimeExecutionList; // this is of type TASK_RUNTIME_INFO
    UINT32 gCurrentTaskRunTimeCounter, gKernelMiscRunTimeCounter;
#endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)
    
#if (USING_TASK_RUNTIME_HISTORY == 1)
    UINT32 gTaskRuntimeHistoryArrayCurrentIndex;
    TASK_RUNTIME_HISTORY gTaskRuntimeHistoryArray[TASK_RUNTIME_HISTORY_SIZE_IN_TASKS];
#endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)

#if ((USING_DELETE_TASK == 1 && IDLE_TASK_PERFORM_DELETE_TASK == 0) || USING_RESTART_TASK == 1)
	static TASK gMaintenanceTask;
#endif // end of USING_DELETE_TASK == 1 || USING_RESTART_TASK == 1)
    
#if (USING_IO_BUFFERS == 1)
	static TASK gIOBufferTask;
#endif // end of USING_DELETE_TASK == 1 || USING_RESTART_TASK == 1)
    
//--------------------------------------------------------------------------------------------------//
//																									//
//				The user should only ever call the below nine methods in this file.                 //
//																									//
//--------------------------------------------------------------------------------------------------//
OS_RESULT InitOS(void)
{
	// should also init CPU scheduler and all modules.
	// the only thing in start os scheduler should be 
	// finding the first task, initializing the os timer
	// and starting the first task.

	if (OS_InitializeHeap() == FALSE)
		return OS_HEAP_INITIALIZATION_FAILED;

	// initialize all needed global variables
	memset((void*)gCPUScheduler, 0, sizeof(gCPUScheduler));
    
    gSystemStackPointer = PortInitializeSystemStack(gSystemStack, sizeof(gSystemStack) / sizeof(OS_WORD));

	gOSTickCount = 0;
	gCurrentTask = &gIdleTask;
	gCurrentCriticalCount = 0;
    gCPUSchedulerRunning = FALSE;

	// initialize the delay queue
	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		InitializeDoubleLinkedListHead(&gDelayQueue);
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	// initialize the suspended queue list
	#if (USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_SUSPEND_TASK_METHOD == 1)
		InitializeDoubleLinkedListHead(&gSuspendedQueueHead);
	#endif // end of USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_SUSPEND_TASK_METHOD == 1

	// initialize the task checkin list
	#if (USING_TASK_CHECK_IN == 1)
		InitializeDoubleLinkedListHead(&gTaskCheckInList);
	#endif // end of USING_TASK_CHECK_IN

	// Initialize the delete list
	#if (USING_DELETE_TASK == 1)
		InitializeDoubleLinkedListHead(&gDeleteTaskList);
	#endif // end of USING_DELETE_TASK

	// Initialize the restart list
	#if (USING_RESTART_TASK == 1)
		InitializeDoubleLinkedListHead(&gRestartTaskList);
	#endif // end of USING_RESTART_TASK

    #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)
        InitializeDoubleLinkedListHead(&gRuntimeExecutionList);
        
        gKernelMiscRunTimeCounter = 0;
    #endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)

    #if (USING_TASK_RUNTIME_HISTORY == 1)
        gTaskRuntimeHistoryArrayCurrentIndex = 0;
        
        memset((void*)gTaskRuntimeHistoryArray, 0, sizeof(gTaskRuntimeHistoryArray));
    #endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)

	#if (USING_TASK_HIBERNATION == 1 || USING_TASK_SIGNAL == 1 || USING_IO_BUFFERS == 1)
		InitializeDoubleLinkedListHead(&gMiscellaneousBlockedQueueHead);
	#endif // end of #if (USING_TASK_HIBERNATION == 1 || USING_TASK_SIGNAL == 1 || USING_IO_BUFFERS == 1)

	// create the idle TASK
	if (CreateTask(	IdleTaskCode, 
					IDLE_TASK_STACK_SIZE_IN_BYTES,
					IDLE_TASK_PRIORITY,
					IDLE_TASK_ARGS,

					#if (USING_RESTART_TASK == 1)
						FALSE,
					#endif // end of #if (USING_RESTART_TASK == 1)

					#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
						READY,
					#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
					
					#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
						FALSE,
					#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

					#if(USING_TASK_NAMES == 1)
						IDLE_TASK_TASK_NAME,
					#endif // end of #if(USING_TASK_NAMES == 1)

					#if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
						(TASK_EXIT_CALLBACK)NULL,
					#endif // end of #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)

					&gIdleTask) == (TASK*)NULL)
		return OS_CREATE_IDLE_TASK_FAILED;

	// create the maintenance TASK
	#if ((USING_DELETE_TASK == 1 && IDLE_TASK_PERFORM_DELETE_TASK == 0) || USING_RESTART_TASK == 1)
		if (CreateTask(	MaintenanceTaskCode,
						MAINTENANCE_TASK_STACK_SIZE_IN_BYTES,
						MAINTENANCE_TASK_PRIORITY,
						MAINTENANCE_TASK_ARGS,

						#if (USING_RESTART_TASK == 1)
							FALSE,
						#endif // end of #if (USING_RESTART_TASK == 1)

						#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
							READY,
						#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
						
						#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
							FALSE,
						#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

						#if(USING_TASK_NAMES == 1)
							MAINTENANCE_TASK_TASK_NAME,
						#endif // end of #if(USING_TASK_NAMES == 1)

						#if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
							(TASK_EXIT_CALLBACK)NULL,
						#endif // end of #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)

						&gMaintenanceTask) == (TASK*)NULL)
			return OS_CREATE_MAINTENANCE_TASK_FAILED;
	#endif // end of #if ((USING_DELETE_TASK == 1 && IDLE_TASK_PERFORM_DELETE_TASK == 0) || USING_RESTART_TASK == 1)

    #if (USING_IO_BUFFERS == 1)
		if (CreateTask(	IOBufferTaskCode,
						IO_BUFFER_TASK_STACK_SIZE_IN_BYTES,
						IO_BUFFER_TASK_PRIORITY,
						IO_BUFFER_TASK_ARGS,

						#if (USING_RESTART_TASK == 1)
							FALSE,
						#endif // end of #if (USING_RESTART_TASK == 1)

						#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
							READY,
						#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1 || USING_TASK_HIBERNATION == 1)
						
						#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
							FALSE,
						#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

						#if(USING_TASK_NAMES == 1)
							IO_BUFFER_TASK_TASK_NAME,
						#endif // end of #if(USING_TASK_NAMES == 1)

						#if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
							(TASK_EXIT_CALLBACK)NULL,
						#endif // end of #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)

						&gIOBufferTask) == (TASK*)NULL)
			return OS_CREATE_IO_BUFFER_TASK_FAILED;
	#endif // end of #if (USING_IO_BUFFERS == 1)

	#if (USING_SOFTWARE_TIMERS == 1)
		if (OS_InitSoftwareTimerLib() == FALSE)
			return OS_INITIALIZE_TIMER_LIB_FAILED;
	#endif // end of #if (USING_SOFTWARE_TIMERS == 1)

	#if (USING_CALLBACK_TIMERS == 1)
		if (OS_InitCallbackTimersLib() == FALSE)
			return OS_INITIALIZE_CALLBACK_TIMER_LIB_FAILED;
	#endif // end of #if (USING_CALLBACK_TIMERS == 1)

	#if (USING_EVENTS == 1)
		if (OS_InitEventsLib() == FALSE)
			return OS_INITIALIZE_EVENTS_LIB_FAILED;
	#endif // end of USING_EVENTS

	#if (USING_EVENT_TIMERS == 1)
		if (OS_InitEventTimersLib() == FALSE)
			return OS_INITIALIZE_EVENT_TIMERS_LIB_FAILED;
	#endif // end of USING_EVENT_TIMERS

	return OS_SUCCESS;
}

void StartOSScheduler(void)
{
	BYTE i = HIGHEST_USER_TASK_PRIORITY;

	// search for the first task to run
	while (gCPUScheduler[i].Beginning == (DOUBLE_LINKED_LIST_NODE*)NULL)
		i--;

	gCurrentTask = (TASK*)gCPUScheduler[i].Beginning->Data;
	gCurrentNode = gCPUScheduler[i].Beginning;

	// set the system priority to the currently selected TASK
	gCurrentSystemPriority = gCurrentTask->TaskInfo.bits.Priority;

	gCPUSchedulerRunning = TRUE;

    // call the specific hardware configuration method for the OS tick and 
    // anything else the CPU needs done to work with the OS.
	PortStartOSScheduler();
    
    #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)
        gCurrentTaskRunTimeCounter = PortGetTaskRunTimeCounter();
    #endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)

    #if (USING_TASK_RUNTIME_HISTORY == 1)
        gTaskRuntimeHistoryArray[gTaskRuntimeHistoryArrayCurrentIndex].TaskRuntimeInfo = gCurrentTask->TaskRunTime;
    #endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)

    // now begin executing the first TASK
	OS_StartFirstTask((OS_WORD*)(gCurrentTask->TaskStackPointer));
}

#if (USING_GET_OS_TICK_COUNT_METHOD == 1)
    UINT32 GetOSTickCount(void)
    {
        UINT32 OSTickCount;

        EnterCritical();

        OSTickCount = gOSTickCount;

        ExitCritical();

        return OSTickCount;
    }
#endif // end of #if (USING_GET_OS_TICK_COUNT_METHOD == 1)

#if (USING_GET_OS_TICK_COUNT_FROM_ISR_METHOD == 1)
    UINT32 GetOSTickCountFromISR(void)
    {
        return gOSTickCount;
    }
#endif // end of #if (USING_GET_OS_TICK_COUNT_FROM_ISR_METHOD == 1)
    
#if (USING_ANALYZE_SYSTEM_STACK_METHOD == 1)
    UINT32 AnaylzeSystemStackUsage(void)
    {
        return PortAnaylzeTaskStackUsage(gSystemStack, sizeof(gSystemStack) / sizeof(OS_WORD));
    }
#endif // end of #if (USING_ANALYZE_SYSTEM_STACK_METHOD == 1)

#if (USING_ENTER_DEVICE_SLEEP_MODE_METHOD == 1)
    void DeviceEnterSleepMode(void)
    {
        // if the OS timer is running because a TASK
        // is sleeping, we need to keep it running.
        EnterCritical();
        
        // is there a TASK delaying?
        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
            if(DoubleLinkedListHasData(&gDelayQueue) == FALSE)
            {
                // there is no TASK delaying, stop the system timer
                PortStopOSTickTimer();
            }
        #else
            // there is no TASK delaying, stop the system timer
            PortStopOSTickTimer();
        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
        
        ExitCritical();
        
        #if (USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)
            EnterDeviceSleepModeUserCallback();
        #endif // end of #if (USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)
        
        // now enter sleep mode
        PortEnterSleepMode();
        
        // now turn on the OS Tick Timer regardless of if we turned it off or not
        PortStartOSTickTimer();
        
        #if (USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)
            ExitDeviceSleepModeUserCallback();
        #endif // end of #if (USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)
    }
#endif // end of #if (USING_ENTER_DEVICE_SLEEP_MODE_METHOD == 1)

#if (USING_TICKS_TO_MILLISECONDS_METHOD == 1)
    UINT32 TicksToMilliseconds(UINT32 Ticks)
    {
        FLOAT32 MillisecondsPerTick = (FLOAT32)((FLOAT32)((FLOAT32)1.0 / (FLOAT32)OS_TICK_RATE_IN_HZ) / (FLOAT32)0.001);
        
        return (UINT32)((FLOAT32)Ticks * MillisecondsPerTick);
    }
#endif // end of #if (USING_TICKS_TO_MILLISECONDS_METHOD == 1)

#if (USING_MILLISECONDS_TO_TICKS_METHOD == 1)
    UINT32 MillisecondsToTicks(UINT32 Milliseconds)
    {
        FLOAT32 MillisecondsPerTick = (FLOAT32)((FLOAT32)((FLOAT32)1.0 / (FLOAT32)OS_TICK_RATE_IN_HZ) / (FLOAT32)0.001);
        
        if(MillisecondsPerTick > (FLOAT32)Milliseconds)
            return 1;
        
        return (UINT32)((FLOAT32)Milliseconds / MillisecondsPerTick);
    }
#endif // end of #if (USING_MILLISECONDS_TO_TICKS_METHOD == 1)

#if (USING_TICKS_TO_MICROSECONDS_METHOD == 1)
    UINT32 TicksToMicroseconds(UINT32 Ticks)
    {
        FLOAT32 MicrosecondsPerTick = (FLOAT32)((FLOAT32)((FLOAT32)1.0 / (FLOAT32)OS_TICK_RATE_IN_HZ) / (FLOAT32)0.000001);
        
        return (UINT32)((FLOAT32)Ticks * MicrosecondsPerTick);
    }
#endif // end of #if (USING_TICKS_TO_MICROSECONDS_METHOD == 1)

#if (USING_MICROSECONDS_TO_TICKS_METHOD == 1)  
    UINT32 MicrosecondsToTicks(UINT32 Microseconds)
    {
        FLOAT32 MicrosecondsPerTick = (FLOAT32)((FLOAT32)((FLOAT32)1.0 / (FLOAT32)OS_TICK_RATE_IN_HZ) / (FLOAT32)0.000001);
        
        if(MicrosecondsPerTick > (FLOAT32)Microseconds)
            return 1;
        
        return (UINT32)((FLOAT32)Microseconds / MicrosecondsPerTick);
    }
#endif // end of #if (USING_MICROSECONDS_TO_TICKS_METHOD == 1)
    
#if (USING_KERNEL_VERSION_TO_STRING == 1)
    static BYTE *KernelVersionINT32ToString(BYTE *ToStringBuffer, UINT32 Value, BOOL LeadingZero)
    {
        if(Value >= 10)
        {
            *ToStringBuffer++ = (Value / 10) + '0';
            
            Value %= 10;
        }
        else
        {
            if(LeadingZero == TRUE)
                *ToStringBuffer++ = '0';
        }
        
        *ToStringBuffer++ = Value + '0';
        
        return ToStringBuffer;
    }
    
    BYTE *KernelVersionToString(BYTE *ToStringBuffer)
    {
        BYTE *ToStringBufferPointer;
        
        if(RAMAddressValid((OS_WORD)ToStringBuffer) == FALSE)
            return (BYTE*)NULL;
        
        // point to their buffer so we can iterate beyond it
        ToStringBufferPointer = ToStringBuffer;
        
        // the version should be something like 
        // major.minor.test version
        
        // major version
        ToStringBufferPointer = KernelVersionINT32ToString(ToStringBufferPointer, KERNEL_MAJOR_VERSION_NUMBER, FALSE);
        
        *ToStringBufferPointer++ = '.';
        
        // minor version
        ToStringBufferPointer = KernelVersionINT32ToString(ToStringBufferPointer, KERNEL_MINOR_VERSION_NUMBER, TRUE);
        
        // test version
        #if (KERNEL_TEST_VERSION_NUMBER > 0)
            *ToStringBufferPointer++ = '.';

            ToStringBufferPointer = KernelVersionINT32ToString(ToStringBufferPointer, KERNEL_TEST_VERSION_NUMBER, TRUE);
        #endif // end of #if (KERNEL_TEST_VERSION_NUMBER > 0)
            
        // now terminate the string
        ToStringBufferPointer = 0;
            
        return ToStringBuffer;
    }
#endif // end of #if (USING_KERNEL_VERSION_TO_STRING == 1)

#if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1 && USING_TASK_RUNTIME_EXECUTION_TO_STRING_METHOD == 1)
    #include <stdio.h>
    
    void TaskRuntimeExecutionListToString(BYTE *ToStringBuffer, BOOL Percentage)
    {
        FLOAT32 OverallPercentage, IndividualTaskPercent;
        TASK_RUNTIME_INFO *TaskRunTimeInfo;
        DOUBLE_LINKED_LIST_NODE *Node = gRuntimeExecutionList.Beginning;
        
        #if (USING_TASK_NAMES != 1 && USING_TASK_UNIQUE_ID != 1)
            UINT32 i = 0;
        #endif // #if (USING_TASK_NAMES != 1 && USING_TASK_UNIQUE_ID != 1)
        
        EnterCritical();
        
        if(Percentage == TRUE)
        {
            OverallPercentage = 0.0;
            
            while(Node != (DOUBLE_LINKED_LIST_NODE*)NULL)
            {
                TaskRunTimeInfo = (TASK_RUNTIME_INFO*)Node->Data;
                
                OverallPercentage += TaskRunTimeInfo->TaskRunTime;
                
                // iterate to the next TASK in the list        
                Node = Node->NextNode;
            }
            
            Node = gRuntimeExecutionList.Beginning;
            
            // now just do this once
            OverallPercentage += gKernelMiscRunTimeCounter;
                   
            // now just do this once too
            OverallPercentage = PortGetExecutionTimeInSeconds(OverallPercentage);
        }
        
        while(Node != (DOUBLE_LINKED_LIST_NODE*)NULL)
        {
            TaskRunTimeInfo = (TASK_RUNTIME_INFO*)Node->Data;

            if(Percentage == TRUE)
            {
                IndividualTaskPercent = PortGetExecutionTimeInSeconds(TaskRunTimeInfo->TaskRunTime) / OverallPercentage * (FLOAT32)100.00;
                
                #if (USING_TASK_NAMES == 1)
                    sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "%s: % 5.2f%%\r\n", TaskRunTimeInfo->TaskName, IndividualTaskPercent);
                #else
                    #if (USING_TASK_UNIQUE_ID == 1)
                        sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "TASK ID %l: %8.6fs\r\n", TaskRunTimeInfo->UniqueID, Percent);
                    #else
                        sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "TASK %i: %8.6fs\r\n", (int)i++, Percent);
                    #endif // end of #if (USING_TASK_UNIQUE_ID == 1)
                #endif // end of #if (USING_TASK_NAMES == 1)
            }
            else
            {
                #if (USING_TASK_NAMES == 1)
                    sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "%s: %8.6fs\r\n", TaskRunTimeInfo->TaskName, PortGetExecutionTimeInSeconds(TaskRunTimeInfo->TaskRunTime));
                #else
                    #if (USING_TASK_UNIQUE_ID == 1)
                        sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "TASK ID %l: %8.6fs\r\n", TaskRunTimeInfo->UniqueID, PortGetExecutionTimeInSeconds(TaskRunTimeInfo->TaskRunTime));
                    #else
                        sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "TASK %i: %8.6fs\r\n", (int)i++, PortGetExecutionTimeInSeconds(TaskRunTimeInfo->TaskRunTime));
                    #endif // end of #if (USING_TASK_UNIQUE_ID == 1)
                #endif // end of #if (USING_TASK_NAMES == 1)
            }

            // iterate to the next TASK in the list        
            Node = Node->NextNode;
        }

        if(Percentage == TRUE)
        {
            IndividualTaskPercent = PortGetExecutionTimeInSeconds(gKernelMiscRunTimeCounter) / OverallPercentage * (FLOAT32)100.00;
            
            sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "MISC KERNEL: % 5.2f%%\r\n", IndividualTaskPercent);
        }
        else
        {
            sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "MISC KERNEL: %8.6fs\r\n", PortGetExecutionTimeInSeconds(gKernelMiscRunTimeCounter));
        }
            
        ExitCritical();
    }
#endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1 && USING_TASK_RUNTIME_EXECUTION_TO_STRING_METHOD == 1)
    
#if (USING_TASK_RUNTIME_HISTORY == 1)
    #include <stdio.h>
    
    void TaskRuntimeHistoryListToString(BYTE *ToStringBuffer, BOOL PrintRuntime)
    {
        TASK_RUNTIME_HISTORY *TaskRunTimeHistory;
        UINT32 i, j = 0;
        
        #if (USING_TASK_NAMES != 1 && USING_TASK_UNIQUE_ID != 1)
            UINT32 j = 0;
        #endif // end of #if (USING_TASK_NAMES != 1 && USING_TASK_UNIQUE_ID != 1)
        
        // get the most recent TASK
        i = gTaskRuntimeHistoryArrayCurrentIndex;
        
        if(i == 0)
            i = TASK_RUNTIME_HISTORY_SIZE_IN_TASKS - 1;
        else
            i--;
        
        EnterCritical();
        
        while(j != TASK_RUNTIME_HISTORY_SIZE_IN_TASKS)
        {
            TaskRunTimeHistory = &gTaskRuntimeHistoryArray[i];

            if(TaskRunTimeHistory->TaskRuntimeInfo != (TASK_RUNTIME_INFO*)NULL)
            {
                #if (USING_TASK_NAMES == 1)
                    sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "%s", TaskRunTimeHistory->TaskRuntimeInfo->TaskName);
                #else
                    #if (USING_TASK_UNIQUE_ID == 1)
                        sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "TASK ID %l", TaskRunTimeHistory->TaskRuntimeInfo->UniqueID);
                    #else
                        sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], "TASK %i", (int)j++);
                    #endif // end of #if (USING_TASK_UNIQUE_ID == 1)
                #endif // end of #if (USING_TASK_NAMES == 1)

                if(PrintRuntime == TRUE)
                    sprintf((char*)&ToStringBuffer[strlen((const char*)ToStringBuffer)], ": %8.6fs", PortGetExecutionTimeInSeconds(TaskRunTimeHistory->PreviousExecutionTime));

                strcat((char*)ToStringBuffer, "\r\n");
            }
            
            if(i == 0)
                i = TASK_RUNTIME_HISTORY_SIZE_IN_TASKS - 1;
            else
                i--;
            
            j++;
        }
        
        ExitCritical();
    }
#endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)
    
//--------------------------------------------------------------------------------------------------//
//																									//
//									End user callable section										//
//																									//
//--------------------------------------------------------------------------------------------------//











//--------------------------------------------------------------------------------------------------//
//																									//
//						The user should NEVER call any of the below methods							//
//																									//
//--------------------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// OS CPU Scheduler manipulation related functions beginning
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
	This method will add the specified Task to the Ready Queue and makes sure all the pointers to the next and previous nodes
	of all the involved Tasks are pointing to the correct nodes.
*/
static void OS_AddTaskToCPUScheduler(TASK *TaskToAdd)
{
	TaskToAdd->TaskInfo.bits.State = READY;

	if (gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].End == (DOUBLE_LINKED_LIST_NODE*)NULL)
	{
		// there are no other tasks at this priority
		gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].End = gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].Beginning = &(TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode);

		TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.NextNode = TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.PreviousNode = &(TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode);
	}
	else
	{
		// Add it to the end of the list at the specified priority
		gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].End->NextNode = &(TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode);

		TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.PreviousNode = gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].End;

		gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].End = &(TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode);

		TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.NextNode = gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].Beginning;

		gCPUScheduler[TaskToAdd->TaskInfo.bits.Priority].Beginning->PreviousNode = &(TaskToAdd->TaskNodeArray[PRIMARY_TASK_NODE].ListNode);
	}
}

/*
	This method will remove the specified Task from the Ready Queue.
*/
static void OS_RemoveTaskFromCPUScheduler(TASK *TaskToRemove)
{	
	// The next item in the list has its previous pointer pointing to the same item that TaskToRemove->SystemQueueTaskNode.PreviousItem is pointing to
	TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.NextNode->PreviousNode = TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.PreviousNode;

	// The previous item in the list has its next pointer to the one after the item being removed.
	TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.PreviousNode->NextNode = TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.NextNode;

	// If the item were removing is the beginning of the list its being removed from
	if (&(TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode) == gCPUScheduler[TaskToRemove->TaskInfo.bits.Priority].Beginning)
	{
		// If TaskToRemove is the beginning of the list but not the end of the list, then there is another Task that has the same priority
		if (&(TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode) != gCPUScheduler[TaskToRemove->TaskInfo.bits.Priority].End)
		{
			// Make the beginning pointer point to the next item since its confirmed to be in the same list
			gCPUScheduler[TaskToRemove->TaskInfo.bits.Priority].Beginning = TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.NextNode;
		}
		else
		{
			//if were the beginning and end of the list of the list, then were the only
			// node in the list.  therefore once were gone the beginning and end pointers of this list must be NULL.
			gCPUScheduler[TaskToRemove->TaskInfo.bits.Priority].Beginning = gCPUScheduler[TaskToRemove->TaskInfo.bits.Priority].End = NULL;
		}
	}
	else
	{
		// if were not the beginning check to see if were the end.
		if (&(TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode) == gCPUScheduler[TaskToRemove->TaskInfo.bits.Priority].End)
		{
			// sice we know were not the beginning but we are the end, our previous item must be of the same priority
			// and in the the same list.  so make the end pointer point to it.
			gCPUScheduler[TaskToRemove->TaskInfo.bits.Priority].End = TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.PreviousNode;
		}
	}

	// now clear out the queue node data pointers
	TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.NextNode = TaskToRemove->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// OS CPU Scheduler manipulation related functions end
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// OS Timer Tick related functions beginning
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#if (USING_TASK_CHECK_IN == 1)
	#if (USING_RESTART_TASK != 1)
		#error "USING_RESTART_TASK in RTOSConfig.h must be defined as 1 if USING_TASK_CHECK_IN is defined as 1!"
	#endif // end of USING_RESTART_TASK

	#if (USING_DELETE_TASK != 1)
		#error "USING_DELETE_TASK in RTOSConfig.h must be defined as 1 if USING_TASK_CHECK_IN is defined as 1!"
	#endif // end of USING_RESTART_TASK
	
	static void OS_UpdateTaskCheckIn(void)
	{
        TASK *Task;
		DOUBLE_LINKED_LIST_NODE *Node = gTaskCheckInList.Beginning;
		
		// Iterate through the task list and see if everyones checking in
		while(Node != (DOUBLE_LINKED_LIST_NODE*)NULL)
		{
			Task = (TASK*)(Node->Data);
	
			Task->TaskCheckIn->CurrentCount++;
	
			Node = (DOUBLE_LINKED_LIST_NODE*)(Node->NextNode);

			if(Task->TaskCheckIn->CurrentCount == Task->TaskCheckIn->Timeout)
			{
				Task->TaskCheckIn->CurrentCount = 0;

				// restart or delete the Task
				#if (USING_TASK_CHECK_IN_USER_CALLBACK == 1)
					if(TaskCheckInUserCallback(Task) == TRUE)
                #endif // end of #if (USING_TASK_CHECK_IN_USER_CALLBACK == 1)
					{
						if (Task->RestartInfo == (TASK_RESTART_INFO*)NULL)
						{
							OS_DeleteTask(Task);
						}
						else
						{
							OS_RestartTask(Task);
						}
					}
			}
		}
	}
#endif // end of USING_TASK_CHECK_IN

#if (USING_TASK_DELAY_TICKS_METHOD == 1)
	static void OS_UpdateTaskDelayList(void)
	{
		DOUBLE_LINKED_LIST_NODE *TempCurrentNode, *TempNextNode;
		TASK *TempTask;
		UINT32 i;

		// now update the timer list
		// we just have to iterate through and see if anybody is done yet
		TempCurrentNode = gDelayQueue.Beginning;

		while (TempCurrentNode != (DOUBLE_LINKED_LIST_NODE*)NULL)
		{
			TempNextNode = TempCurrentNode->NextNode;

			TempTask = (TASK*)TempCurrentNode->Data;

			TempTask->DelayInTicks--;

			if (TempTask->DelayInTicks == 0)
			{
				// remove all the nodes from any lists they were on
				for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
					OS_RemoveTaskFromList(&TempTask->TaskNodeArray[i]);

				// now set the indicator which means the timeout was done.
				TempTask->DelayInTicks = TASK_TIMEOUT_DONE_VALUE;

				// now add it to the READY_QUEUE
				OS_AddTaskToReadyQueue(TempTask);
			}

			TempCurrentNode = TempNextNode;
		}
	}
#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

static void UpdateOSTick(UINT32 CurrentOSTickCount)
{
	// we have to update OS tick
	PortClearOSTickTimerInterruptFlag();
	
	// allow the hardware to update any timer related values for the next tick.
	PortUpdateOSTimer();

	#if (USING_OS_TICK_UPDATE_USER_CALLBACK == 1)
		OSTickUpdateUserCallback(CurrentOSTickCount);
	#endif // end of USING_OS_TICK_UPDATE_USER_CALLBACK

	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		OS_UpdateTaskDelayList();
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	#if(USING_SOFTWARE_TIMERS == 1)
		OS_UpdateSoftwareTimers();
	#endif // end of USING_SOFTWARE_TIMERS

	#if(USING_CALLBACK_TIMERS == 1)
		OS_UpdateCallbackTimers(CurrentOSTickCount);
	#endif // end of USING_CALLBACK_TIMERS

	#if (USING_TASK_CHECK_IN == 1)
		OS_UpdateTaskCheckIn();
	#endif // end of USING_TASK_CHECK_IN
}

OS_WORD *OS_NextTask(OS_WORD *CurrentTaskStackPointer)
{
    #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)
        TASK_RUNTIME_INFO *TaskRunTimeInfo;
        UINT32 TempTaskRuntimeCounter = PortGetTaskRunTimeCounter();
        
        TaskRunTimeInfo = (TASK_RUNTIME_INFO*)gCurrentTask->TaskRunTime;
        
        // did we roll over?
        if(TempTaskRuntimeCounter < gCurrentTaskRunTimeCounter)
        {
            TaskRunTimeInfo->TaskRunTime += TempTaskRuntimeCounter + (0xFFFFFFFF - gCurrentTaskRunTimeCounter);
            
            #if (USING_TASK_RUNTIME_HISTORY == 1)
                gTaskRuntimeHistoryArray[gTaskRuntimeHistoryArrayCurrentIndex++].PreviousExecutionTime = TempTaskRuntimeCounter + (0xFFFFFFFF - gCurrentTaskRunTimeCounter);
            #endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)
        }
        else
        {
            TaskRunTimeInfo->TaskRunTime += TempTaskRuntimeCounter - gCurrentTaskRunTimeCounter;
            
            #if (USING_TASK_RUNTIME_HISTORY == 1)
                gTaskRuntimeHistoryArray[gTaskRuntimeHistoryArrayCurrentIndex++].PreviousExecutionTime = TempTaskRuntimeCounter - gCurrentTaskRunTimeCounter;
            #endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)
        }
    #endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)

	// clear the core interrupt flag regardless of if it is set or not
	PortClearCoreInterruptFlag();
    
    #if (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)
        if(PortIsStackOverflowed(CurrentTaskStackPointer, gCurrentTask->StartOfTaskStackPointer, gCurrentTask->StartingTaskStackSizeInWords) == TRUE)
            TaskStackOverflowUserCallback(gCurrentTask);
    #endif // end of #if (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)

	// Save the Stack Pointer of the Task whose Context we saved in ContextSwitch().
	gCurrentTask->TaskStackPointer = CurrentTaskStackPointer;

	gCurrentTask->CriticalCount = gCurrentCriticalCount;

	#if (USING_CONTEXT_SWITCH_USER_CALLBACK == 1)
		ContextSwitchUserCallback();
	#endif // end of USING_CONTEXT_SWITCH_USER_CALLBACK

	if (PortGetOSTickTimerInterruptFlag())
	{
		// we have to update OS tick
		gOSTickCount++;

		// now call any timer related functions
		UpdateOSTick(gOSTickCount);
	}

	// Iterate CurrentNode to the Next Node in the Ready Queue.
	gCurrentNode = (DOUBLE_LINKED_LIST_NODE*)gCurrentNode->NextNode;

	// Update CurrentTask to point to the new Task that will be Executed.
	gCurrentTask = (TASK*)gCurrentNode->Data;

	gCurrentCriticalCount = gCurrentTask->CriticalCount;
    
    #if (USING_TASK_RUNTIME_HISTORY == 1)
        if(gTaskRuntimeHistoryArrayCurrentIndex == TASK_RUNTIME_HISTORY_SIZE_IN_TASKS)
            gTaskRuntimeHistoryArrayCurrentIndex = 0;
    
        gTaskRuntimeHistoryArray[gTaskRuntimeHistoryArrayCurrentIndex].TaskRuntimeInfo = gCurrentTask->TaskRunTime;
    #endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)

    #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)
        gCurrentTaskRunTimeCounter = PortGetTaskRunTimeCounter();
        
        // now get the misc kernel time
        if(gCurrentTaskRunTimeCounter < TempTaskRuntimeCounter)
        {
            gKernelMiscRunTimeCounter += gCurrentTaskRunTimeCounter + (0xFFFFFFFF - TempTaskRuntimeCounter);
        }
        else
        {
            gKernelMiscRunTimeCounter += gCurrentTaskRunTimeCounter - TempTaskRuntimeCounter;
        }
    #endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)

	// Return the Stack Pointer of the Task whose Context will be restored by ContextSwitch().
	return gCurrentTask->TaskStackPointer;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// OS Timer Tick related functions end
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#if (USING_OS_GENERAL_EXCEPTION_HANDLER == 1)
    #if (USING_OS_GENERAL_EXCEPTION_TO_STRING == 1)
        const char* ExceptionsErrorStrings[] = {
           "INTERRUPT ERROR\0",
           "ADDRESS ERROR I\0",
           "ADDRESS ERROR LS\0",
           "BUS ERROR I\0",
           "BUS ERROR LS\0",
           "SYSCALL ERROR\0",
           "BREAKPOINT ERROR\0",
           "RESERVE INST ERROR\0",
           "COCPU UNUSABLE\0",
           "OVERFLOW ERROR\0",
           "TRAP ERROR\0",
           "IMP SPECIFIC ERROR\0",
           "COREXT UNUSABLE\0",
           "COCPU 2 UNUSABLE\0"
        };

        const BYTE *ExceptionToString(OS_EXCEPTION_CODE EPCCode)
        {
            switch(EPCCode)
            {
                case INTERRUPT_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[0];
                    
                case ADDRESS_LOAD_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[1];

                case ADDRESS_STORE_EXCEPTION_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[2];

                case INSTRUCTION_BUS_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[3];

                case DEBUG_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[4];

                case SYSCALL_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[5];

                case BREAKPOINT_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[6];

                case RESERVED_INSTRUCTION_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[7];

                case CPU_UNUSABLE_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[8];

                case OVERFLOW_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[9];

                case TRAP_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[10];

                case IMPLEMENTATION_SPECIFIC_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[11];

                case COR_EXTEND_UNUSABLE_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[12];

                case COPROCESSOR_2_EXCEPTION:
                    return (BYTE*)ExceptionsErrorStrings[13];

                default:
                    return (BYTE*)NULL;
            }
        }
    #endif // end of #if (USING_OS_GENERAL_EXCEPTION_TO_STRING == 1)
    
    static BOOL OS_KernalTaskRunning(void)
	{
		if (gCurrentTask == &gIdleTask)
			return TRUE;

		#if(USING_RESTART_TASK == 1 || USING_DELETE_TASK == 1)
			if(gCurrentTask == &gMaintenanceTask)
				return TRUE;
		#endif // end of USING_RESTART_TASK and USING_DELETE_TASK

		return FALSE;
	}

	OS_WORD *GeneralExceptionHandler(OS_EXCEPTION_CODE ExceptionCode, UINT32 ExceptionAddress)
	{
        BOOL UserCodeCurrentlyExecuting = (BOOL)(gCurrentCriticalCount == 0 && OS_KernalTaskRunning() == FALSE);
        
		#if (USING_CPU_EXCEPTION_RAISED_USER_CALLBACK == 1)
            BOOL RestartCurrentTask;
            
			RestartCurrentTask = GeneralExceptionHandlerUserCallback(gCurrentTask, ExceptionCode, ExceptionAddress, UserCodeCurrentlyExecuting);
		#endif // end of #if (USING_CPU_EXCEPTION_RAISED_USER_CALLBACK == 1)

		// Check to see if we're in the OS or not, if so we can't guarantee anything about anything
		// when it comes to calling an OS method.
		// If a Task was running then we do have the power to restart it or delete it and then
		// "gracefully" recover from this error.
		if(UserCodeCurrentlyExecuting == TRUE)
		{
			// We can only raise this exception if we can continue along gracefully...
			#if (USING_CPU_EXCEPTION_RAISED_EVENT == 1)
				OS_RaiseEvent(CPU_EXCEPTION_RAISED_EVENT);
			#endif // end of #if (USING_CPU_EXCEPTION_RAISED_EVENT == 1)

			// Let the user decide if we must delete or restart the current task.
			// To the OS it doesn't care which choice is made, either one is valid.
			#if (USING_CPU_EXCEPTION_RAISED_USER_CALLBACK == 1)
                #if (USING_DELETE_TASK == 1 && USING_RESTART_TASK == 1) 
                    if (RestartCurrentTask == TRUE && gCurrentTask->RestartInfo != (TASK_RESTART_INFO*)NULL)
                        OS_RestartTask(gCurrentTask);
                    else
                        OS_DeleteTask(gCurrentTask);
                #else
                    #if (USING_DELETE_TASK == 1)
                        if(RestartCurrentTask == TRUE)
                            OS_DeleteTask(gCurrentTask);
                        else
                            return (OS_WORD*)NULL;
                    #else
                        #if(USING_RESTART_TASK == 1)
                            if(RestartCurrentTask == TRUE && gCurrentTask->RestartInfo != (TASK_RESTART_INFO*)NULL))
                                OS_RestartTask(gCurrentTask);
                            else 
                                return (OS_WORD*)NULL;
                        #endif // end of #if(USING_RESTART_TASK == 1)
                    #endif // end of #if (USING_DELETE_TASK == 1)
                #endif // end of #if (USING_DELETE_TASK == 1 && USING_RESTART_TASK == 1) 
			#endif // end of #if (USING_CPU_EXCEPTION_RAISED_USER_CALLBACK == 1)
	
			// we need to swap the TASK because the current one
			// generated the exception event and cannot run
			return OS_NextTask((OS_WORD*)NULL);
		}

		// put up a BSOD, we cannot recover from this
	
		return (OS_WORD*)NULL;
	}
#endif // end of USING_OS_GENERAL_EXCEPTION_HANDLER

OS_WORD *OS_InitializeTaskStack(TASK *Task, TASK_ENTRY_POINT StartingAddress, void *Args, UINT32 StackSizeInWords)
{
	OS_WORD *Stack = (OS_WORD*)AllocateMemory(StackSizeInWords * OS_WORD_SIZE_IN_BYTES);

	if(Stack == (OS_WORD*)NULL)
		return (OS_WORD*)NULL;

	#if (USING_RESTART_TASK == 1 || USING_DELETE_TASK == 1) || (ANALYZE_TASK_STACK_USAGE == 1) || (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)
		Task->StartOfTaskStackPointer = Stack;
	#endif // end of #if (USING_RESTART_TASK == 1 || USING_DELETE_TASK == 1) || (ANALYZE_TASK_STACK_USAGE == 1) || (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)

    #if (ANALYZE_TASK_STACK_USAGE == 1) || (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)
        Task->StartingTaskStackSizeInWords = StackSizeInWords;
    #endif // end of #if (ANALYZE_TASK_STACK_USAGE == 1) || (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)

	return PortInitializeTaskStack(Stack, StackSizeInWords, StartingAddress, Args);
}

#if (USING_BINARY_SEMAPHORE_STARVATION_PROTECTION == 1)
	void OS_IncrementTaskListPriorities(DOUBLE_LINKED_LIST_HEAD *ListHead)
	{
		TASK *TaskHande1;
		DOUBLE_LINKED_LIST_NODE *PriorityIterator = ListHead->Beginning;

		while (PriorityIterator != (DOUBLE_LINKED_LIST_NODE*)NULL)
		{
			TaskHande1 = (TASK*)(PriorityIterator->Data);

			if (TaskHande1->TaskInfo.bits.Priority != OS_HIGHEST_TASK_PRIORITY)
				TaskHande1->TaskInfo.bits.Priority++;

			PriorityIterator = (DOUBLE_LINKED_LIST_NODE*)(PriorityIterator->NextNode);
		}
	}
#endif // end of #if (USING_SEMAPHORE_STARVATION_PROTECTION == 1)

#if (USING_BINARY_SEMAPHORES == 1)
	/*
		This method primarily designed for use by ReleaseBinarySemaphore is used to get the next Task
		in a Double Linked List that has the highest Priority.  It can however be used on any
		Double Linked List, not just BinarySemaphores Blocked List.  One minor quirk of this is that
		the calling method must determine if the Double Linked List is empty before calling
		this method.
	*/
	DOUBLE_LINKED_LIST_NODE *OS_PriorityRemoveDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head)
	{
		TASK *TaskIterator1, *TaskIterator2;
		DOUBLE_LINKED_LIST_NODE *Iterator1, *Iterator2;
		UINT32 i;

		// If there's only one item in the list just remove it
		if (Head->Beginning == Head->End)
		{
			DOUBLE_LINKED_LIST_NODE *TempNode = Head->Beginning;
			TASK *TaskHandle = TempNode->Data;

			// remove all the nodes from any lists they were on
			for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
				OS_RemoveTaskFromList(&TaskHandle->TaskNodeArray[i]);

			return &TaskHandle->TaskNodeArray[PRIMARY_TASK_NODE].ListNode;
		}

		// There is more than one Task in the Double Linked List if we're here.

		/*
			Iterator1 and Task1 will be set the to Task with the highest Priority.  Iterator2 and Task2 will
			iterate through the List and comapre their values to Task1, and if Task2 has a higher priority,
			Task1 will be set to Task2.  This process will repeat until Task2 and Iterator2 equal NULL.
		*/

		// Iterator1 will be set to the first Item in the list, and Iterator2 will be set the 2nd Item in the List.
		Iterator1 = Head->Beginning;
		Iterator2 = (DOUBLE_LINKED_LIST_NODE*)Head->Beginning->NextNode;

		// Tasks assigned to their respective spots.
		TaskIterator1 = (TASK*)Iterator1->Data;
		TaskIterator2 = (TASK*)Iterator2->Data;

		/*
		This loop will iterate through the entire List and determine the Task in the List with the highest Priority.
		*/
		do
		{
			// If Task2 has a higher Priority than Task1, Task1 equals Task2, and Iterator1 equals Iterator2.
			if (TaskIterator2->TaskInfo.bits.Priority > TaskIterator1->TaskInfo.bits.Priority)
			{
				Iterator1 = Iterator2;
				TaskIterator1 = TaskIterator2;
			}

			Iterator2 = (DOUBLE_LINKED_LIST_NODE*)Iterator2->NextNode;

			// Increment Iterator2 to the next Item in the List.
			if (Iterator2 != (DOUBLE_LINKED_LIST_NODE*)NULL)
			{
				TaskIterator2 = (TASK*)Iterator2->Data;
			}
		} 
		while (Iterator2 != (DOUBLE_LINKED_LIST_NODE*)NULL);

		// remove all the nodes from any lists they were on
		for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
			OS_RemoveTaskFromList(&TaskIterator1->TaskNodeArray[i]);

		// Remove and return the Task with the highest Priority from the List specified by the DOUBLE_LINKED_LIST_HEAD *Head parameter.
		return &TaskIterator1->TaskNodeArray[PRIMARY_TASK_NODE].ListNode;
	}
#endif // end of #if (USING_BINARY_SEMAPHORES == 1)

#if (USING_CHANGE_TASK_PRIORITY_METHOD == 1 || USING_MUTEXES == 1 || USING_BINARY_SEMAPHORE_STARVATION_PROTECTION == 1 || USING_PIPE_STARVATION_PROTECTION == 1)
	BOOL OS_ChangeTaskPriority(TASK *Task, BYTE Priority)
	{
		BOOL CallNextTask = FALSE;

		// if the TASK isn't ready, just change the priority
		if (Task->TaskInfo.bits.State != READY)
		{
			// just change the priority to the current task
			Task->TaskInfo.bits.Priority = Priority;
		}
		else
		{
			// the TASK is in the ready queue, we must remove it
			// prior to changing the priority.

			if (OS_RemoveTaskFromReadyQueue(Task) == TRUE)
				CallNextTask = TRUE;

			Task->TaskInfo.bits.Priority = Priority;

			if (OS_AddTaskToReadyQueue(Task) == TRUE)
				CallNextTask = TRUE;
		}

		return CallNextTask;
	}
#endif // end of #if (USING_CHANGE_TASK_PRIORITY_METHOD == 1 || USING_MUTEXES == 1 || USING_BINARY_SEMAPHORE_STARVATION_PROTECTION == 1 || USING_PIPE_STARVATION_PROTECTION == 1)

#if (USING_DELETE_TASK == 1 || USING_RESTART_TASK == 1)
	BOOL OS_PrepTaskForRemoval(TASK *Task)
	{
		if (Task->TaskInfo.bits.State != RESTARTING && Task->TaskInfo.bits.State != DELETING)
		{
			#if (USING_TASK_CHECK_IN == 1)
				if (Task->TaskCheckIn != (TASK_CHECK_IN*)NULL)
					OS_RemoveTaskFromList(&(Task->TaskCheckIn->TaskCheckInNode));
			#endif // end of USING_TASK_CHECK_IN

			// Get it out of the ready queue so we can add it to the restart list
			if (Task->TaskInfo.bits.State == READY)
			{
				OS_RemoveTaskFromReadyQueue(Task);
			}
			else
			{
				UINT32 i;

				// remove all the nodes from any lists they were on
				for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
					OS_RemoveTaskFromList(&Task->TaskNodeArray[i]);
			}

			return TRUE;
		}

		return FALSE;
	}
#endif // end of USING_DELETE_TASK and USING_RESTART_TASK

#if (USING_DELETE_TASK == 1)
	BOOL OS_DeleteTask(TASK *Task)
	{
		if (OS_PrepTaskForRemoval(Task) == TRUE)
		{
            #if(USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_PIPE_DELETE_FROM_ISR_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
                if(Task->DeleteBlockCounter != (UINT32*)NULL)
                {
                    // decrement whatever resource the TASK was waiting on
                    (*Task->DeleteBlockCounter)--;
                    
                    // make it NULL for completeness, nothing should be using this
                    // pointer after this area, but it makes things more tidy.
                    Task->DeleteBlockCounter = (UINT32*)NULL;
                }
            #endif // end of #if(USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_PIPE_DELETE_FROM_ISR_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
            
			OS_PlaceTaskOnBlockedList(Task, &gDeleteTaskList, &Task->TaskNodeArray[PRIMARY_TASK_NODE], DELETING, FALSE);

			return TRUE;
		}

		return FALSE;
	}
#endif // end of #if (USING_DELETE_TASK == 1)

#if (USING_RESTART_TASK == 1)
	BOOL OS_RestartTask(TASK *Task)
	{
		if (OS_PrepTaskForRemoval(Task) == TRUE)
		{
			OS_PlaceTaskOnBlockedList(Task, &gRestartTaskList, &Task->TaskNodeArray[PRIMARY_TASK_NODE], RESTARTING, FALSE);

			return TRUE;
		}

		return FALSE;
	}
#endif // end of USING_RESTART_TASK

#if (USING_TASK_DELAY_TICKS_METHOD == 1)
	void OS_AddTaskToDelayQueue(TASK *Task, TASK_NODE *Node, INT32 TicksToDelay, BOOL RemoveTaskFromReadyQueue)
	{
		Task->DelayInTicks = TicksToDelay;

		OS_PlaceTaskOnBlockedList(Task, &gDelayQueue, Node, BLOCKED, RemoveTaskFromReadyQueue);
	}
#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
    
#if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)
    BOOL OS_AddTaskToRuntimeExecutionList(TASK *Task)
    {
        TASK_RUNTIME_INFO *TaskRuntimeInfo = OS_AllocateMemory(sizeof(TASK_RUNTIME_INFO));
        
        if(TaskRuntimeInfo == (TASK_RUNTIME_INFO*)NULL)
            return FALSE;
        
        #if (USING_TASK_NAMES == 1)
            strcpy((char*)TaskRuntimeInfo->TaskName, (char*)Task->TaskName);
        #endif // end of #if (USING_TASK_NAMES == 1)

        #if (USING_TASK_UNIQUE_ID == 1)
            TaskRuntimeInfo->TaskUniqueID = Task->UniqueID;
        #endif // end of #if (USING_TASK_UNIQUE_ID == 1)

        // now add it to the list
        InsertNodeAtEndOfDoubleLinkedList(&gRuntimeExecutionList, &TaskRuntimeInfo->Node);
        
        TaskRuntimeInfo->Node.Data = (void*)TaskRuntimeInfo;
        
        TaskRuntimeInfo->TaskRunTime = 0;
        
        Task->TaskRunTime = (void*)TaskRuntimeInfo;
        
        return TRUE;
    }
#endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER == 1)

/*
	This method is only created to serve the purpose of place ONE Task into the Ready Queue.  This method CANNOT be called
	multiple times in a row to add multiple Tasks.
*/
BOOL OS_AddTaskToReadyQueue(TASK *TaskToAddToReadyQueue)
{	
	OS_AddTaskToCPUScheduler(TaskToAddToReadyQueue);

	if(TaskToAddToReadyQueue->TaskInfo.bits.Priority > gCurrentSystemPriority)
	{
		gCurrentSystemPriority = TaskToAddToReadyQueue->TaskInfo.bits.Priority;

		gCurrentNode = TaskToAddToReadyQueue->TaskNodeArray[PRIMARY_TASK_NODE].ListNode.PreviousNode;

		return gCPUSchedulerRunning;
	}

	return FALSE;
}

/*
	This method performs all the necessary actions needed to fully remove a Task from the Ready Queue.  This is the only method needed to
	remove a Task from the Ready Queue.  It first decrements the Number of Ready Tasks, then removes the specified Task from the Pending Queue
	if it is on there.  Then it Removes and returns the Task specified from the Ready Queue.
*/
BOOL OS_RemoveTaskFromReadyQueue(TASK *TaskToRemoveFromReadyQueue)
{
	// If we are removing the current Node...
	if (gCurrentNode == &(TaskToRemoveFromReadyQueue->TaskNodeArray[PRIMARY_TASK_NODE].ListNode))
	{
		gCurrentNode = gCurrentNode->PreviousNode;
	}
		
	OS_RemoveTaskFromCPUScheduler(TaskToRemoveFromReadyQueue);

	// if we have removed all tasks from the current priority, we must find a new one
	if(gCPUScheduler[TaskToRemoveFromReadyQueue->TaskInfo.bits.Priority].End == (DOUBLE_LINKED_LIST_NODE*)NULL && gCurrentSystemPriority == TaskToRemoveFromReadyQueue->TaskInfo.bits.Priority)
	{
		// this means we have to find a new priority level to execute
		do
		{
			gCurrentSystemPriority--;
		}
		while(gCPUScheduler[gCurrentSystemPriority].Beginning == (DOUBLE_LINKED_LIST_NODE*)NULL);

		// get the current node to point to the last node
		gCurrentNode = gCPUScheduler[gCurrentSystemPriority].End;

		return TRUE;
	}

	// no context switch needed
	return FALSE;
}

BOOL OS_AddTaskListToReadyQueue(DOUBLE_LINKED_LIST_HEAD *ListHead)
{
	DOUBLE_LINKED_LIST_NODE *Iterator;
	TASK *TaskHandle;
	UINT32 i;
	BOOL CallNextTask = FALSE;

	if (ListHead->Beginning == NULL)
		return FALSE;

	Iterator = ListHead->Beginning;

	while (Iterator != (DOUBLE_LINKED_LIST_NODE*)NULL)
	{
		TaskHandle = (TASK*)Iterator->Data;

		// remove all the nodes from any lists they were on
		for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
			OS_RemoveTaskFromList(&TaskHandle->TaskNodeArray[i]);

		if (OS_AddTaskToReadyQueue(TaskHandle) == TRUE)
			CallNextTask = TRUE;

		Iterator = ListHead->Beginning;
	}

	return CallNextTask;
}

BOOL OS_PlaceTaskOnBlockedList(TASK *Task, DOUBLE_LINKED_LIST_HEAD *BlockedListHead, TASK_NODE *TaskNode, TASK_STATE NewTaskState, BOOL RemoveTaskFromReadyQueue)
{
	BOOL CallNextTask = FALSE;

	// set the current task state to BLOCKED
	Task->TaskInfo.bits.State = NewTaskState;
	
	// remove the current task from the ready queue
	if (RemoveTaskFromReadyQueue == TRUE)
		CallNextTask = OS_RemoveTaskFromReadyQueue(Task);

	// Make the Task point to the head of the list it is in, wherever it might be.
	OS_AddTaskToList(BlockedListHead, TaskNode);

	return CallNextTask;
}

void OS_InitializeTaskNode(TASK_NODE *TaskNode, void *Data)
{
	InitializeDoubleLinkedListNode(&TaskNode->ListNode);

	TaskNode->ListNode.Data = Data;

	TaskNode->ListHead = (DOUBLE_LINKED_LIST_HEAD*)NULL;
}

void OS_AddTaskToList(DOUBLE_LINKED_LIST_HEAD *Head, TASK_NODE *TaskNode)
{
	TaskNode->ListHead = Head;
	
	InsertNodeAtEndOfDoubleLinkedList(Head, &TaskNode->ListNode);
}

TASK *OS_RemoveTaskFromList(TASK_NODE *TaskNode)
{
	// If it's empty, just return
	if (TaskNode->ListHead == (DOUBLE_LINKED_LIST_HEAD*)NULL)
		return (TASK*)TaskNode->ListNode.Data;

	RemoveNodeFromDoubleLinkedList(TaskNode->ListHead, &TaskNode->ListNode);

	TaskNode->ListHead = (DOUBLE_LINKED_LIST_HEAD*)NULL;

	return (TASK*)TaskNode->ListNode.Data;
}

//--------------------------------------------------------------------------------------------------//
//																									//
//									End user non-callable section									//
//																									//
//--------------------------------------------------------------------------------------------------//
