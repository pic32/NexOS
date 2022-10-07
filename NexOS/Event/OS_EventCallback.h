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
	OS_EventCallbacks Description:
        This file contains all the callback functions an EVENT can call.  These are left to 
        be implemented by the user.  These functions should never call a method which may
        block.  It is safe to call a method which has the TLA ISR in its name from any of the 
        callback methods in this file.
*/

#ifndef OS_EVENT_CALLBACKS_H
    #define OS_EVENT_CALLBACKS_H

#if (USING_UART_1_ERROR_CALLBACK == 1) || (USING_UART_2_ERROR_CALLBACK == 1) || (USING_UART_3_ERROR_CALLBACK == 1) || (USING_UART_4_ERROR_CALLBACK == 1) || (USING_UART_5_ERROR_CALLBACK == 1) || (USING_UART_6_ERROR_CALLBACK == 1)
    #if (USING_UART_1_IO_BUFFER == 1) || (USING_UART_2_IO_BUFFER == 1) || (USING_UART_3_IO_BUFFER == 1) || (USING_UART_4_IO_BUFFER == 1) || (USING_UART_5_IO_BUFFER == 1) || (USING_UART_6_IO_BUFFER == 1)
        #include "../NexOS/IOBuffer/IOBuffer.h"
    #endif // end of #if (USING_UART_1_IO_BUFFER == 1) || (USING_UART_2_IO_BUFFER == 1) || (USING_UART_3_IO_BUFFER == 1) || (USING_UART_4_IO_BUFFER == 1) || (USING_UART_5_IO_BUFFER == 1) || (USING_UART_6_IO_BUFFER == 1)
#endif // end of #if (USING_UART_1_ERROR_CALLBACK == 1) || (USING_UART_2_ERROR_CALLBACK == 1) || (USING_UART_3_ERROR_CALLBACK == 1) || (USING_UART_4_ERROR_CALLBACK == 1) || (USING_UART_5_ERROR_CALLBACK == 1) || (USING_UART_6_ERROR_CALLBACK == 1)

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
	void ADC1InterruptCallback(UINT16 ADCValue)

	Description: This method is called when the ADC 1 interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		UINT16 ADCValue - The value read from the ADC port.

	Returns: 
        None

	Notes:
		- USING_ADC_1_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if(USING_ADC_1_IO_BUFFER == 1)
    void ADC1InterruptCallback(UINT16 ADCValue);
#else
    void ADC1InterruptCallback(void);
#endif // end of #if(USING_ADC_1_IO_BUFFER == 1)

