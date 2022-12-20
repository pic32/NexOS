/*
    NexOS Kernel Version v1.02.00
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

#include "GenericTypes.h"

#include "RTOSConfig.h"

/*
 * This intent of this file is to show the user the different callbacks, their
 * names, and the arguments they take depending on the configuration of the 
 * RTOS.  This file does not need to be included in the build if the user decides
 * to implement the callbacks in their own files.  Otherwise, if callbacks are
 * used, include this file to avoid linking errors and implement your callback
 * code in each desired function.
 */

#if (USING_EXT_INT_0_CALLBACK == 1)
    void ExternalInterrupt0Callback(void)
    {

    }
#endif // end of #if (USING_EXT_INT_0_CALLBACK == 1)

#if (USING_EXT_INT_1_CALLBACK == 1)
    void ExternalInterrupt1Callback(void)
    {

    }
#endif // end of #if (USING_EXT_INT_1_CALLBACK == 1)
    
#if (USING_EXT_INT_2_CALLBACK == 1)
    void ExternalInterrupt2Callback(void)
    {

    }
#endif // end of #if (USING_EXT_INT_2_CALLBACK == 1)
    
#if (USING_EXT_INT_3_CALLBACK == 1)
    void ExternalInterrupt3Callback(void)
    {

    }
#endif // end of #if (USING_EXT_INT_3_CALLBACK == 1)
    
#if (USING_EXT_INT_4_CALLBACK == 1)
    void ExternalInterrupt4Callback(void)
    {

    }
#endif // end of #if (USING_EXT_INT_4_CALLBACK == 1)

#if (USING_CN_INT_CALLBACK == 1)
    void ChangeNotificationInterruptCallback(void)
    {

    }
#endif // end of #if (USING_CN_INT_CALLBACK == 1)
    
#if (USING_TIMER_1_CALLBACK == 1)
    void Timer1InterruptCallback(void)
    {

    }
#endif // end of #if (USING_TIMER_1_CALLBACK == 1)

#if (USING_TIMER_2_CALLBACK == 1)
    void Timer2InterruptCallback(void)
    {

    }
#endif // end of #if (USING_TIMER_2_CALLBACK == 1)

#if (USING_TIMER_3_CALLBACK == 1)
    void Timer3InterruptCallback(void)
    {

    }
#endif // end of #if (USING_TIMER_3_CALLBACK == 1)

#if (USING_TIMER_4_CALLBACK == 1)
    void Timer4InterruptCallback(void)
    {

    }
#endif // end of #if (USING_TIMER_4_CALLBACK == 1)

#if (USING_TIMER_5_CALLBACK == 1)
    void Timer5InterruptCallback(void)
    {

    }
#endif // end of #if (USING_TIMER_5_CALLBACK == 1)

#if (USING_ADC_1_CALLBACK == 1)
    #if(USING_ADC_1_IO_BUFFER == 1)
        void ADC1InterruptCallback(UINT16 ADCValue)
        {
            
        }
    #else
        void ADC1InterruptCallback(void)
        {

        }
    #endif // end of #if(USING_ADC_1_IO_BUFFER == 1)
#endif // end of #if (USING_ADC_1_CALLBACK == 1)
    
#if(USING_RTCC_ALARM_CALLBACK == 1)
    void RTCCAlarmInterruptCallback(void)
    {
        
    }
#endif // end of #if (USING_RTCC_ALARM_CALLBACK == 1)
    
#if (USING_EXT_OSC_FAILED_CALLBACK == 1)
    void ExternalOscillatorFailedInterruptCallback(void)
    {

    }
#endif // end of #if (USING_EXT_OSC_FAILED_CALLBACK == 1)

#if (USING_UART_1_TX_CALLBACK == 1)
    #if(USING_UART_1_IO_BUFFER == 1)
        void UART1InterruptTXCallback(BYTE Data)
        {

        }
    #else
        void UART1InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_1_IO_BUFFER == 1)
#endif // end of #if (USING_UART_1_TX_CALLBACK == 1)

