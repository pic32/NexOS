/*
    NexOS Kernel Version v1.01.03
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

#include "../NexOS/Kernel/Kernel.h"
#include "../NexOS/Kernel/Memory.h"
#include "EventTimer.h"
#include "CriticalSection.h"

/*
	This is the EVENT_TIMER list which holds all EVENT_TIMERs that have
	their variable EventTimerEnabled set to TRUE.
*/
static DOUBLE_LINKED_LIST_HEAD gEventTimerList;

static void OS_DisableEventTimer(EVENT_TIMER *EventTimer)
{
	if (EventTimer->EventTimerEnabled == TRUE)
	{
		EventTimer->EventTimerEnabled = FALSE;

		RemoveNodeFromDoubleLinkedList(&gEventTimerList, &EventTimer->Node);
	}
}

static void OS_EventTimerProcessStartEvent(EVENT Event, EVENT_TIMER *EventTimer, UINT32 CurrentOSTickCount)
{
	SOFTWARE_TIMER *Timer = &EventTimer->Timer;
    
    #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
        // now call the callback
        if(EventTimer->StartEventCallback != (START_EVENT_CALLBACK)NULL)
            EventTimer->StartEventCallback(CurrentOSTickCount);
    #endif // end of #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
    
    if(EventTimer->TimerStartPolicy == CALLBACK_ON_EACH_START_EVENT)
        return;

	switch (EventTimer->TimerStartPolicy)
	{
		case ONLY_START_WHEN_NOT_RUNNING:
		{
			if (OS_SoftwareTimerIsRunning(Timer) == FALSE)
			{
				// its a match, start the timer
				OS_SoftwareTimerEnable(Timer, TRUE);
			}

			break;
		}

		case RESTART_TIMER_ON_EACH_START_EVENT:
		{
			if (OS_SoftwareTimerIsRunning(Timer) == FALSE)
			{
				// its a match, start the timer
				OS_SoftwareTimerEnable(Timer, TRUE);
			}

			// clear the timer tick counter
			OS_SoftwareTimerClear(Timer);

			break;
		}
        
        default:
        {
            // we should never get here
            
            break;
        }
	}
}

static void OS_ProcessEventTimerStopEvent(EVENT Event, EVENT_TIMER *EventTimer, UINT32 CurrentOSTickCount)
{
	SOFTWARE_TIMER *Timer = &EventTimer->Timer;

    // we only care about stopping if the timer isn't running
    if (OS_SoftwareTimerIsRunning(Timer) == TRUE)
    {
        #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)
            // now call the callback
            if(EventTimer->StopEventCallback != (STOP_EVENT_CALLBACK)NULL)
                EventTimer->StopEventCallback(CurrentOSTickCount, OS_SoftwareTimerGetTicks(Timer), (void*)EventTimer);
        #endif // end of #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)

        // if they just wanted the callback, leave
        if (EventTimer->TimerStopPolicy == CALLBACK_ON_EACH_STOP_EVENT)
            return;

        // stop the timer since every state but CALLBACK_ON_EACH_STOP_EVENT wants the timer stopped
        OS_SoftwareTimerEnable(Timer, FALSE);

        switch (EventTimer->TimerStopPolicy)
        {
            case CALLBACK_ON_EACH_STOP_EVENT:
            {
                break;
            }

            case STOP_TIMER:
            {
                // the timer has already been stopped
                break;
            }

            case STOP_CLEAR_TIMER:
            {
                // just need to clear it out here
                OS_SoftwareTimerClear(Timer);

                break;
            }

            case STOP_CLEAR_DISABLE_TIMER:
            {
                // we need to disable and clear the timer
                OS_DisableEventTimer(EventTimer);

                // just need to clear it out here
                OS_SoftwareTimerClear(Timer);

                break;
            }

            case STOP_DISABLE_TIMER:
            {
                // just need to disable the timer here
                OS_DisableEventTimer(EventTimer);

                break;
            }
            
            default:
            {
                break;
            }
        }
    }
}

