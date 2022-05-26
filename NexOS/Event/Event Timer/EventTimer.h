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
	EVENT_TIMERS Description:
		An EVENT_TIMER can be used to measure the time between 2 or multiple EVENTs.
        If a TASK is used to time EVENTs it must contend with the fact that it may get
        preempted.  Otherwise the TASK may miss the EVENTs it is looking to time.
        To gurantee a TASK does not get preempted it must either stop the scheduler, or run
        at the highest available priority.  Neither of these 2 approaches are optimal for 
        overall system performance.  This is where the EVENT_TIMER can be used to measure 
        in ticks the time between EVENTs.
 
        An EVENT_TIMER can be configured to start on a certain EVENT and stop on a certain 
        EVENT.  It then contains the time in ticks betwen the 2 EVENTs and a TASK can read
        this time as needed.  There is more configurability to the starting and stopping of
        an EVENT_TIMER explained in the below method descriptions.  An EVENT_TIMER allows
        keeping track of time between EVENTs without full attention of a TASK or the CPU.
 */

#ifndef EVENT_TIMERS_H
	#define EVENT_TIMERS_H

#include "../Event.h"
#include "../../Timer/SoftwareTimer.h"

#if (USING_EVENT_TIMERS != 1)
	#error "USING_EVENT_TIMERS must be defined as a 1 in RTOSConfig.h to use Event Timers"
#endif // end of #if USING_EVENT_TIMERS != 1)

typedef void(*START_EVENT_CALLBACK)(UINT32 CurrentOSTickCount);

// The void *EventTimer is a pointer to a EVENT_TIMER.  Since STOP_EVENT_CALLBACK is used in the
// definition of a EVENT_TIMER, a void * was used instead.
typedef void(*STOP_EVENT_CALLBACK)(UINT32 CurrentOSTickCount, UINT32 EventTimerCountInTicks, void *EventTimer);

typedef enum
{
    // This is used in conjunction with a START_EVENT_CALLBACK.  The 
    // START_EVENT_CALLBACK must call EventTimerStartCallbackAction() with the
    // policy that it wants to enforce for the start event.
    CALLBACK_ON_EACH_START_EVENT = 0,
            
    // This will only start the EVENT_TIMER if it is currently not running.
	ONLY_START_WHEN_NOT_RUNNING,
            
    // This will start the EVENT_TIMER and reset its current tick count to zero.
	RESTART_TIMER_ON_EACH_START_EVENT,

	NUMBER_OF_TIMER_START_POLICIES
}EVENT_TIMER_START_POLICY;

typedef enum
{
    // This is used in conjunction with a STOP_EVENT_CALLBACK.  The 
    // STOP_EVENT_CALLBACK must call EventTimerStopCallbackAction() with the
    // policy that it wants to enforce for the stop event.
	CALLBACK_ON_EACH_STOP_EVENT = 0,
            
    // This will only stop the timer and preserves the tick count.
    // The EVENT_TIMER remains enabled and will start on the next start event.
	STOP_TIMER,
           
    // This will stop the EVENT_TIMER and clear the tick count back to zero.
    // The EVENT_TIMER remains enabled and will start on the next start event.
	STOP_CLEAR_TIMER,
            
    // This will clear the tick count back to zero and disable the EVENT_TIMER.
    // The EVENT_TIMER will not start on the next start event.
	STOP_CLEAR_DISABLE_TIMER,
            
    // This will stop the EVENT_TIMER and disable it.  The timer will not start
    // on the next start event, and its tick count is preserved.
	STOP_DISABLE_TIMER,

	NUMBER_OF_TIMER_STOP_POLICIES
}EVENT_TIMER_STOP_POLICY;

typedef struct
{
	EVENT StartEvent; // this is the event which will start the timer
	EVENT StopEvent; // this is the event which will stop the timer
    
    #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
        START_EVENT_CALLBACK StartEventCallback; // this is called whenever there is a start event (optional)
    #endif // end of #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
    
    #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)
        STOP_EVENT_CALLBACK StopEventCallback; // this is called whenever there is a stop event (optional)
    #endif // end of #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)

	SOFTWARE_TIMER Timer;
	EVENT_TIMER_START_POLICY TimerStartPolicy; // this is what the timer will do on each start event.
	EVENT_TIMER_STOP_POLICY TimerStopPolicy; // this is what the timer will do n each stop event if running.
	DOUBLE_LINKED_LIST_NODE Node; // this is for use in the event timer list
	BOOL EventTimerEnabled; // This means that the event timer is on event timer list
}EVENT_TIMER;