#if (USING_UART_2_TX_CALLBACK == 1)
    #if(USING_UART_2_IO_BUFFER == 1)
        void UART2InterruptTXCallback(BYTE Data)
        {

        }
    #else
        void UART2InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_2_IO_BUFFER == 1)
#endif // end of #if (USING_UART_2_TX_CALLBACK == 1)

#if (USING_UART_3_TX_CALLBACK == 1)
    #if(USING_UART_3_IO_BUFFER == 1)
        void UART3InterruptTXCallback(BYTE Data)
        {

        }
    #else
        void UART3InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_3_IO_BUFFER == 1)
#endif // end of #if (USING_UART_3_TX_CALLBACK == 1)

#if (USING_UART_4_TX_CALLBACK == 1)
    #if(USING_UART_4_IO_BUFFER == 1)
        void UART4InterruptTXCallback(BYTE Data)
        {

        }
    #else
        void UART4InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_4_IO_BUFFER == 1)
#endif // end of #if (USING_UART_4_TX_CALLBACK == 1)

#if (USING_UART_5_TX_CALLBACK == 1)
    #if(USING_UART_5_IO_BUFFER == 1)
        void UART5InterruptTXCallback(BYTE Data)
        {

        }
    #else
        void UART5InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_5_IO_BUFFER == 1)
#endif // end of #if (USING_UART_5_TX_CALLBACK == 1)

#if (USING_UART_6_TX_CALLBACK == 1)
    #if(USING_UART_6_IO_BUFFER == 1)
        void UART6InterruptTXCallback(BYTE Data)
        {

        }
    #else
        void UART6InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_6_IO_BUFFER == 1)
#endif // end of #if (USING_UART_6_TX_CALLBACK == 1)

#if (USING_UART_1_RX_CALLBACK == 1)
    #if(USING_UART_1_IO_BUFFER == 1)
        void UART1InterruptRXCallback(BYTE Data)
        {

        }
    #else
        void UART1InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_1_IO_BUFFER == 1)
#endif // end of #if (USING_UART_1_RX_CALLBACK == 1)

#if (USING_UART_2_RX_CALLBACK == 1)
    #if(USING_UART_2_IO_BUFFER == 1)
        void UART2InterruptRXCallback(BYTE Data)
        {

        }
    #else
        void UART2InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_2_IO_BUFFER == 1)
#endif // end of #if (USING_UART_2_RX_CALLBACK == 1)

#if (USING_UART_3_RX_CALLBACK == 1)
    #if(USING_UART_3_IO_BUFFER == 1)
        void UART3InterruptRXCallback(BYTE Data)
        {

        }
    #else
        void UART3InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_3_IO_BUFFER == 1)
#endif // end of #if (USING_UART_3_RX_CALLBACK == 1)

#if (USING_UART_4_RX_CALLBACK == 1)
    #if(USING_UART_4_IO_BUFFER == 1)
        void UART4InterruptRXCallback(BYTE Data)
        {

        }
    #else
        void UART4InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_4_IO_BUFFER == 1)
#endif // end of #if (USING_UART_4_RX_CALLBACK == 1)

#if (USING_UART_5_RX_CALLBACK == 1)
    #if(USING_UART_5_IO_BUFFER == 1)
        void UART5InterruptRXCallback(BYTE Data)
        {

        }
    #else
        void UART5InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_5_IO_BUFFER == 1)
#endif // end of #if (USING_UART_5_RX_CALLBACK == 1)

#if (USING_UART_6_RX_CALLBACK == 1)
    #if(USING_UART_6_IO_BUFFER == 1)
        void UART6InterruptRXCallback(BYTE Data)
        {

        }
    #else
        void UART6InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_UART_6_IO_BUFFER == 1)
#endif // end of #if (USING_UART_6_RX_CALLBACK == 1)
    
#if (USING_UART_1_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"

    #if(USING_UART_1_IO_BUFFER == 1)
        void UART1InterruptErrorCallback(UART_ERROR Error)
        {

        }
    #else
        void UART1InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_UART_1_IO_BUFFER == 1)
#endif // end of #if (USING_UART_1_ERROR_CALLBACK == 1)
    
