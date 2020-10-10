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

/*
	OS_EventCallbacks Description:
        This file contains all the callback functions an EVENT can call.  These are left to 
        be implemented by the user.  These functions should never call a method which may
        block.  It is safe to call a method which has the TLA ISR in its name from any of the 
        callback methods in this file.
*/

#ifndef OS_EVENT_CALLBACKS_H
    #define OS_EVENT_CALLBACKS_H

/*
	void ExternalInterrupt0Callback(void)

	Description: This method is called when the external interrupt 0 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_EXT_INT_0_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- ExternalInterrupt1Callback(), ExternalInterrupt2Callback(), ExternalInterrupt3Callback(),
          ExternalInterrupt4Callback()
*/
void ExternalInterrupt0Callback(void);

/*
	void ExternalInterrupt1Callback(void)

	Description: This method is called when the external interrupt 1 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_EXT_INT_1_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- ExternalInterrupt0Callback(), ExternalInterrupt2Callback(), ExternalInterrupt3Callback(),
          ExternalInterrupt4Callback()
*/
void ExternalInterrupt1Callback(void);

/*
	void ExternalInterrupt2Callback(void)

	Description: This method is called when the external interrupt 2 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_EXT_INT_2_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- ExternalInterrupt0Callback(), ExternalInterrupt1Callback(), ExternalInterrupt3Callback(),
          ExternalInterrupt4Callback()
*/
void ExternalInterrupt2Callback(void);

/*
	void ExternalInterrupt3Callback(void)

	Description: This method is called when the external interrupt 3 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_EXT_INT_3_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- ExternalInterrupt0Callback(), ExternalInterrupt1Callback(), ExternalInterrupt2Callback(),
          ExternalInterrupt4Callback()
*/
void ExternalInterrupt3Callback(void);

/*
	void ExternalInterrupt4Callback(void)

	Description: This method is called when the external interrupt 4 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_EXT_INT_4_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- ExternalInterrupt0Callback(), ExternalInterrupt1Callback(), ExternalInterrupt2Callback(),
          ExternalInterrupt3Callback()
*/
void ExternalInterrupt4Callback(void);

/*
	void ChangeNotificationInterruptCallback(void)

	Description: This method is called when the change notification interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_CN_INT_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void ChangeNotificationInterruptCallback(void);

/*
	void Timer1InterruptCallback(void)

	Description: This method is called when the timer 1 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_TIMER_1_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- Timer2InterruptCallback(), Timer3InterruptCallback(), Timer4InterruptCallback()
          Timer5InterruptCallback()
*/
void Timer1InterruptCallback(void);

/*
	void Timer2InterruptCallback(void)

	Description: This method is called when the timer 2 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_TIMER_2_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- Timer1InterruptCallback(), Timer3InterruptCallback(), Timer4InterruptCallback()
          Timer5InterruptCallback()
*/
void Timer2InterruptCallback(void);

/*
	void Timer3InterruptCallback(void)

	Description: This method is called when the timer 3 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_TIMER_3_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- Timer1InterruptCallback(), Timer2InterruptCallback(), Timer4InterruptCallback()
          Timer5InterruptCallback()
*/
void Timer3InterruptCallback(void);

/*
	void Timer4InterruptCallback(void)

	Description: This method is called when the timer 4 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_TIMER_4_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- Timer1InterruptCallback(), Timer2InterruptCallback(), Timer3InterruptCallback()
          Timer5InterruptCallback()
*/
void Timer4InterruptCallback(void);

/*
	void Timer5InterruptCallback(void)

	Description: This method is called when the timer 5 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_TIMER_5_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- Timer1InterruptCallback(), Timer2InterruptCallback(), Timer3InterruptCallback()
          Timer4InterruptCallback()
*/
void Timer5InterruptCallback(void);

/*
	void ExternalOscillatorFailedInterruptCallback(void)

	Description: This method is called when the external oscillator failed interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_EXT_OSC_FAILED_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void ExternalOscillatorFailedInterruptCallback(void);

#endif // end of #ifndef OS_EVENT_CALLBACKS_H