/*
    BOOL OS_EventTimersProcessEvent(EVENT Event)

	Description: This method takes the specified EVENT and processes it for EVENT_TIMER's.
    This entails calling all start and stop timer methods for the qualifying event.

	Blocking: No

	User Callable: No

	Arguments:
		EVENT Event - The EVENT to process.

	Returns: Nothing

	Notes:
		- This method is only callable by the OS.

	See Also:
		- None
*/
void OS_EventTimersProcessEvent(EVENT Event);

/*
    BOOL OS_InitEventTimersLib(void)

	Description: This method is called once at startup by the OS to initialize
    the EVENT_TIMER feature.

	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns: 
        BOOL - TRUE if successful, FALSE otherwise.

	Notes:
		- Only the OS calls this method and it is once at startup.

	See Also:
		- None
*/
BOOL OS_InitEventTimersLib(void);

/*
	EVENT_TIMER *CreateEventTimer(EVENT_TIMER *EventTimer, EVENT StartEvent, EVENT StopEvent, START_EVENT_CALLBACK StartEventCallback, STOP_EVENT_CALLBACK StopEventCallback, EVENT_TIMER_START_POLICY TimerStartPolicy, EVENT_TIMER_STOP_POLICY TimerStopPolicy, BOOL Enable)

	Description: This method attempts to create an EVENT_TIMER with the specified EVENTS, callbacks, and start/stop policy.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - This parameter is optional.  If the value is set
        to (EVENT_TIMER*)NULL space for a EVENT_TIMER is acquired from the OS heap.  
        If this value is not (EVENT_TIMER*)NULL the EVENT_TIMER will be initialized
        at the pointed to location in RAM.

        EVENT StartEvent - A valid EVENT which will begin the tick count of the EVENT_TIMER.

        EVENT StopEvent - A valid EVENT which will stop the tick count of the EVENT_TIMER.

        START_EVENT_CALLBACK StartEventCallback - This parameter is optional.  If
        not (START_EVENT_CALLBACK*)NULL, every start EVENT which starts the 
        EVENT_TIMER will also cause the method specified by StartEventCallback 
        to be called.  These methods cannot have any blocking calls made from them.

        STOP_EVENT_CALLBACK StopEventCallback - This parameter is optional.  If
        not (STOP_EVENT_CALLBACK*)NULL, every stop EVENT which stops the 
        EVENT_TIMER will also cause the method specified by StopEventCallback 
        to be called.  These methods cannot have any blocking calls made from them.

        EVENT_TIMER_START_POLICY TimerStartPolicy - The criteria which is used to 
        determine what to do when a valid start EVENT is generated.  Below is a 
        list of valid values and their definitions:

            CALLBACK_ON_EACH_START_EVENT - Only call the user START_EVENT_CALLBACK 
            when a valid start EVENT is generated.

            ONLY_START_WHEN_NOT_RUNNING - Only start the EVENT_TIMER counter when 
            it is not started.

            RESTART_TIMER_ON_EACH_START_EVENT - Restart the EVENT_TIMER counter 
            each time a valid start EVENT is generated.

        EVENT_TIMER_STOP_POLICY TimerStopPolicy - The criteria which is used to 
        determine what to do when a valid stop EVENT is generated.

            CALLBACK_ON_EACH_STOP_EVENT - Only call the user STOP_EVENT_CALLBACK
            when a valid stop EVENT is generated.

            STOP_TIMER - Only stop the EVENT_TIMER from counting.  The EVENT_TIMER
            is still enabled and will start counting again if a valid start EVENT 
            is generated.

            STOP_CLEAR_TIMER - Stop the EVENT_TIMER from counting and clear the 
            count value.  The EVENT_TIMER is still enabled and will start counting 
            again if a valid start EVENT is generated.

            STOP_CLEAR_DISABLE_TIMER - Stop the EVENT_TIMER from counting and clear the 
            count value.  The EVENT_TIMER is disabled and will not start counting 
            again if a valid start EVENT is generated.

            STOP_DISABLE_TIMER - Stop the EVENT_TIMER from counting.  The 
            EVENT_TIMER is disabled and will not start counting again if a 
            valid start EVENT is generated.

        BOOL Enable - If set to TRUE the EVENT_TIMER will be enabled.  Only when
        an EVENT_TIMER is enabled will an EVENT start or stop the EVENT_TIMER.

	Returns: 
        EVENT_TIMER * - A pointer to a valid EVENT_TIMER if successful, or
        (EVENT_TIMER*)NULL otherwise.

	Notes:
		- If an EVENT_TIMER is to be deleted later on, then (EVENT_TIMER*)NULL
          should be supplied to the argument EVENT_TIMER *EventTimer and the returned
          pointer EVENT_TIMER* should be held onto.

	See Also:
		- DeleteEventTimer()
*/
EVENT_TIMER *CreateEventTimer   (EVENT_TIMER *EventTimer, EVENT StartEvent, EVENT StopEvent, EVENT_TIMER_START_POLICY TimerStartPolicy, EVENT_TIMER_STOP_POLICY TimerStopPolicy, BOOL Enable

                                #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)
                                    , START_EVENT_CALLBACK StartEventCallback
                                #endif // end of #if (USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER == 1)

                                #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)
                                    , STOP_EVENT_CALLBACK StopEventCallback
                                #endif // end of #if (USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER == 1)

                                );
