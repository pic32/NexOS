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

#ifndef TASKOBJECT_H
    #define TASKOBJECT_H

#include "RTOSConfig.h"
#include "GenericTypeDefs.h"
#include "../../Generic Libraries/Double Linked List/DoubleLinkedList.h"
#include "Port.h"

#if (USING_TASK_DELAY_TICKS_METHOD == 1)
    // This is how many TASK_NODE's are in a TASK
    #define NUMBER_OF_INTERNAL_TASK_NODES							2
#else
    #define NUMBER_OF_INTERNAL_TASK_NODES							1
#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

// these node declarations are for use in the blocked lists
#define PRIMARY_TASK_NODE										0
#define SECONDARY_TASK_NODE										1

#ifndef TASK_ENTRY_POINT_DATA_TYPE
	#define TASK_ENTRY_POINT_DATA_TYPE

	typedef UINT32 (*TASK_ENTRY_POINT)(void *);
#endif // end of #ifndef TASK_ENTRY_POINT_DATA_TYPE

#ifndef TASK_EXIT_POINT_DATA_TYPE
	#define TASK_EXIT_POINT_DATA_TYPE

	typedef void (*TASK_EXIT_CALLBACK)(void);
#endif // end of #ifndef TASK_EXIT_POINT_DATA_TYPE

typedef enum
{
    READY = 0,

	#if(USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)
		SUSPENDED,
	#endif // end of #if(USING_SUSPEND_TASK_METHOD == 1 || USING_SUSPEND_ALL_TASKS_METHOD == 1)

    BLOCKED,
    RESTARTING,
    DELETING,

	#if(USING_TASK_HIBERNATION == 1)
		HIBERNATING,
	#endif // end of #if(USING_TASK_HIBERNATION == 1)

    NUMBER_OF_TASK_STATES
}TASK_STATE;

/*
 * This data structure is used to store information so that a TASK can be restarted.
 */
typedef struct
{
	TASK_ENTRY_POINT StartingAddress;
	BYTE TaskPriority;
	TASK_STATE TaskState;
	UINT32 StartingTaskStackSizeInWords;
}TASK_RESTART_INFO;

typedef struct
{
	DOUBLE_LINKED_LIST_NODE ListNode;	// This is used by whatever Queue the Task is in
	DOUBLE_LINKED_LIST_HEAD *ListHead;	// this is the list that the node is blocked on
}TASK_NODE;

typedef struct
{
	UINT32 CurrentCount;
	UINT32 Timeout;
	TASK_NODE TaskCheckInNode;
}TASK_CHECK_IN;

typedef struct
{
    union
    {
        struct
        {
            unsigned int Bit0 : 1;
            unsigned int Bit1 : 1;
            unsigned int Bit2 : 1;
            unsigned int Bit3 : 1;
            unsigned int Bit4 : 1;
            unsigned int Bit5 : 1;
            unsigned int Bit6 : 1;
            unsigned int Bit7 : 1;
            unsigned int Bit8 : 1;
            unsigned int Bit9 : 1;
            unsigned int Bit10 : 1;
            unsigned int Bit11 : 1;
            unsigned int Bit12 : 1;
            unsigned int Bit13 : 1;
            unsigned int Bit14 : 1;
            unsigned int Bit15 : 1;
        }BITS;
        UINT16 Word;
    }WaitOnSignals;
    
    union
    {
        struct
        {
            unsigned int Bit0 : 1;
            unsigned int Bit1 : 1;
            unsigned int Bit2 : 1;
            unsigned int Bit3 : 1;
            unsigned int Bit4 : 1;
            unsigned int Bit5 : 1;
            unsigned int Bit6 : 1;
            unsigned int Bit7 : 1;
            unsigned int Bit8 : 1;
            unsigned int Bit9 : 1;
            unsigned int Bit10 : 1;
            unsigned int Bit11 : 1;
            unsigned int Bit12 : 1;
            unsigned int Bit13 : 1;
            unsigned int Bit14 : 1;
            unsigned int Bit15 : 1;
        }BITS;
        UINT16 Word;
    }CurrentlySetSignals;
}SIGNAL_FLAGS;

