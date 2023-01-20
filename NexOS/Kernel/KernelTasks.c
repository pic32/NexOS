/*
    NexOS Kernel Version v1.02.01
    Copyright (c) 2023 brodie

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

#include "Kernel.h"
#include "Memory.h"
#include "KernelTasks.h"
#include "CriticalSection.h"
#include "OS_Callback.h"
#include "Task.h"

#if (USING_DELETE_TASK == 1 || USING_RESTART_TASK == 1)
    #include "../../Generic Libraries/Double Linked List/DoubleLinkedList.h"
	#include "Kernel.h"

    #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
        extern UINT32 gNumberOfTasks;
    #endif // end of #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)

    #if (USING_TASK_HIBERNATION == 1)
        extern DOUBLE_LINKED_LIST_HEAD gMiscellaneousBlockedQueueHead;
    #endif // end of #if (USING_TASK_HIBERNATION == 1)

    #if (USING_DELETE_TASK == 1)
		extern DOUBLE_LINKED_LIST_HEAD gDeleteTaskList;

        /*
        * This method fully deletes the TASK passed to it.
        */
		static void MaintenanceDeleteTask(TASK *Task)
		{
			#if (USING_RESTART_TASK == 1)
				if(Task->RestartInfo != (TASK_RESTART_INFO*)NULL)
					OS_ReleaseMemory((void*)Task->RestartInfo);
			#endif // end of #if (USING_RESTART_TASK == 1)

			// make sure to delete the task check in so no memory leaks occur
			#if (USING_TASK_CHECK_IN == 1)
				if(Task->TaskCheckIn != (TASK_CHECK_IN*)NULL)
					OS_ReleaseMemory((void*)Task->TaskCheckIn);
			#endif // end of USING_TASK_CHECK_IN
                
			OS_ReleaseMemory((void*)Task->StartOfTaskStackPointer);
	
			#if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
				if(Task->TaskExitMethod != (void(*)(void))NULL)
					Task->TaskExitMethod();
			#endif // end of USING_TASK_EXIT_METHOD_CALLBACK
            
			OS_ReleaseMemory((void*)Task);
            
            #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
                gNumberOfTasks--;
            #endif // end of #if (USING_GET_NUMBER_OF_TASKS_METHOD == 1)
		}
        
        /*
         * This method sends for deletion any TASKs found on the gDeleteTaskList list.
         */
        static void MaintenanceCheckDeleteTask(void)
        {
            DOUBLE_LINKED_LIST_NODE *Node;
            TASK *Task;
            
            EnterCritical();
            {
                while (DoubleLinkedListHasData(&gDeleteTaskList) == TRUE)
                {
                    Node = RemoveNodeFromDoubleLinkedList(&gDeleteTaskList, gDeleteTaskList.Beginning);

                    Task = (TASK*)Node->Data;

                    MaintenanceDeleteTask(Task);
                }
            }
            ExitCritical();
        }
	#endif // end of USING_DELETE_TASK

    #if (USING_RESTART_TASK == 1)
		extern DOUBLE_LINKED_LIST_HEAD gRestartTaskList;

		#if (USING_TASK_HIBERNATION == 1)
			extern DOUBLE_LINKED_LIST_HEAD gHibernationQueueHead;
		#endif // end of #if (USING_TASK_HIBERNATION == 1)

		#if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
			extern DOUBLE_LINKED_LIST_HEAD gSuspendedQueueHead;
		#endif // end of #if (USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

		static void MaintenanceRestartTask(TASK *Task)
		{
			UINT32 i;

			OS_ReleaseMemory((void*)Task->StartOfTaskStackPointer);
	
			// re-initialize the TASK stack
			//Task->TaskStackPointer = OS_InitializeTaskStack(Task, Task->RestartInfo->StartingAddress, (void*)NULL, Task->RestartInfo->StartingTaskSize);
            Task->TaskStackPointer = PortInitializeTaskStack(Task->StartOfTaskStackPointer, Task->RestartInfo->StartingTaskStackSizeInWords, Task->RestartInfo->StartingAddress, (void*)NULL);
	
			// reset the critical count
			Task->CriticalCount = 0;
	
			// shouldn't be anything but 0, but just to be sure
			#if (USING_TASK_DELAY_METHOD == 1)
				Task->DelayInTicks = 0;
			#endif // end of USING_TASK_DELAY_METHOD
	
			Task->TaskInfo.bits.State = Task->RestartInfo->TaskState;
			Task->TaskInfo.bits.Priority = Task->RestartInfo->TaskPriority;

			for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
				OS_InitializeTaskNode(&Task->TaskNodeArray[i], (void*)Task);
			
			// make sure to delete the task checking so no memory leaks occur
			#if (USING_TASK_CHECK_IN == 1)
				if (Task->TaskCheckIn != (TASK_CHECK_IN*)NULL)
					OS_ReleaseMemory((void*)Task->TaskCheckIn);
			#endif // end of USING_TASK_CHECK_IN

			#if (USING_MAILBOXES == 1)
				// Delete any unread messages
			#endif // end of USING_MAILBOXES

			EnterCritical();
			{
				switch (Task->TaskInfo.bits.State)
				{
					case READY:
					{
						// we will always be higher priority than the TASK 
						// that is being added.  So we will not have to check
						// to see if we should surrender the CPU.
						OS_AddTaskToReadyQueue(Task);

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
				}
			}
			ExitCritical();
		}		
	#endif // end of USING_RESTART_TASK
#endif // end of #if (USING_DELETE_TASK == 1 || USING_RESTART_TASK == 1)

/*
	This is the Task code for when the Idle Task Executes.
*/
UINT32 IdleTaskCode(void *Args)
{
    while(1)
    {
		#if (USING_IDLE_TASK_USER_CALLBACK == 1)
			IdleTaskUserCallback(Args);
		#endif // end of #if (USING_IDLE_TASK_USER_CALLBACK != 1)

        #if (USING_DELETE_TASK == 1 || USING_RESTART_TASK == 1)
            #if (IDLE_TASK_PERFORM_DELETE_TASK == 1)
                MaintenanceCheckDeleteTask();
            #endif // end of if (IDLE_TASK_PERFORM_DELETE_TASK == 1)
        #endif // end of #if (USING_DELETE_TASK == 1 || USING_RESTART_TASK == 1)
	}
}

#if ((USING_DELETE_TASK == 1 && IDLE_TASK_PERFORM_DELETE_TASK == 0) || USING_RESTART_TASK == 1)
	UINT32 MaintenanceTaskCode(void *Args)
	{
        #if (USING_RESTART_TASK == 1)
            DOUBLE_LINKED_LIST_NODE *Node;
            TASK *Task;
        #endif // end of #if (USING_RESTART_TASK == 1)

		while(1)
		{
			TaskDelayTicks(MAINTENANCE_TASK_DELAY_TICKS);

			#if (USING_MAINTENANCE_TASK_USER_CALLBACK == 1)
				MaintenanceTaskUserCallback(Args);
			#endif // end of #if (USING_MAINTENANCE_TASK_USER_CALLBACK == 1)

			#if (USING_DELETE_TASK == 1 && IDLE_TASK_PERFORM_DELETE_TASK == 0)
                MaintenanceCheckDeleteTask();
			#endif // end of #if (USING_DELETE_TASK == 1 && IDLE_TASK_PERFORM_DELETE_TASK == 0)

			#if (USING_RESTART_TASK == 1)
				EnterCritical();
				{
					while (DoubleLinkedListHasData(&gRestartTaskList) == TRUE)
					{
						Node = RemoveNodeFromDoubleLinkedList(&gRestartTaskList, gRestartTaskList.Beginning);

						ExitCritical();

						Task = (TASK*)Node->Data;

						MaintenanceRestartTask(Task);

						EnterCritical();
					}
				}
				ExitCritical();
			#endif // end of USING_RESTART_TASK
		}
	}
#endif // end of #if ((USING_DELETE_TASK == 1 && IDLE_TASK_PERFORM_DELETE_TASK == 0) || USING_RESTART_TASK == 1)

#if (USING_IO_BUFFERS == 1)
    #if (USING_TASK_DELAY_TICKS_METHOD == 0)
        #error "USING_TASK_DELAY_TICKS_METHOD must be defined as 1 if USING_IO_BUFFERS == 1"
    #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 0)
    
    #include "../IOBuffer/IOBuffer.h"
    #include "IOBufferPort.h"

	UINT32 IOBufferTaskCode(void *Args)
	{
		IO_BUFFER_ID CurrentIOBuffer;

		while(1)
		{
            for(CurrentIOBuffer = INVALID_IO_BUFFER + 1; CurrentIOBuffer < NUMBER_OF_BUFFER_IDS; CurrentIOBuffer++)
            {
                EnterCritical();

                // update the CurrentIOBuffer here
                OSUpdateIOBuffer(CurrentIOBuffer);

                ExitCritical();
            }
            
			#if (USING_IO_BUFFER_TASK_USER_CALLBACK == 1)
				IOBufferTaskUserCallback(Args);
			#endif // end of #if (USING_IO_BUFFER_TASK_USER_CALLBACK == 1)

            TaskDelayTicks(IO_BUFFER_TASK_PERIODICITY_IN_OS_TICKS);
		}
	}
#endif // end of #if (USING_IO_BUFFERS == 1)