/*
	OS_RESULT EventTimerSetEnable(EVENT_TIMER *EventTimer, BOOL Enable)

	Description: This either enables or disables an EVENT_TIMER.  An EVENT_TIMER
    will only process EVENTs if it is enabled.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.
 
        BOOL Enable - If TRUE the EVENT_TIMER will be enabled.  If FALSE the
        EVENT_TIMER will be disabled.

	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if the EVENT_TIMER was 
        successfully enabled or disabled.  Otherwise this method returns an error.

	Notes:
		- If the EVENT_TIMER is already in the requested Enable state, this function
          has no effect.
        - When transitioning Enabled states, the current number of timer ticks remain
          unchanged.
        - USING_EVENT_TIMER_SET_ENABLE_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerGetEnable()
*/
OS_RESULT EventTimerSetEnable(EVENT_TIMER *EventTimer, BOOL Enable);

/*
	OS_RESULT EventTimerSetStartEvent(EVENT_TIMER *EventTimer, EVENT StartEvent)

	Description: This method will attempt to modify the start EVENT to the 
    newly specified EVENT.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.
 
        EVENT StartEvent - An EVENT to start the EVENT_TIMER.  Once started, the 
        tick count will begin to increment.

	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if successful.  Otherwise 
        this method returns an error.

	Notes:
		- The StartEvent can be changed at any point in time.
        - The StartEvent cannot be the same as the StopEvent of the EVENT_TIMER.
        - USING_EVENT_TIMER_SET_START_EVENT_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerGetStartEvent(), EventTimerSetStopEvent()
*/
OS_RESULT EventTimerSetStartEvent(EVENT_TIMER *EventTimer, EVENT StartEvent);

/*
	OS_RESULT EventTimerSetStopEvent(EVENT_TIMER *EventTimer, EVENT StopEvent)

	Description: This method will attempt to modify the stop EVENT to the 
    newly specified EVENT.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.
  
        EVENT StopEvent - A valid EVENT which will cause the EVENT_TIMER to stop
        if the EVENT occurs.

	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if successful.  Otherwise 
        this method returns an error.

	Notes:
		- The StopEvent can be changed at any point in time.
        - The StopEvent cannot be the same as the StartEvent of the EVENT_TIMER.
        - USING_EVENT_TIMER_SET_STOP_EVENT_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.
 
	See Also:
		- EventTimerGetStopEvent(), EventTimerSetStartEvent()
*/
OS_RESULT EventTimerSetStopEvent(EVENT_TIMER *EventTimer, EVENT StopEvent);

/*
	OS_RESULT EventTimerSetStartEventCallback(EVENT_TIMER *EventTimer, START_EVENT_CALLBACK StartEventCallback)

	Description: This method will attempt to modify the start EVENT callback
    method to the newly specified method.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

        START_EVENT_CALLBACK StartEventCallback - A valid START_EVENT_CALLBACK or NULL.
        If a valid START_EVENT_CALLBACK is specified, it is called each time an EVENT
        matches the StartEvent of the EVENT_TIMER.  A START_EVENT_CALLBACK is equivalent
        to void(*)(UINT32 CurrentOSTickCount).

	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if successful.  Otherwise 
        this method returns an error.

	Notes:
        - USING_EVENT_TIMER_SET_START_EVENT_CALLBACK_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerGetStartEventCallback(), EventTimerSetStopEventCallback()
*/
OS_RESULT EventTimerSetStartEventCallback(EVENT_TIMER *EventTimer, START_EVENT_CALLBACK StartEventCallback);