typedef union
{
    SIGNAL_FLAGS SignalFlags;
    UINT32 Word;
}SIGNALS;

typedef struct
{
    SIGNALS UserSignals; // these are the bits the TASK is waiting to be set
    SIGNALS OS_Signals; // these are the bits that are set by the outside world
}TASK_SIGNAL;

typedef union
{
    struct
    {
        unsigned int State : 3; // The tasks current state, whether it's READY, BLOCKED, SUSPENDED etc.
        unsigned int Suspendable : 1; // If set to TRUE, the task can be suspended
        unsigned int Priority : 8; // The Tasks priority, the higher the number, the higher the priority
        unsigned int Reserved : 20; // These are currently reserved for future use.
    }bits;
    
    UINT32 Word;
}TASK_INFO;

typedef struct
{
	TASK_NODE TaskNodeArray[NUMBER_OF_INTERNAL_TASK_NODES]; // this is what is used by lists to store the TASK
    OS_WORD *TaskStackPointer; // A pointer to a memory location as to where the tasks stack resides
	OS_WORD CriticalCount;  // This stores the critical count of the system when the TASK was blocked by the OS
    TASK_INFO TaskInfo; // this stores various bits of information about the TASK

	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		UINT32 DelayInTicks; // This is how many ticks the TASK is delaying
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	#if(USING_TASK_CHECK_IN == 1)
		TASK_CHECK_IN *TaskCheckIn;
	#endif // end of USING_TASK_CHECK_IN

    #if (USING_TASK_NAMES == 1)
        BYTE TaskName[TASK_NAME_LENGTH_IN_BYTES + 1];
    #endif // end of USING_TASK_NAMES

    #if (USING_DELETE_TASK == 1) || (USING_RESTART_TASK == 1)
        OS_WORD *StartOfTaskStackPointer;
    #endif // end of using task restart or delete task

    #if (USING_RESTART_TASK == 1)
        TASK_RESTART_INFO *RestartInfo;
    #endif // end of (RESTART_TASK == 1)

    #if (USING_TASK_EXIT_METHOD_CALLBACK == 1)
        // when a task is deleted, the user can have the OS call this method for a cleanup
		TASK_EXIT_CALLBACK TaskExitMethod;
    #endif // end of USING_TASK_EXIT_METHOD_CALLBACK

	#if (USING_TASK_SIGNAL == 1)
		TASK_SIGNAL TaskSignal;
	#endif // end of #if (USING_TASK_SIGNAL == 1)

    #if(USING_DELETE_TASK == 1 && (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_PIPE_DELETE_FROM_ISR_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1))
        /*
         * This points to a block counter inside of a resource.  This allows a TASK which
         * is blocking on a resource to be deleted while blocking.  Once the TASK is marked
         * for deletion, if DeleteBlockCounter != (UINT32*)NULL the OS assumes it is pointing
         * to a valid resource block counter.  The OS will then decrement the count at *DeleteBlockCounter
         * prior to deleting a TASK.  If this wasn't done, then when the TASK blocked on a resource
         * was deleted, the block count on that resource would never reach 0.  If it could
         * never reach zero, then the resource could never be deleted if need be.
         */
        UINT32 *DeleteBlockCounter;
    #endif // end of #if(USING_DELETE_TASK == 1 && (USING_DELETE_BINARY_SEMAPHORE_METHOD == 1 || USING_PIPE_DELETE_FROM_ISR_METHOD == 1 || USING_DELETE_COUNTING_SEMAPHORE_METHOD == 1 || USING_DELETE_MUTEX_METHOD == 1 || USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1))
        
    #if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)
        void *TaskLocalThreadStorageArray[NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS];
    #endif // end of #if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)
}TASK;

#endif	/* TASKOBJECT_H */

