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

#include "CriticalSection.h"
#include "Event.h"
#include "../Kernel/Kernel.h"
#include "../Kernel/Memory.h"

#if (USING_EVENT_TIMERS == 1)
	#include "../Event/Event Timer/EventTimer.h"
#endif // end of #if (USING_EVENT_TIMERS == 1)

static DOUBLE_LINKED_LIST_HEAD gSystemEventsList[NUMBER_OF_EVENTS - 1];

extern TASK * volatile gCurrentTask;

BOOL OS_InitEventsLib(void)
{
	UINT32 i;

	// just initialize all elements in the array
	for (i = 0; i < (UINT32)NUMBER_OF_EVENTS - 1; i++)
		InitializeDoubleLinkedListHead(&gSystemEventsList[i]);

	return TRUE;
}

BOOL OS_RaiseEvent(EVENT Event)
{
	// if event timers are enabled, pass the event to them too.
	#if (USING_EVENT_TIMERS == 1)
		OS_EventTimersProcessEvent(Event);
	#endif // end of #if (USING_EVENT_TIMERS == 1)

	// we must normalize the number since NO_EVENT is zero
	Event--;

	// just add the blocked list to the ready queue
	return OS_AddTaskListToReadyQueue(&gSystemEventsList[Event]);
}

#if (USING_RAISE_EVENT_METHOD == 1)
	OS_RESULT RaiseEvent(EVENT Event)
	{
		if (Event < USER_1_EVENT || Event >= NUMBER_OF_EVENTS)
			return OS_INVALID_EVENT;

		EnterCritical();

		if (OS_RaiseEvent(Event) == TRUE)
			SurrenderCPU();

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of #if (USING_RAISE_EVENT_METHOD == 1)

OS_RESULT WaitOnEvent(	EVENT Event

						#if (USING_TASK_DELAY_TICKS_METHOD == 1)
							, UINT32 TimeoutInTicks
						#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

						)
{
	OS_RESULT Result = OS_SUCCESS;

	if (Event >= NUMBER_OF_EVENTS)
		return OS_INVALID_ARGUMENT;

	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		if (TimeoutInTicks >= INVALID_TIMEOUT_TICKS_VALUE)
			return OS_INVALID_ARGUMENT;
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	EnterCritical();

	// we must normalize the number since NO_EVENT is zero
	Event--;

	// add the task to the event list
	OS_PlaceTaskOnBlockedList(gCurrentTask, &gSystemEventsList[Event], &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], BLOCKED, TRUE);

	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		if (TimeoutInTicks > 0)
		{
			// place on timer list
			OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], TimeoutInTicks, FALSE);
		}
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	SurrenderCPU();

	#if (USING_TASK_DELAY_TICKS_METHOD == 1)
		if (gCurrentTask->DelayInTicks == TASK_TIMEOUT_DONE_VALUE)
		{
			// we timed out
			Result = OS_TASK_TIMEOUT;
		}

		gCurrentTask->DelayInTicks = 0;
	#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

	ExitCritical();

	return Result;
}