/*
	OS_RESULT EventTimerSetStopEventCallback(EVENT_TIMER *EventTimer, STOP_EVENT_CALLBACK StopEventCallback)

	Description: This method will attempt to modify the stop EVENT callback
    method to the newly specified method.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

        STOP_EVENT_CALLBACK StopEventCallback - A valid STOP_EVENT_CALLBACK or NULL.
        If a valid STOP_EVENT_CALLBACK is specified, it is called each time an EVENT
        matches the StopEvent of the EVENT_TIMER.  A STOP_EVENT_CALLBACK is equivalent
        to void(*)(UINT32 CurrentOSTickCount, UINT32 EventTimerCountInTicks, void *EventTimer).
 
   	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if successful.  Otherwise 
        this method returns an error.

	Notes:
        - USING_EVENT_TIMER_SET_STOP_EVENT_CALLBACK_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerGetStopEventCallback(), EventTimerSetStartEventCallback()
*/
OS_RESULT EventTimerSetStopEventCallback(EVENT_TIMER *EventTimer, STOP_EVENT_CALLBACK StopEventCallback);

/*
	OS_RESULT EventTimerSetTimerStartPolicy(EVENT_TIMER *EventTimer, EVENT_TIMER_START_POLICY TimerStartPolicy)

	Description: This method will attempt to modify the EVENT_TIMER_START_POLICY to
    to the newly specified EVENT_TIMER_START_POLICY.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.
  
        EVENT_TIMER_START_POLICY TimerStartPolicy - 

	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if successful.  Otherwise 
        this method returns an error.

	Notes:
		- USING_EVENT_TIMER_SET_EVENT_TIMER_START_POLICY_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerGetTimerStartPolicy()
*/
OS_RESULT EventTimerSetTimerStartPolicy(EVENT_TIMER *EventTimer, EVENT_TIMER_START_POLICY TimerStartPolicy);

/*
	OS_RESULT EventTimerSetTimerStopPolicy(EVENT_TIMER *EventTimer, EVENT_TIMER_STOP_POLICY TimerStopPolicy)

	Description: This method will attempt to modify the EVENT_TIMER_STOP_POLICY to
    to the newly specified EVENT_TIMER_STOP_POLICY.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.
 
        EVENT_TIMER_STOP_POLICY TimerStopPolicy - 

	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if successful.  Otherwise 
        this method returns an error.

	Notes:
		- USING_EVENT_TIMER_SET_EVENT_TIMER_STOP_POLICY_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerGetTimerStopPolicy(), EventTimerSetTimerStartPolicy()
*/
OS_RESULT EventTimerSetTimerStopPolicy(EVENT_TIMER *EventTimer, EVENT_TIMER_STOP_POLICY TimerStopPolicy);

/*
	OS_RESULT EventTimerClearTimer(EVENT_TIMER *EventTimer)

	Description: This method will attempt to clear the tick count of the 
    specified EVENT_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        OS_RESULT - This method will return OS_SUCCESS if successful.  Otherwise 
        this method returns an error.

	Notes:
		- USING_EVENT_TIMER_CLEAR_TIMER_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.
        - This method can be called at any point in time on an EVENT_TIMER.

	See Also:
		- EventTimerGetTicks()
*/
OS_RESULT EventTimerClearTimer(EVENT_TIMER *EventTimer);

/*
	BOOL EventTimerGetEnable(EVENT_TIMER *EventTimer)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        BOOL - TRUE if the EVENT_TIMER is enabled, FALSE otherwise.

	Notes:
		- USING_EVENT_TIMER_GET_ENABLE_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetEnable()
*/
BOOL EventTimerGetEnable(EVENT_TIMER *EventTimer);

/*
	EVENT EventTimerGetStartEvent(EVENT_TIMER *EventTimer)

	Description: This function will return the start EVENT of the EVENT_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        EVENT - This method will return the start EVENT or NO_EVENT if there is
        some type of error.

	Notes:
		- USING_EVENT_TIMER_GET_START_EVENT_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetStartEvent(), EventTimerGetStopEvent()
*/
EVENT EventTimerGetStartEvent(EVENT_TIMER *EventTimer);

