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

/*
	EVENTS Description:
		An EVENT can be used to signal TASK's that a particular external/internal or user generated EVENT
		has occurred.  This can be useful for TASK synchronization and is also the basis of event timers.

		A TASK can block on an EVENT and then be placed in the CPU scheduler once the EVENT occurs.  An
		EVENT_TIMER can start and stop a timer based on EVENTS.  This is useful for catching EVENTs that 
		a TASK would miss if it got swapped out.

		All non user EVENTs also have a callback which can be enabled in RTOSConfig.h.
*/

#ifndef EVENTS_H
	#define EVENTS_H

#include "RTOSConfig.h"
#include "../Kernel/Kernel.h"

#if (USING_EVENTS != 1)
	#error "USING_EVENTS must be defined as a 1 in RTOSConfig.h to use EVENTs"
#endif // end of #if (USING_EVENTS != 1)

typedef enum
{
	// we use this here to start zero in the enumeration
	NO_EVENT = 0,

	#if(USING_EXT_INT_0_EVENT == 1)
		EXTERNAL_INTERRUPT_0_EVENT,
	#endif // end of #if(USING_EXT_INT_0_EVENT == 1)

	#if(USING_EXT_INT_1_EVENT == 1)
		EXTERNAL_INTERRUPT_1_EVENT,
	#endif // end of #if(USING_EXT_INT_1_EVENT == 1)

	#if(USING_EXT_INT_2_EVENT == 1)
		EXTERNAL_INTERRUPT_2_EVENT,
	#endif // end of #if(USING_EXT_INT_2_EVENT == 1)

	#if(USING_EXT_INT_3_EVENT == 1)
		EXTERNAL_INTERRUPT_3_EVENT,
	#endif // end of #if(USING_EXT_INT_3_EVENT == 1)

	#if(USING_EXT_INT_4_EVENT == 1)
		EXTERNAL_INTERRUPT_4_EVENT,
	#endif // end of #if(USING_EXT_INT_4_EVENT == 1)

	#if(USING_CN_INT_EVENT == 1)
		CHANGE_NOTIFICATION_INTERRUPT_EVENT,
	#endif // end of #if(USING_CN_INT_EVENT == 1)

	#if(USING_TIMER_1_EVENT == 1)
		TIMER_1_EVENT,
	#endif // end of #if(USING_TIMER_1_EVENT == 1)

	#if(USING_TIMER_2_EVENT == 1)
		TIMER_2_EVENT,
	#endif // end of #if(USING_TIMER_2_EVENT == 1)

	#if(USING_TIMER_3_EVENT == 1)
		TIMER_3_EVENT,
	#endif // end of #if(USING_TIMER_3_EVENT == 1)

	#if(USING_TIMER_4_EVENT == 1)
		TIMER_4_EVENT,
	#endif // end of #if(USING_TIMER_4_EVENT == 1)

	#if(USING_TIMER_5_EVENT == 1)
		TIMER_5_EVENT,
	#endif // end of #if(USING_TIMER_5_EVENT == 1)

	#if(USING_ADC_1_EVENT == 1)
		ADC_1_EVENT,
	#endif // end of #if(USING_ADC_1_EVENT == 1)

	#if(USING_RTCC_ALARM_EVENT == 1)
		RTCC_ALARM_EVENT,
	#endif // end of #if(USING_RTCC_ALARM_EVENT == 1)

    #if(USING_EXT_OSC_FAILED_EVENT == 1)
		EXTERNAL_OSCILLATOR_FAILED_EVENT,
	#endif // end of #if(USING_EXT_OSC_FAILED_EVENT == 1)

	#if(USING_UART_1_TX_DONE_EVENT == 1)
		UART_1_TX_DONE_EVENT,
	#endif // end of #if(USING_UART_1_TX_DONE_EVENT == 1)

	#if(USING_UART_2_TX_DONE_EVENT == 1)
		UART_2_TX_DONE_EVENT,
	#endif // end of #if(USING_UART_2_TX_DONE_EVENT == 1)

	#if(USING_UART_3_TX_DONE_EVENT == 1)
		UART_3_TX_DONE_EVENT,
	#endif // end of #if(USING_UART_3_TX_DONE_EVENT == 1)

	#if(USING_UART_4_TX_DONE_EVENT == 1)
		UART_4_TX_DONE_EVENT,
	#endif // end of #if(USING_UART_4_TX_DONE_EVENT == 1)

	#if(USING_UART_5_TX_DONE_EVENT == 1)
		UART_5_TX_DONE_EVENT,
	#endif // end of #if(USING_UART_5_TX_DONE_EVENT == 1)

	#if(USING_UART_6_TX_DONE_EVENT == 1)
		UART_6_TX_DONE_EVENT,
	#endif // end of #if(USING_UART_6_TX_DONE_EVENT == 1)

	#if(USING_UART_1_RX_DONE_EVENT == 1)
		UART_1_RX_DONE_EVENT,
	#endif // end of #if(USING_UART_1_RX_DONE_EVENT == 1)

	#if(USING_UART_2_RX_DONE_EVENT == 1)
		UART_2_RX_DONE_EVENT,
	#endif // end of #if(USING_UART_2_RX_DONE_EVENT == 1)

	#if(USING_UART_3_RX_DONE_EVENT == 1)
		UART_3_RX_DONE_EVENT,
	#endif // end of #if(USING_UART_3_RX_DONE_EVENT == 1)

	#if(USING_UART_4_RX_DONE_EVENT == 1)
		UART_4_RX_DONE_EVENT,
	#endif // end of #if(USING_UART_4_RX_DONE_EVENT == 1)

	#if(USING_UART_5_RX_DONE_EVENT == 1)
		UART_5_RX_DONE_EVENT,
	#endif // end of #if(USING_UART_5_RX_DONE_EVENT == 1)

	#if(USING_UART_6_RX_DONE_EVENT == 1)
		UART_6_RX_DONE_EVENT,
	#endif // end of #if(USING_UART_6_RX_DONE_EVENT == 1)

	#if(USING_UART_1_ERROR_EVENT == 1)
		UART_1_ERROR_EVENT,
	#endif // end of #if(USING_UART_1_ERROR_EVENT == 1)

	#if(USING_UART_2_ERROR_EVENT == 1)
		UART_2_ERROR_EVENT,
	#endif // end of #if(USING_UART_2_ERROR_EVENT == 1)

	#if(USING_UART_3_ERROR_EVENT == 1)
		UART_3_ERROR_EVENT,
	#endif // end of #if(USING_UART_3_ERROR_EVENT == 1)

	#if(USING_UART_4_ERROR_EVENT == 1)
		UART_4_ERROR_EVENT,
	#endif // end of #if(USING_UART_4_ERROR_EVENT == 1)

	#if(USING_UART_5_ERROR_EVENT == 1)
		UART_5_ERROR_EVENT,
	#endif // end of #if(USING_UART_5_ERROR_EVENT == 1)

	#if(USING_UART_6_ERROR_EVENT == 1)
		UART_6_ERROR_EVENT,
	#endif // end of #if(USING_UART_6_ERROR_EVENT == 1)

    #if(USING_CAN_1_TX_EVENT == 1)
		CAN_1_TX_DONE_EVENT,
	#endif // end of #if(USING_CAN_1_TX_EVENT == 1)

	#if(USING_CAN_2_TX_EVENT == 1)
		CAN_2_TX_DONE_EVENT,
	#endif // end of #if(USING_CAN_2_TX_EVENT == 1)

	#if(USING_CAN_1_RX_EVENT == 1)
		CAN_1_RX_DONE_EVENT,
	#endif // end of #if(USING_CAN_1_RX_EVENT == 1)

	#if(USING_CAN_2_RX_EVENT == 1)
		CAN_2_RX_DONE_EVENT,
	#endif // end of #if(USING_CAN_2_RX_EVENT == 1)

	#if(USING_CAN_1_ERROR_EVENT == 1)
		CAN_1_ERROR_EVENT,
	#endif // end of #if(USING_CAN_1_ERROR_EVENT == 1)

	#if(USING_CAN_2_ERROR_EVENT == 1)
		CAN_2_ERROR_EVENT,
	#endif // end of #if(USING_CAN_2_ERROR_EVENT == 1)

	#if(USING_MEMORY_WARNING_EVENT == 1)
		MEMORY_WARNING_EVENT,
	#endif // end of #if(USING_MEMORY_WARNING_EVENT == 1)

	#if(USING_MEMORY_WARNING_CLEAR_EVENT == 1)
		MEMORY_WARNING_CLEAR_EVENT,
	#endif // end of #if(USING_MEMORY_WARNING_CLEAR_EVENT == 1)

	#if(USING_CPU_EXCEPTION_RAISED_EVENT == 1)
		CPU_EXCEPTION_RAISED_EVENT,
	#endif // end of #if(USING_CPU_EXCEPTION_RAISED_EVENT == 1)

    #if (USING_IO_BUFFER_OVERFLOW_EVENT == 1)
        IO_BUFFER_OVERFLOW_EVENT,
    #endif // end of #if (USING_IO_BUFFER_OVERFLOW_EVENT == 1)

	#if(USING_USER_1_EVENT == 1)
		USER_1_EVENT,
	#endif // end of #if(USING_USER_1_EVENT == 1)

	#if(USING_USER_2_EVENT == 1)
		USER_2_EVENT,
	#endif // end of #if(USING_USER_2_EVENT == 1)

	#if(USING_USER_3_EVENT == 1)
		USER_3_EVENT,
	#endif // end of #if(USING_USER_3_EVENT == 1)

	#if(USING_USER_4_EVENT == 1)
		USER_4_EVENT,
	#endif // end of #if(USING_USER_4_EVENT == 1)

	#if(USING_USER_5_EVENT == 1)
		USER_5_EVENT,
	#endif // end of #if(USING_USER_5_EVENT == 1)

	#if(USING_USER_6_EVENT == 1)
		USER_6_EVENT,
	#endif // end of #if(USING_USER_6_EVENT == 1)

	#if(USING_USER_7_EVENT == 1)
		USER_7_EVENT,
	#endif // end of #if(USING_USER_7_EVENT == 1)

	#if(USING_USER_8_EVENT == 1)
		USER_8_EVENT,
	#endif // end of #if(USING_USER_8_EVENT == 1)

	#if(USING_USER_9_EVENT == 1)
		USER_9_EVENT,
	#endif // end of #if(USING_USER_9_EVENT == 1)

	#if(USING_USER_10_EVENT == 1)
		USER_10_EVENT,
	#endif // end of #if(USING_USER_10_EVENT == 1)

	NUMBER_OF_EVENTS
}EVENT;