static void OS_EventTimerProcessEvent(EVENT Event, EVENT_TIMER *EventTimer, UINT32 CurrentOSTickCount)
{
	// do we have a start event?
	if (Event == EventTimer->StartEvent)
    {
		OS_EventTimerProcessStartEvent(Event, EventTimer, CurrentOSTickCount);
    }
	else
    {
        if (Event == EventTimer->StopEvent)
        {
            OS_ProcessEventTimerStopEvent(Event, EventTimer, CurrentOSTickCount);
        }
    }
}

void OS_EventTimersProcessEvent(EVENT Event)
{
	DOUBLE_LINKED_LIST_NODE *TempCurrentNode, *TempNextNode;
	UINT32 CurrentOSTickCount = GetOSTickCountFromISR();

	// now update the timer list
	// we just have to iterate through and see if any timer has the event for start or stop
	// only EVENT_TIMERs which are currently enabled are on this list.
	TempCurrentNode = gEventTimerList.Beginning;

	while (TempCurrentNode != (DOUBLE_LINKED_LIST_NODE*)NULL)
	{
		TempNextNode = TempCurrentNode->NextNode;

		// This method takes the event and will start/stop timer based on it.
		OS_EventTimerProcessEvent(Event, (EVENT_TIMER*)TempCurrentNode->Data, CurrentOSTickCount);

		TempCurrentNode = TempNextNode;
	}
}

BOOL OS_InitEventTimersLib(void)
{
	InitializeDoubleLinkedListHead(&gEventTimerList);

	return TRUE;
}

EVENT_TIMER *CreateEventTimer   (EVENT_TIMER *EventTimer, EVENT StartEvent, EVENT StopEvent, EVENT_TIMER_START_POLICY TimerStartPolicy, EVENT_TIMER_STOP_POLICY TimerStopPolicy, BOOL Enable

                                #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
                                    , START_EVENT_CALLBACK StartEventCallback
                                #endif // end of #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)

                                #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)
                                    , STOP_EVENT_CALLBACK StopEventCallback
                                #endif // end of #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)

                                )
{
    #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
        // the two events cannot be the same
        if (StartEvent == StopEvent)
            return (EVENT_TIMER*)NULL;

        if (StartEvent >= NUMBER_OF_EVENTS || StopEvent >= NUMBER_OF_EVENTS)
            return (EVENT_TIMER*)NULL;

        if (StartEvent == NO_EVENT || StopEvent == NO_EVENT)
            return (EVENT_TIMER*)NULL;

        #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
            if(StartEventCallback != (START_EVENT_CALLBACK)NULL)
                if (ProgramAddressValid((OS_WORD)StartEventCallback) == FALSE)
                    return (EVENT_TIMER*)NULL;

            if(StartEventCallback == (START_EVENT_CALLBACK)NULL)
                if(TimerStartPolicy == CALLBACK_ON_EACH_START_EVENT)
                    return (EVENT_TIMER*)NULL;
        #endif // end of #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)

        #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)
            if(StopEventCallback != (STOP_EVENT_CALLBACK)NULL)
                if (ProgramAddressValid((OS_WORD)StopEventCallback) == FALSE)
                    return (EVENT_TIMER*)NULL;

            if(StopEventCallback == (STOP_EVENT_CALLBACK)NULL)
                if(TimerStopPolicy == CALLBACK_ON_EACH_STOP_EVENT)
                    return (EVENT_TIMER*)NULL;
        #endif // end of #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)

        if (TimerStartPolicy >= NUMBER_OF_TIMER_START_POLICIES)
            return (EVENT_TIMER*)NULL;

        if (TimerStopPolicy >= NUMBER_OF_TIMER_STOP_POLICIES)
            return (EVENT_TIMER*)NULL;
    #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

	if (EventTimer != (EVENT_TIMER*)NULL)
	{
		if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
			return (EVENT_TIMER*)NULL;
	}
	else
	{
		EventTimer = (EVENT_TIMER*)AllocateMemory(sizeof(EVENT_TIMER));

		if (EventTimer == (EVENT_TIMER*)NULL)
			return (EVENT_TIMER*)NULL;
	}

	// now just populate everything
	EventTimer->Node.NextNode = EventTimer->Node.PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;
	EventTimer->Node.Data = EventTimer;
	EventTimer->StartEvent = StartEvent;
	EventTimer->StopEvent = StopEvent;
    
    #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
        EventTimer->StartEventCallback = StartEventCallback;
    #endif // end of #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)

    #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)
        EventTimer->StopEventCallback = StopEventCallback;
    #endif // end of #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)
    
	EventTimer->TimerStartPolicy = TimerStartPolicy;
	EventTimer->TimerStopPolicy = TimerStopPolicy;
	EventTimer->EventTimerEnabled = Enable;

	CreateSoftwareTimer(&(EventTimer->Timer), FALSE);

	// if they wanted it enabled, put it on the event timer list
	if (Enable == TRUE)
	{
		EnterCritical();

		InsertNodeAtEndOfDoubleLinkedList(&gEventTimerList, &EventTimer->Node);
	
		ExitCritical();
	}

	return EventTimer;
}