/*
	EVENT EventTimerGetStopEvent(EVENT_TIMER *EventTimer)

	Description: This function will return the stop EVENT of the EVENT_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        EVENT - This method will return the stop EVENT or NO_EVENT if there is
        some type of error.

	Notes:
		- USING_EVENT_TIMER_GET_STOP_EVENT_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetStopEvent(), EventTimerGetStartEvent()
*/
EVENT EventTimerGetStopEvent(EVENT_TIMER *EventTimer);

/*
	START_EVENT_CALLBACK EventTimerGetStartEventCallback(EVENT_TIMER *EventTimer)

	Description: This function will return the START_EVENT_CALLBACK in the EVENT_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        START_EVENT_CALLBACK - This method will return the START_EVENT_CALLBACK.  If
        (START_EVENT_CALLBACK)NULL is returned there was either an error or no callback.

	Notes:
		- USING_EVENT_TIMER_GET_START_EVENT_CALLBACK_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetStartEventCallback(), EventTimerGetStopEventCallback()
*/
START_EVENT_CALLBACK EventTimerGetStartEventCallback(EVENT_TIMER *EventTimer);

/*
	STOP_EVENT_CALLBACK EventTimerGetStopEventCallback(EVENT_TIMER *EventTimer)

	Description: This function will return the STOP_EVENT_CALLBACK in the EVENT_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        STOP_EVENT_CALLBACK - This method will return the STOP_EVENT_CALLBACK.  If
        (STOP_EVENT_CALLBACK)NULL is returned there was either an error or no callback.

	Notes:
		- USING_EVENT_TIMER_GET_STOP_EVENT_CALLBACK_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetStopEventCallback(), EventTimerGetStartEventCallback()
*/
STOP_EVENT_CALLBACK EventTimerGetStopEventCallback(EVENT_TIMER *EventTimer);

/*
	EVENT_TIMER_START_POLICY EventTimerGetTimerStartPolicy(EVENT_TIMER *EventTimer)

	Description: This method will return the EVENT_TIMER_START_POLICY of the EVENT_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        EVENT_TIMER_START_POLICY - This method will return the EVENT_TIMER_START_POLICY of the 
        EVENT_TIMER.  If there is an error NUMBER_OF_TIMER_START_POLICIES will be returned.

	Notes:
		- USING_EVENT_TIMER_GET_TIMER_START_POLICY_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetTimerStartPolicy(), EventTimerGetTimerStopPolicy()
*/
EVENT_TIMER_START_POLICY EventTimerGetTimerStartPolicy(EVENT_TIMER *EventTimer);

/*
	EVENT_TIMER_STOP_POLICY EventTimerGetTimerStopPolicy(EVENT_TIMER *EventTimer)

	Description: This method will return the EVENT_TIMER_STOP_POLICY of the EVENT_TIMER.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        EVENT_TIMER_STOP_POLICY - This method will return the EVENT_TIMER_STOP_POLICY of the 
        EVENT_TIMER.  If there is an error NUMBER_OF_TIMER_STOP_POLICIES will be returned.

	Notes:
		- USING_EVENT_TIMER_GET_TIMER_STOP_POLICY_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetTimerStopPolicy(), EventTimerGetTimerStartPolicy()
*/
EVENT_TIMER_STOP_POLICY EventTimerGetTimerStopPolicy(EVENT_TIMER *EventTimer);

/*
	UINT32 EventTimerGetTicks(EVENT_TIMER *EventTimer)

	Description: This method will return how many OS ticks the EVENT_TIMER has 
    accumulated.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        UINT32 - The number of OS ticks the EVENT_TIMER has been running.  A
        zero will be returned if there is an error.

	Notes:
		- USING_EVENT_TIMER_GET_TIMER_TICKS_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.
        - This method can be called at any point.  The EVENT_TIMER does not need
          to be disabled to call this method.

	See Also:
		- EventTimerClearTimer(), EventTimerGetMilliseconds()
*/
UINT32 EventTimerGetTicks(EVENT_TIMER *EventTimer);