/*
	void RTCCInterruptCallback(void)

	Description: This method is called when the RTCC interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_RTCC_ALARM_EVENT inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void RTCCInterruptCallback(void);

/*
	void UART1InterruptTXCallback(void)

	Description: This method is called when the UART 1 TX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_UART_1_TX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void UART1InterruptTXCallback(void);

/*
	void UART2InterruptTXCallback(void)

	Description: This method is called when the UART 2 TX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_UART_2_TX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void UART2InterruptTXCallback(void);

/*
	void UART3InterruptTXCallback(void)

	Description: This method is called when the UART 3 TX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_UART_3_TX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void UART3InterruptTXCallback(void);

/*
	void UART4InterruptTXCallback(void)

	Description: This method is called when the UART 4 TX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_UART_4_TX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void UART4InterruptTXCallback(void);

/*
	void UART5InterruptTXCallback(void)

	Description: This method is called when the UART 5 TX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_UART_5_TX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void UART5InterruptTXCallback(void);

/*
	void UART6InterruptTXCallback(void)

	Description: This method is called when the UART 6 TX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- USING_UART_6_TX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
void UART6InterruptTXCallback(void);

/*
	void UART1InterruptRXCallback(BYTE Data)

	Description: This method is called when the UART 1 RX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
        BYTE Data - The value read from the UART port.

	Returns: 
        None

	Notes:
		- USING_UART_1_RX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_1_IO_BUFFER == 1)
    void UART1InterruptRXCallback(BYTE Data);
#else
    void UART1InterruptRXCallback(void);
#endif // end of #if (USING_UART_1_IO_BUFFER == 1)

/*
	void UART2InterruptRXCallback(BYTE Data)

	Description: This method is called when the UART 2 RX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		BYTE Data - The value read from the UART port.

	Returns: 
        None

	Notes:
		- USING_UART_2_RX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_2_IO_BUFFER == 1)
    void UART2InterruptRXCallback(BYTE Data);
#else
    void UART2InterruptRXCallback(void);
#endif // end of #if (USING_UART_2_IO_BUFFER == 1)

/*
	void UART3InterruptRXCallback(BYTE Data)

	Description: This method is called when the UART 3 RX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		BYTE Data - The value read from the UART port.

	Returns: 
        None

	Notes:
		- USING_UART_3_RX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_3_IO_BUFFER == 1)
    void UART3InterruptRXCallback(BYTE Data);
#else
    void UART3InterruptRXCallback(void);
#endif // end of #if (USING_UART_3_IO_BUFFER == 1)

/*
	void UART4InterruptRXCallback(BYTE Data)

	Description: This method is called when the UART 4 RX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		BYTE Data - The value read from the UART port.

	Returns: 
        None

	Notes:
		- USING_UART_4_RX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_4_IO_BUFFER == 1)
    void UART4InterruptRXCallback(BYTE Data);
#else
    void UART4InterruptRXCallback(void);
#endif // end of #if (USING_UART_4_IO_BUFFER == 1)

/*
	void UART5InterruptRXCallback(BYTE Data)

	Description: This method is called when the UART 5 RX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		BYTE Data - The value read from the UART port.

	Returns: 
        None

	Notes:
		- USING_UART_5_RX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_5_IO_BUFFER == 1)
    void UART5InterruptRXCallback(BYTE Data);
#else
    void UART5InterruptRXCallback(void);
#endif // end of #if (USING_UART_5_IO_BUFFER == 1)

/*
	void UART6InterruptRXCallback(BYTE Data)

	Description: This method is called when the UART 6 RX interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		BYTE Data - The value read from the UART port.

	Returns: 
        None

	Notes:
		- USING_UART_6_RX_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_6_IO_BUFFER == 1)
    void UART6InterruptRXCallback(BYTE Data);
#else
    void UART6InterruptRXCallback(void);
#endif // end of #if (USING_UART_6_IO_BUFFER == 1)

/*
	void UART1InterruptErrorCallback(UART_ERROR Error)

	Description: This method is called when the UART 1 error interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		UART_ERROR Error - The error type the UART experienced.

	Returns: 
        None

	Notes:
		- USING_UART_1_ERROR_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_1_ERROR_CALLBACK == 1)
    #if (USING_UART_1_IO_BUFFER == 1)
        void UART1InterruptErrorCallback(UART_ERROR Error);
    #else
        void UART1InterruptErrorCallback(void);
    #endif // end of #if (USING_UART_1_IO_BUFFER == 1)
#endif // end of #if (USING_UART_1_ERROR_CALLBACK == 1)

/*
	void UART2InterruptErrorCallback(UART_ERROR Error)

	Description: This method is called when the UART 2 error interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		UART_ERROR Error - The error type the UART experienced.

	Returns: 
        None

	Notes:
		- USING_UART_2_ERROR_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_2_ERROR_CALLBACK == 1)
    #if (USING_UART_2_IO_BUFFER == 1)
        void UART2InterruptErrorCallback(UART_ERROR Error);
    #else
        void UART2InterruptErrorCallback(void);
    #endif // end of #if (USING_UART_2_IO_BUFFER == 1)
#endif // end of #if (USING_UART_2_ERROR_CALLBACK == 1)

/*
	void UART3InterruptErrorCallback(UART_ERROR Error)

	Description: This method is called when the UART 3 error interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		UART_ERROR Error - The error type the UART experienced.

	Returns: 
        None

	Notes:
		- USING_UART_3_ERROR_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_3_ERROR_CALLBACK == 1)
    #if (USING_UART_3_IO_BUFFER == 1)
        void UART3InterruptErrorCallback(UART_ERROR Error);
    #else
        void UART3InterruptErrorCallback(void);
    #endif // end of #if (USING_UART_3_IO_BUFFER == 1)
#endif // end of #if (USING_UART_3_ERROR_CALLBACK == 1)

/*
	void UART4InterruptErrorCallback(UART_ERROR Error)

	Description: This method is called when the UART 4 error interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		UART_ERROR Error - The error type the UART experienced.

	Returns: 
        None

	Notes:
		- USING_UART_4_ERROR_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_4_ERROR_CALLBACK == 1)
    #if (USING_UART_4_IO_BUFFER == 1)
        void UART4InterruptErrorCallback(UART_ERROR Error);
    #else
        void UART4InterruptErrorCallback(void);
    #endif // end of #if (USING_UART_4_IO_BUFFER == 1)
#endif // end of #if (USING_UART_4_ERROR_CALLBACK == 1)

/*
	void UART5InterruptErrorCallback(UART_ERROR Error)

	Description: This method is called when the UART 5 error interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		UART_ERROR Error - The error type the UART experienced.

	Returns: 
        None

	Notes:
		- USING_UART_5_ERROR_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_5_ERROR_CALLBACK == 1)
    #if (USING_UART_5_IO_BUFFER == 1)
        void UART5InterruptErrorCallback(UART_ERROR Error);
    #else
        void UART5InterruptErrorCallback(void);
    #endif // end of #if (USING_UART_5_IO_BUFFER == 1)
#endif // end of #if (USING_UART_5_ERROR_CALLBACK == 1)

/*
	void UART6InterruptErrorCallback(UART_ERROR Error)

	Description: This method is called when the UART 6 error interrupt handler
    is triggered.  It is a means to allow the user a callback when this interrupt occurs.

	Blocking: No

	User Callable: No

	Arguments:
		UART_ERROR Error - The error type the UART experienced.

	Returns: 
        None

	Notes:
		- USING_UART_6_ERROR_CALLBACK inside of RTOSConfig.h must be defined as a 1 to
          use this callback feature.

	See Also:
		- None
*/
#if (USING_UART_6_ERROR_CALLBACK == 1)
    #if (USING_UART_6_IO_BUFFER == 1)
        void UART6InterruptErrorCallback(UART_ERROR Error);
    #else
        void UART6InterruptErrorCallback(void);
    #endif // end of #if (USING_UART_6_IO_BUFFER == 1)
#endif // end of #if (USING_UART_6_ERROR_CALLBACK == 1)

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