#if (USING_EVENT_TIMER_SET_ENABLE_METHOD == 1)
	OS_RESULT EventTimerSetEnable(EVENT_TIMER *EventTimer, BOOL Enable)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// if it is already in the requested state, just leave
		if (EventTimer->EventTimerEnabled != Enable)
		{
			if (Enable == TRUE)
			{
				InsertNodeAtEndOfDoubleLinkedList(&gEventTimerList, &EventTimer->Node);
			}
			else
			{
				RemoveNodeFromDoubleLinkedList(&gEventTimerList, &EventTimer->Node);

				// if we are disabling the software timer, take it off the SOFTWARE_TIMER list
				SoftwareTimerEnable(&(EventTimer->Timer), Enable);
			}

			EventTimer->EventTimerEnabled = Enable;
		}

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of #if (USING_EVENT_TIMER_SET_ENABLE_METHOD == 1)

#if (USING_EVENT_TIMER_SET_START_EVENT_METHOD == 1)
	OS_RESULT EventTimerSetStartEvent(EVENT_TIMER *EventTimer, EVENT StartEvent)
	{
		OS_RESULT Result;

        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if (StartEvent >= NUMBER_OF_EVENTS)
                return OS_INVALID_ARGUMENT;

            if (StartEvent == NO_EVENT)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// start and stop cannot be the same
		if (EventTimer->StopEvent == StartEvent)
		{
			Result = OS_INVALID_ARGUMENT;
		}
		else
		{
			// now set the start event since we know it is valid
			EventTimer->StartEvent = StartEvent;

			Result = OS_SUCCESS;
		}

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_EVENT_TIMER_SET_START_EVENT_METHOD == 1)

#if (USING_EVENT_TIMER_SET_STOP_EVENT_METHOD == 1)
	OS_RESULT EventTimerSetStopEvent(EVENT_TIMER *EventTimer, EVENT StopEvent)
	{
		OS_RESULT Result;

        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if (StopEvent >= NUMBER_OF_EVENTS)
                return OS_INVALID_ARGUMENT;

            if (StopEvent == NO_EVENT)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// start and stop cannot be the same
		if (EventTimer->StartEvent == StopEvent)
		{
			Result = OS_INVALID_ARGUMENT;
		}
		else
		{
			// now set the start event since we know it is valid
			EventTimer->StopEvent = StopEvent;

			Result = OS_SUCCESS;
		}

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_EVENT_TIMER_SET_STOP_EVENT_METHOD == 1)