/*
	UINT32 EventTimerGetMilliseconds(EVENT_TIMER *EventTimer)

	Description: This method will take how many OS ticks the EVENT_TIMER has
    accumulated and convert them into milliseconds.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        UINT32 - The number of milliseconds the EVENT_TIMER has been running.  A
        value of zero will be returned if there is an error.

	Notes:
		- USING_EVENT_TIMER_GET_TIMER_MILLISECONDS_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.
        - This method can be called at any point.  The EVENT_TIMER does not need
          to be disabled to call this method.

	See Also:
		- EventTimerGetTicks(), EventTimerClearTimer()
*/
UINT32 EventTimerGetMilliseconds(EVENT_TIMER *EventTimer);

/*
	BOOL EventTimerIsRunning(EVENT_TIMER *EventTimer)

	Description: This method checks and returns the value indicating if the EVENT_TIMER
    is currently running.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

	Returns: 
        BOOL - This method will return TRUE if the EVENT_TIMER is running, FALSE otherwise.

	Notes:
		- USING_EVENT_TIMER_GET_TIMER_IS_RUNNING_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- EventTimerSetEnable(), EventTimerGetEnable()
*/
BOOL EventTimerIsRunning(EVENT_TIMER *EventTimer);

/*
	OS_RESULT DeleteEventTimer(EVENT_TIMER *EventTimer)

	Description: This method attempts to delete the specified EVENT_TIMER.  When
    deleted the memory allocated to create the EVENT_TIMER will be released.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER* to free the data pointed
        to in the OS heap.

	Returns: OS_SUCCESS if successful, an error code otherwise.		

	Notes:
		- An EVENT_TIMER should only be deleted if the argument EVENT_TIMER *EventTimer
          of the method CreateEventTimer() was set to (EVENT_TIMER*)NULL when the
          EVENT_TIMER was created.
        - USING_EVENT_TIMER_DELETE_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- CreateEventTimer()
*/
OS_RESULT DeleteEventTimer(EVENT_TIMER *EventTimer);

/*
	BOOL EventTimerStartCallbackAction(EVENT_TIMER *EventTimer, EVENT_TIMER_START_POLICY StartAction)

	Description: This method provides a means for the CALLBACK_ON_EACH_START_EVENT 
    to determine when and how to start the timer.  This method is designed to 
    only be called from a valid START_EVENT_CALLBACK method.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

        EVENT_TIMER_START_POLICY StopAction - A valid enum found within EVENT_TIMER_START_POLICY.
        The value CALLBACK_ON_EACH_START_EVENT is not valid for this function.

	Returns: 
        BOOL - TRUE if the operation was successful, FALSE otherwise.

	Notes:
		- Only call this method if you want to start the EVENT_TIMER.  This allows
          multiple EVENTs of the same type to trigger before the EVENT_TIMER is 
          started by the user based on some type of conditional situation.
        - USING_EVENT_TIMER_START_CALLBACK_ACTION_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.

	See Also:
		- None
*/
BOOL EventTimerStartCallbackAction(EVENT_TIMER *EventTimer, EVENT_TIMER_START_POLICY StartAction);

/*
	BOOL EventTimerStopCallbackAction(EVENT_TIMER *EventTimer, EVENT_TIMER_STOP_POLICY StopAction)

	Description: This method provides a means for the CALLBACK_ON_EACH_STOP_EVENT 
    to determine when and how to stop the timer.  This method is designed to 
    only be called from a valid STOP_EVENT_CALLBACK method.

	Blocking: No

	User Callable: Yes

	Arguments:
		EVENT_TIMER *EventTimer - A valid EVENT_TIMER created with the function
        CreateEventTimer.

        EVENT_TIMER_STOP_POLICY StopAction - A valid enum found within EVENT_TIMER_STOP_POLICY.
        The value CALLBACK_ON_EACH_STOP_EVENT is not valid for this function.

	Returns: 
        BOOL - TRUE if the operation was successful, FALSE otherwise.

	Notes:
		- Only call this method if you want to stop the EVENT_TIMER.  This allows
          multiple EVENTs of the same type to trigger before the EVENT_TIMER is 
          stopped by the user based on some type of conditional situation.
        - USING_EVENT_TIMER_STOP_CALLBACK_ACTION_METHOD inside of RTOSConfig.h must be set 
          to a 1 to use this method.
 
	See Also:
		- None
*/
BOOL EventTimerStopCallbackAction(EVENT_TIMER *EventTimer, EVENT_TIMER_STOP_POLICY StopAction);

#endif // end of #ifndef EVENT_TIMERS_H