/*
	BOOL OS_InitEventsLib(void)

	Description: This method initializes the event manager in the OS.  This method
	should not be called by the user.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
		BOOL - TRUE if the event manager was initialized successfully, FALSE otherwise.

	Notes:
		- This is called only by the Kernel.

	See Also:
		- None
*/
BOOL OS_InitEventsLib(void);

/*
	BOOL OS_RaiseEvent(EVENT Event)

	Description: This method raises the specified event within the OS.  This
	method should not be called by the user.

	Blocking: No

	User Callable: No

	Arguments:
		EVENT Event - A valid EVENT to raise within the OS.

	Returns: 
		BOOL - TRUE is a TASK with a higher priority than the currently running
		TASK was made ready due to the event, FALSE otherwise.

	Notes:
		- None

	See Also:
		- None
*/
BOOL OS_RaiseEvent(EVENT Event);

/*
	OS_RESULT RaiseEvent(EVENT Event)

	Description: This method raises the specified event within the OS.  Only USER
	events should be passed to this method.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		EVENT Event - A valid User EVENT to raise within the OS.  This should only
		have one of the USER EVENTs specified.

	Returns: 
		OS_RESULT - OS_SUCCESS if the event was successfully raised, another value if 
		the operation failed.

	Notes:
		- None

	See Also:
		- WaitOnEvent()
*/
OS_RESULT RaiseEvent(EVENT Event);

/*
	OS_RESULT WaitOnEvent(EVENT Event, UINT32 TimeoutInTicks)

	Description: This method blocks the current TASK until the specified event 
	occurs.  There is also an optional timeout.

	Blocking: Yes

	User Callable: Yes

	Arguments:
		EVENT Event - A valid User EVENT to wait to occur.

		UINT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the event to occur.  The timeout is in OS timer ticks.

	Returns: OS_SUCCESS if the event was successfully encountered within the
	specified timeout, or another OS_RESULT value otherwise.

	Notes:
 		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- RaiseEvent()
*/
OS_RESULT WaitOnEvent(	EVENT Event

						#if (USING_TASK_DELAY_TICKS_METHOD == 1)
							, UINT32 TimeoutInTicks
						#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

						);

#endif // end of #ifndef EVENTS_H