#if (USING_EVENT_TIMER_SET_START_EVENT_CALLBACK_METHOD == 1)
	OS_RESULT EventTimerSetStartEventCallback(EVENT_TIMER *EventTimer, START_EVENT_CALLBACK StartEventCallback)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(StartEventCallback != (START_EVENT_CALLBACK)NULL)
                if (ProgramAddressValid((OS_WORD)StartEventCallback) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now set the callback since we know it's valid
		EventTimer->StartEventCallback = StartEventCallback;

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of #if (USING_EVENT_TIMER_SET_START_EVEVENT_CALLBACK_METHOD == 1)

#if (USING_EVENT_TIMER_SET_STOP_EVENT_CALLBACK_METHOD == 1)
	OS_RESULT EventTimerSetStopEventCallback(EVENT_TIMER *EventTimer, STOP_EVENT_CALLBACK StopEventCallback)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(StopEventCallback != (STOP_EVENT_CALLBACK)NULL)
                if (ProgramAddressValid((OS_WORD)StopEventCallback) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now set the callback since we know it's valid
		EventTimer->StopEventCallback = StopEventCallback;

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of #if (USING_EVENT_TIMER_SET_STOP_EVEVENT_CALLBACK_METHOD == 1)

#if (USING_EVENT_TIMER_SET_EVENT_TIMER_START_POLICY_METHOD == 1)
	OS_RESULT EventTimerSetTimerStartPolicy(EVENT_TIMER *EventTimer, EVENT_TIMER_START_POLICY TimerStartPolicy)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if (TimerStartPolicy >= NUMBER_OF_TIMER_START_POLICIES)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now set the timer policy since we know it is valid
		EventTimer->TimerStartPolicy = TimerStartPolicy;

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of #if (USING_EVENT_TIMER_SET_EVENT_TIMER_START_POLICY_METHOD == 1)

#if (USING_EVENT_TIMER_SET_EVENT_TIMER_STOP_POLICY_METHOD == 1)
	OS_RESULT EventTimerSetTimerStopPolicy(EVENT_TIMER *EventTimer, EVENT_TIMER_STOP_POLICY TimerStopPolicy)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if (TimerStopPolicy >= NUMBER_OF_TIMER_STOP_POLICIES)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now set the timer policy since we know it is valid
		EventTimer->TimerStopPolicy = TimerStopPolicy;

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of #if (USING_EVENT_TIMER_SET_EVENT_TIMER_STOP_POLICY_METHOD == 1)

#if (USING_EVENT_TIMER_CLEAR_TIMER_METHOD == 1)
	OS_RESULT EventTimerClearTimer(EVENT_TIMER *EventTimer)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		OS_SoftwareTimerClear(&EventTimer->Timer);

		ExitCritical();

		return OS_SUCCESS;
	}
#endif // end of #if (USING_EVENT_TIMER_CLEAR_TIMER_METHOD == 1)

#if (USING_EVENT_TIMER_GET_ENABLE_METHOD == 1)
	BOOL EventTimerGetEnable(EVENT_TIMER *EventTimer)
	{
        BOOL Enabled;
        
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return FALSE;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now get the timer enabled
		Enabled = EventTimer->EventTimerEnabled;

		ExitCritical();

		return Enabled;
	}
#endif // end of #if (USING_EVENT_TIMER_GET_ENABLE_METHOD == 1)
    
#if (USING_EVENT_TIMER_GET_START_EVENT_METHOD == 1)
	EVENT EventTimerGetStartEvent(EVENT_TIMER *EventTimer)
	{
        EVENT StartEvent;
        
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return NO_EVENT;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now get the start event
		StartEvent = EventTimer->StartEvent;

		ExitCritical();

		return StartEvent;
	}
#endif // end of #if (USING_EVENT_TIMER_GET_START_EVENT_METHOD == 1)

#if (USING_EVENT_TIMER_GET_STOP_EVENT_METHOD == 1)
	EVENT EventTimerGetStopEvent(EVENT_TIMER *EventTimer)
	{
        EVENT StopEvent;
        
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return NO_EVENT;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now get the Stop event
		StopEvent = EventTimer->StopEvent;

		ExitCritical();

		return StopEvent;
	}
#endif // end of #if (USING_EVENT_TIMER_GET_STOP_EVENT_METHOD == 1)

#if (USING_EVENT_TIMER_GET_START_EVENT_CALLBACK_METHOD == 1)
	START_EVENT_CALLBACK EventTimerGetStartEventCallback(EVENT_TIMER *EventTimer)
	{
        START_EVENT_CALLBACK StartEventCallback;
        
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return (START_EVENT_CALLBACK)NULL;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now get the start callback
		StartEventCallback = EventTimer->StartEventCallback;

		ExitCritical();

		return StartEventCallback;
	}
#endif // end of #if (USING_EVENT_TIMER_GET_START_EVENT_CALLBACK_METHOD == 1)

#if (USING_EVENT_TIMER_GET_STOP_EVENT_CALLBACK_METHOD == 1)
	STOP_EVENT_CALLBACK EventTimerGetStopEventCallback(EVENT_TIMER *EventTimer)
	{
        STOP_EVENT_CALLBACK StopEventCallback;
        
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return (STOP_EVENT_CALLBACK)NULL;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now get the stop callback
		StopEventCallback = EventTimer->StopEventCallback;

		ExitCritical();

		return StopEventCallback;
	}
#endif // end of #if (USING_EVENT_TIMER_GET_STOP_EVENT_CALLBACK_METHOD == 1)

#if (USING_EVENT_TIMER_GET_TIMER_START_POLICY_METHOD == 1)
	EVENT_TIMER_START_POLICY EventTimerGetTimerStartPolicy(EVENT_TIMER *EventTimer)
	{
        EVENT_TIMER_START_POLICY StartPolicy;
        
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return NUMBER_OF_TIMER_START_POLICIES;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now get the start policy
		StartPolicy = EventTimer->TimerStartPolicy;

		ExitCritical();

		return StartPolicy;
	}
#endif // end of #if (USING_EVENT_TIMER_GET_TIMER_START_POLICY_METHOD == 1)

#if (USING_EVENT_TIMER_GET_TIMER_STOP_POLICY_METHOD == 1)
	EVENT_TIMER_STOP_POLICY EventTimerGetTimerStopPolicy(EVENT_TIMER *EventTimer)
	{
        EVENT_TIMER_STOP_POLICY StopPolicy;
        
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return NUMBER_OF_TIMER_STOP_POLICIES;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		EnterCritical();

		// now get the stop policy
		StopPolicy = EventTimer->TimerStopPolicy;

		ExitCritical();

		return StopPolicy;
	}
#endif // end of #if (USING_EVENT_TIMER_GET_TIMER_STOP_POLICY_METHOD == 1)

#if (USING_EVENT_TIMER_GET_TIMER_TICKS_METHOD == 1)
	UINT32 EventTimerGetTicks(EVENT_TIMER *EventTimer)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return 0;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		return SoftwareTimerGetTicks(&EventTimer->Timer);
	}