#if (USING_UART_2_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"

    #if(USING_UART_2_IO_BUFFER == 1)
        void UART2InterruptErrorCallback(UART_ERROR Error)
        {

        }
    #else
        void UART2InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_UART_2_IO_BUFFER == 1)
#endif // end of #if (USING_UART_2_ERROR_CALLBACK == 1)

#if (USING_UART_3_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"

    #if(USING_UART_3_IO_BUFFER == 1)
        void UART3InterruptErrorCallback(UART_ERROR Error)
        {

        }
    #else
        void UART3InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_UART_3_IO_BUFFER == 1)
#endif // end of #if (USING_UART_3_ERROR_CALLBACK == 1)

#if (USING_UART_4_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"

    #if(USING_UART_4_IO_BUFFER == 1)
        void UART4InterruptErrorCallback(UART_ERROR Error)
        {

        }
    #else
        void UART4InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_UART_4_IO_BUFFER == 1)
#endif // end of #if (USING_UART_4_ERROR_CALLBACK == 1)

#if (USING_UART_5_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"

    #if(USING_UART_5_IO_BUFFER == 1)
        void UART5InterruptErrorCallback(UART_ERROR Error)
        {

        }
    #else
        void UART5InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_UART_5_IO_BUFFER == 1)
#endif // end of #if (USING_UART_5_ERROR_CALLBACK == 1)

#if (USING_UART_6_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"

    #if(USING_UART_6_IO_BUFFER == 1)
        void UART6InterruptErrorCallback(UART_ERROR Error)
        {

        }
    #else
        void UART6InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_UART_6_IO_BUFFER == 1)
#endif // end of #if (USING_UART_6_ERROR_CALLBACK == 1)

#if (USING_CAN_1_TX_CALLBACK == 1)
    #if(USING_CAN_1_IO_BUFFER == 1)
        void CAN1InterruptTXCallback(BYTE *Buffer, UINT32 DataSizeInBytes)
        {

        }
    #else
        void CAN1InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)
#endif // end of #if (USING_CAN_1_TX_CALLBACK == 1)

#if (USING_CAN_2_TX_CALLBACK == 1)
    #if(USING_CAN_2_IO_BUFFER == 1)
        void CAN2InterruptTXCallback(BYTE *Buffer, UINT32 DataSizeInBytes)
        {

        }
    #else
        void CAN2InterruptTXCallback(void)
        {

        }
    #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)
#endif // end of #if (USING_CAN_2_TX_CALLBACK == 1)
        
#if (USING_CAN_1_RX_CALLBACK == 1)
    #if(USING_CAN_1_IO_BUFFER == 1)
        void CAN1InterruptRXCallback(BYTE *Buffer, UINT32 DataSizeInBytes)
        {

        }
    #else
        void CAN1InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)
#endif // end of #if (USING_CAN_1_RX_CALLBACK == 1)

#if (USING_CAN_2_RX_CALLBACK == 1)
    #if(USING_CAN_2_IO_BUFFER == 1)
        void CAN2InterruptRXCallback(BYTE *Buffer, UINT32 DataSizeInBytes)
        {

        }
    #else
        void CAN2InterruptRXCallback(void)
        {

        }
    #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)
#endif // end of #if (USING_CAN_2_RX_CALLBACK == 1)

#if (USING_CAN_1_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"
    
    #if(USING_CAN_1_IO_BUFFER == 1)
        void CAN1InterruptErrorCallback(CAN_ERROR Error, UINT16 RXErrorCount, UINT16 TXErrorCount)
        {

        }
    #else
        void CAN1InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)
#endif // end of #if (USING_CAN_1_ERROR_CALLBACK == 1)
    
#if (USING_CAN_2_ERROR_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"
    
    #if(USING_CAN_2_IO_BUFFER == 1)
        void CAN2InterruptErrorCallback(CAN_ERROR Error, UINT16 RXErrorCount, UINT16 TXErrorCount)
        {

        }
    #else
        void CAN2InterruptErrorCallback(void)
        {

        }
    #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)
#endif // end of #if (USING_CAN_2_ERROR_CALLBACK == 1)