#endif // end of #if (USING_EVENT_TIMER_GET_TIMER_TICKS_METHOD == 1)

#if (USING_EVENT_TIMER_GET_TIMER_MILLISECONDS_METHOD == 1)
	UINT32 EventTimerGetMilliseconds(EVENT_TIMER *EventTimer)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return 0;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		return SoftwareTimerGetMilliseconds(&EventTimer->Timer);
	}
#endif // end of #if (USING_EVENT_TIMER_GET_TIMER_MILLISECONDS_METHOD == 1)

#if (USING_EVENT_TIMER_GET_TIMER_IS_RUNNING_METHOD == 1)
	BOOL EventTimerIsRunning(EVENT_TIMER *EventTimer)
	{
        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return FALSE;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		return SoftwareTimerIsRunning(&EventTimer->Timer);
	}
#endif // end of #if (USING_EVENT_TIMER_GET_TIMER_IS_RUNNING_METHOD == 1)

#if (USING_EVENT_TIMER_DELETE_METHOD == 1)
	OS_RESULT DeleteEventTimer(EVENT_TIMER *EventTimer)
	{
        OS_RESULT Result;

        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            // if it is not in the heap, then just leave 
            if (AddressInHeap((OS_WORD)EventTimer) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

        EnterCritical();
        
		// disable the event timer
		if ((Result = EventTimerSetEnable(EventTimer, FALSE)) == OS_SUCCESS)
        {
            // now we can attempt to delete it
            if(ReleaseMemory((void*)EventTimer) == TRUE)
                Result = OS_SUCCESS;
            else
                Result = OS_RESOURCE_NOT_IN_OS_HEAP;
        }
        
        ExitCritical();
        
        return Result;
	}
#endif // end of #if (USING_EVENT_TIMER_DELETE_METHOD == 1)

#if (USING_EVENT_TIMER_START_CALLBACK_ACTION_METHOD == 1)
	// This method provides a means for the CALLBACK_ON_EACH_START_EVENT to determine
	// when and how to start the timer.  This method is designed to only be called from
	// a valid START_EVENT_CALLBACK method.
	BOOL EventTimerStartCallbackAction(EVENT_TIMER *EventTimer, EVENT_TIMER_START_POLICY StartAction)
	{
		SOFTWARE_TIMER *Timer;

        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return FALSE;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		// the user can only call this method if the EVENT_TIMER has
		// the CALLBACK_ON_EACH_START_EVENT for the stop policy.
		if (EventTimer->TimerStartPolicy != CALLBACK_ON_EACH_START_EVENT)
			return FALSE;

		Timer = &EventTimer->Timer;

		switch (StartAction)
		{
            case ONLY_START_WHEN_NOT_RUNNING:
            {
                if (OS_SoftwareTimerIsRunning(Timer) == FALSE)
                {
                    // its a match, start the timer
                    OS_SoftwareTimerEnable(Timer, TRUE);
                }

                break;
            }

            case RESTART_TIMER_ON_EACH_START_EVENT:
            {
                if (OS_SoftwareTimerIsRunning(Timer) == FALSE)
                {
                    // its a match, start the timer
                    OS_SoftwareTimerEnable(Timer, TRUE);
                }

                // clear the timer tick counter
                OS_SoftwareTimerClear(Timer);

                break;
            }
            
            default:
            {
                return FALSE;
            }
		}

		return TRUE;
	}
#endif // end of #if (USING_EVENT_TIMER_START_CALLBACK_ACTION_METHOD == 1)
    
#if (USING_EVENT_TIMER_STOP_CALLBACK_ACTION_METHOD == 1)
	// This method provides a means for the CALLBACK_ON_EACH_STOP_EVENT to determine
	// when and how to stop the timer.  This method is designed to only be called from
	// a valid STOP_EVENT_CALLBACK method.
	BOOL EventTimerStopCallbackAction(EVENT_TIMER *EventTimer, EVENT_TIMER_STOP_POLICY StopAction)
	{
		SOFTWARE_TIMER *Timer;

        #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)EventTimer) == FALSE)
                return FALSE;
        #endif // end of #if (USING_CHECK_EVENT_TIMER_PARAMETERS == 1)

		// the user can only call this method if the EVENT_TIMER has
		// the CALLBACK_ON_EACH_STOP_EVENT for the stop policy.
		if (EventTimer->TimerStopPolicy != CALLBACK_ON_EACH_STOP_EVENT)
			return FALSE;

        // get a handle to the EVENT_TIMERs SOFTWARE_TIMER
		Timer = &EventTimer->Timer;

		switch (StopAction)
		{
			case STOP_TIMER:
			{
				OS_SoftwareTimerEnable(Timer, FALSE);

				break;
			}

			case STOP_CLEAR_TIMER:
			{
				OS_SoftwareTimerEnable(Timer, FALSE);

				// just need to clear it out here
				OS_SoftwareTimerClear(Timer);

				break;
			}

			case STOP_CLEAR_DISABLE_TIMER:
			{
				// disable both the software timer, and event timer data structures
				OS_SoftwareTimerEnable(Timer, FALSE);

				// we need to disable the timer
				OS_DisableEventTimer(EventTimer);

				// just need to clear it out here
				OS_SoftwareTimerClear(Timer);

				break;
			}

			case STOP_DISABLE_TIMER:
			{
				OS_SoftwareTimerEnable(Timer, FALSE);

				// we need to disable the timer
				OS_DisableEventTimer(EventTimer);

				break;
			}

			default:
			{
				return FALSE;
			}
		}

		return TRUE;
	}
#endif // end of #if (USING_EVENT_TIMER_STOP_CALLBACK_ACTION_METHOD == 1)
