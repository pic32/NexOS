/*
    NexOS Kernel Version v1.01.04
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

#include "RTOSConfig.h"
#include "PortInterruptHandler.h"
#include "../NexOS/Kernel/Kernel.h"
#include "../NexOS/Event/OS_EventCallback.h"

#if (USING_IO_BUFFERS == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"
    #include "IOBufferPort.h"

    void OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_ID IOBufferID);
#endif // end of #if (USING_IO_BUFFERS == 1)

#if (USING_EVENTS == 1)
    #include "../NexOS/Event/Event.h"
#endif // end of #if (USING_EVENTS == 1)
    
#if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)
    void ExternalInterrupt0Callback(void);
    
    OS_WORD *ExternalInterrupt0Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt0Flag();
        
        #if (USING_EXT_INT_0_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_0_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_0_EVENT == 1)

        #if (USING_EXT_INT_0_CALLBACK == 1)
            ExternalInterrupt0Callback();
        #endif // end of #if (USING_EXT_INT_0_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)
    
#if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)
    void ExternalInterrupt1Callback(void);
    
    OS_WORD *ExternalInterrupt1Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt1Flag();

        #if (USING_EXT_INT_1_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_1_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_1_EVENT == 1)

        #if (USING_EXT_INT_1_CALLBACK == 1)
            ExternalInterrupt1Callback();
        #endif // end of #if (USING_EXT_INT_1_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)

#if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
    void ExternalInterrupt2Callback(void);
    
    OS_WORD *ExternalInterrupt2Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt2Flag();

        #if (USING_EXT_INT_2_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_2_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_2_EVENT == 1)

        #if (USING_EXT_INT_2_CALLBACK == 1)
            ExternalInterrupt2Callback();
        #endif // end of #if (USING_EXT_INT_2_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
    
#if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
    void ExternalInterrupt3Callback(void);
    
    OS_WORD *ExternalInterrupt3Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt3Flag();

        #if (USING_EXT_INT_3_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_3_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_3_EVENT == 1)

        #if (USING_EXT_INT_3_CALLBACK == 1)
            ExternalInterrupt3Callback();
        #endif // end of #if (USING_EXT_INT_3_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
    
#if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)
    void ExternalInterrupt4Callback(void);
    
    OS_WORD *ExternalInterrupt4Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt4Flag();

        #if (USING_EXT_INT_4_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_4_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_4_EVENT == 1)

        #if (USING_EXT_INT_4_CALLBACK == 1)
            ExternalInterrupt4Callback();
        #endif // end of #if (USING_EXT_INT_4_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)

#if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)
    void ChangeNotificationInterruptCallback(void);
    
    OS_WORD *ChangeNotificationInterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearChangeNotificationInterruptFlag();

        #if (USING_CN_INT_EVENT == 1)
            if(OS_RaiseEvent(CHANGE_NOTIFICATION_INTERRUPT_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_4_EVENT == 1)

        #if (USING_CN_INT_CALLBACK == 1)
            ChangeNotificationInterruptCallback();
        #endif // end of #if (USING_CN_INT_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)
    
#if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)
    void Timer1InterruptCallback(void);
    
    OS_WORD *Timer1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer1InterruptFlag();

        #if (USING_TIMER_1_EVENT == 1)
            if(OS_RaiseEvent(TIMER_1_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_1_EVENT == 1)

        #if (USING_TIMER_1_CALLBACK == 1)
            Timer1InterruptCallback();
        #endif // end of #if (USING_TIMER_1_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)  
    
#if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)
    void Timer2InterruptCallback(void);
    
    OS_WORD *Timer2InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer2InterruptFlag();

        #if (USING_TIMER_2_EVENT == 1)
            if(OS_RaiseEvent(TIMER_2_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_2_EVENT == 1)

        #if (USING_TIMER_2_CALLBACK == 1)
            Timer2InterruptCallback();
        #endif // end of #if (USING_TIMER_2_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)  
    
#if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)
    void Timer3InterruptCallback(void);
    
    OS_WORD *Timer3InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer3InterruptFlag();

        #if (USING_TIMER_3_EVENT == 1)
            if(OS_RaiseEvent(TIMER_3_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_3_EVENT == 1)

        #if (USING_TIMER_3_CALLBACK == 1)
            Timer3InterruptCallback();
        #endif // end of #if (USING_TIMER_3_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)  
    
#if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)
    void Timer4InterruptCallback(void);
    
    OS_WORD *Timer4InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer4InterruptFlag();

        #if (USING_TIMER_4_EVENT == 1)
            if(OS_RaiseEvent(TIMER_4_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_4_EVENT == 1)

        #if (USING_TIMER_4_CALLBACK == 1)
            Timer4InterruptCallback();
        #endif // end of #if (USING_TIMER_4_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)  
 
#if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)
    void Timer5InterruptCallback(void);
    
    OS_WORD *Timer5InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer5InterruptFlag();

        #if (USING_TIMER_5_EVENT == 1)
            if(OS_RaiseEvent(TIMER_5_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_5_EVENT == 1)

        #if (USING_TIMER_5_CALLBACK == 1)
            Timer5InterruptCallback();
        #endif // end of #if (USING_TIMER_5_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)  
    
#if (USING_ADC_1_IO_BUFFER == 1)
    void ADC1InterruptCallback(UINT16 ADCValue);

    BOOL UpdateADC1Buffer(UINT16 *Data, UINT32 DataBufferSize)
    {
        BOOL SwapTask = FALSE;
        
        if(PortIOBufferDataAvailable(IO_BUFFER_ADC_1_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            UINT32 BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_ADC_1_ID, (BYTE*)Data, ADC_1_ISR_DATA_BUFFER_SIZE_IN_SAMPLES * 2);

            // now write the data to the IO Buffer
            if(BytesRead != 0)
            {
                if(OS_WriteToIOBuffer(IO_BUFFER_ADC_1_ID, (BYTE*)Data, BytesRead) == TRUE)
                    SwapTask = TRUE;

                #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL
                    #define USE_RX_INTERRUPT_TRIGGER_LEVEL
                #endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

                // now update the RX interrupt trigger level
                OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_ADC_1_ID);
            }

            #if (USING_ADC_1_CALLBACK == 1)
            {
                UINT32 i;

                for(i = 0; i < BytesRead / 2; i++)
                    ADC1InterruptCallback(Data[i]);
            }
            #endif // end of #if (USING_ADC_1_CALLBACK == 1)

            #if (USING_ADC_1_EVENT == 1)
                if(OS_RaiseEvent(ADC_1_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_ADC_1_EVENT == 1)
        }
        
        return SwapTask;
    }
    
    OS_WORD *ADC1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        UINT16 Data[ADC_1_ISR_DATA_BUFFER_SIZE_IN_SAMPLES];

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        if(UpdateADC1Buffer(Data, sizeof(Data)) == TRUE)
            CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);

        // clear the interrupt flag
        PortIOBufferClearADC1InterruptFlag();
        
        return CurrentTaskStackPointer;
    }
#else
    #if (USING_ADC_1_EVENT == 1 || USING_ADC_1_CALLBACK == 1)
        void ADC1InterruptCallback(void);

        OS_WORD *ADC1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_ADC_1_CALLBACK == 1)
                ADC1InterruptCallback();
            #endif // end of #if (USING_ADC_1_CALLBACK == 1)

            #if (USING_ADC_1_EVENT == 1)
                if(OS_RaiseEvent(ADC_1_EVENT) == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if(USING_ADC_1_EVENT == 1)

            // now clear out the interrupt flag
            PortClearADC1InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_ADC_1_EVENT == 1 || USING_ADC_1_CALLBACK == 1)
#endif // end of #if (USING_ADC_1_IO_BUFFER == 1)
    
#if (USING_RTCC_ALARM_EVENT == 1 || USING_RTCC_ALARM_CALLBACK == 1)
    void RTCCAlarmInterruptCallback(void);
    
    OS_WORD *RTCCAlarmInterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        #if (USING_RTCC_ALARM_EVENT == 1)
            if(OS_RaiseEvent(RTCC_ALARM_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_RTCC_ALARM_EVENT == 1)

        #if (USING_RTCC_ALARM_CALLBACK == 1)
            RTCCAlarmInterruptCallback();
        #endif // end of #if (USING_RTCC_ALARM_CALLBACK == 1)

        // now clear out the interrupt flag
        PortClearRTCCInterruptFlag();
        
        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_RTCC_ALARM_EVENT == 1 || USING_RTCC_ALARM_CALLBACK == 1)
    
#if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)
    void ExternalOscillatorFailedInterruptCallback(void);
    
    OS_WORD *ExternalOscillatorFailedInterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalOscillatorFailedInterruptFlag();

        #if (USING_EXT_OSC_FAILED_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_OSCILLATOR_FAILED_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_OSC_FAILED_EVENT == 1)

        #if (USING_EXT_OSC_FAILED_CALLBACK == 1)
            ExternalOscillatorFailedInterruptCallback();
        #endif // end of #if (USING_EXT_OSC_FAILED_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)

#if (USING_UART_1_IO_BUFFER == 1)
    extern IO_BUFFER gUART1IOBuffer;

    BOOL UpdateUART1RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        if(PortIOBufferDataAvailable(IO_BUFFER_UART_1_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            UINT32 BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_UART_1_ID, Data, DataBufferSize);

            // now write the data to the IO Buffer
            if(BytesRead != 0)
            {
                if(OS_WriteToIOBuffer(IO_BUFFER_UART_1_ID, Data, BytesRead) == TRUE)
                    SwapTask = TRUE;

                #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL
                    #define USE_RX_INTERRUPT_TRIGGER_LEVEL
                #endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

                // now update the RX interrupt trigger level
                OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_UART_1_ID);
            }

            #if (USING_UART_1_RX_CALLBACK == 1)
            {
                UINT32 i;

                for(i = 0; i < BytesRead; i++)
                    UART1InterruptRXCallback(Data[i]);
            }
            #endif // end of #if (USING_UART_1_RX_CALLBACK == 1)

            #if (USING_UART_1_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(UART_1_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_UART_1_RX_DONE_EVENT == 1)

            // clear the interrupt flag
            PortIOBufferClearUART1RXInterruptFlag();
        }

        return SwapTask;
    }

    OS_WORD *UART1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        BYTE Data[UART_1_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        BOOL SwapTask = FALSE;

        #if(USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_ERROR_CALLBACK == 1)
            if(PortIOBufferGetUART1ErrorInterruptFlag())
            {
                #if (USING_UART_1_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(UART_1_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_UART_1_ERROR_EVENT == 1)

                #if (USING_UART_1_ERROR_CALLBACK == 1)
                    UART1InterruptErrorCallback((UART_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_UART_1_ID));
                #endif // end of #if (USING_UART_1_ERROR_CALLBACK == 1)

                PortIOBufferClearUART1ErrorInterruptFlag();
            }
        #endif // end of #if(USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_ERROR_CALLBACK == 1)

        SwapTask = UpdateUART1RXBuffer(Data, UART_1_ISR_DATA_BUFFER_SIZE_IN_BYTES);

        if(PortIOBufferGetUART1TXDoneInterruptFlag())
        {
            if(gUART1IOBuffer.TXState == IO_BUFFER_WRITE || gUART1IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
            {
                #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    // now if there is data to write out the port, do so
                    if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART1IOBuffer) != 0)
                    {
                        UINT32 BytesRead;

                        while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART1IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_1_ID) >= sizeof(Data))
                        {
                            // read 1 byte from the IO_BUFFER which will be TX'ed out!
                            BytesRead = OS_ReadFromIOBuffer(&gUART1IOBuffer, Data, sizeof(Data), PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_1_ID));

                            // otherwise write the data out the hardware
                            if(PortIOBufferWriteData(IO_BUFFER_UART_1_ID, Data, BytesRead) == FALSE)
                            {
                                gUART1IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                // turn interrupts off
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_1_ID, 0);
                            }
                            else
                            {
                                // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART1IOBuffer) != 0)
                                    PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_1_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART1IOBuffer));
                            }
                        }
                    }
                    else
                    {
                        // we need to stop the TX'ing
                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_1_ID, 0);

                        // only if the buffer was in non Async mode, do we add the TASK
                        // to the ready queue
                        if(gUART1IOBuffer.TXState == IO_BUFFER_WRITE)
                        {
                            // now add the TASK to the ready queue
                            if(OS_RemoveTaskFromIOBufferBlockedList(&gUART1IOBuffer) == TRUE)
                                SwapTask = TRUE;
                        }

                        // set the state to open now that the TX is done
                        gUART1IOBuffer.TXState = IO_BUFFER_OPEN;
                    }
                #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

                #if (USING_UART_1_TX_CALLBACK == 1)
                    UART1InterruptTXCallback();
                #endif // end of #if (USING_UART_1_TX_CALLBACK == 1)

                #if (USING_UART_1_TX_DONE_EVENT == 1)
                    if(OS_RaiseEvent(UART_1_TX_DONE_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if(USING_UART_1_TX_DONE_EVENT == 1)
            }

            PortIOBufferClearUART1TXInterruptFlag();
        }

        if(SwapTask == TRUE)
            CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);

        PortIOBufferClearUART1InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_RX_CALLBACK == 1 || USING_UART_1_TX_CALLBACK == 1 || USING_UART_1_ERROR_CALLBACK == 1)
        OS_WORD *UART1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1)

            #if(USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_ERROR_CALLBACK == 1)
                if(PortIOBufferGetUART1ErrorInterruptFlag())
                {
                    #if (USING_UART_1_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(UART_1_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_UART_1_ERROR_EVENT == 1)

                    #if (USING_UART_1_ERROR_CALLBACK == 1)
                        UART1InterruptErrorCallback();
                    #endif // end of #if (USING_UART_1_ERROR_CALLBACK == 1)

                    PortIOBufferClearUART1ErrorInterruptFlag();
                }
            #endif // end of #if(USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_ERROR_CALLBACK == 1)

            #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_RX_CALLBACK == 1)
                if(PortIOBufferGetUART1RXDataInterruptFlag())
                {
                    #if (USING_UART_1_RX_CALLBACK == 1)
                        UART1InterruptRXCallback();
                    #endif // end of #if (USING_UART_1_RX_CALLBACK == 1)

                    #if (USING_UART_1_RX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_1_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_1_RX_DONE_EVENT == 1)

                    PortIOBufferClearUART1RXInterruptFlag();
                }
            #endif // end of #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_RX_CALLBACK == 1)

            #if (USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_TX_CALLBACK == 1)
                if(PortIOBufferGetUART1TXDoneInterruptFlag())
                {
                    #if (USING_UART_1_TX_CALLBACK == 1)
                        UART1InterruptTXCallback();
                    #endif // end of #if (USING_UART_1_TX_CALLBACK == 1)

                    #if (USING_UART_1_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_1_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_1_TX_DONE_EVENT == 1)

                    PortIOBufferClearUART1TXInterruptFlag();
                }
            #endif // end of #if (USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_TX_CALLBACK == 1)

            #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1)

            PortIOBufferClearUART1InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_UART_1_RX_DONE_EVENT == 1 || USING_UART_1_TX_DONE_EVENT == 1 || USING_UART_1_ERROR_EVENT == 1 || USING_UART_1_RX_CALLBACK == 1 || USING_UART_1_TX_CALLBACK == 1 || USING_UART_1_ERROR_CALLBACK == 1)
#endif // end of #if (USING_UART_1_IO_BUFFER == 1)

#if (USING_UART_2_IO_BUFFER == 1)
    extern IO_BUFFER gUART2IOBuffer;

    BOOL UpdateUART2RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        if(PortIOBufferDataAvailable(IO_BUFFER_UART_2_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            UINT32 BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_UART_2_ID, Data, DataBufferSize);

            // now write the data to the IO Buffer
            if(BytesRead != 0)
            {
                if(OS_WriteToIOBuffer(IO_BUFFER_UART_2_ID, Data, BytesRead) == TRUE)
                    SwapTask = TRUE;

                #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL
                    #define USE_RX_INTERRUPT_TRIGGER_LEVEL
                #endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

                // now update the RX interrupt trigger level
                OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_UART_2_ID);
            }

            #if (USING_UART_2_RX_CALLBACK == 1)
            {
                UINT32 i;

                for(i = 0; i < BytesRead; i++)
                    UART2InterruptRXCallback(Data[i]);
            }
            #endif // end of #if (USING_UART_2_RX_CALLBACK == 1)

            #if (USING_UART_2_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(UART_2_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_UART_2_RX_DONE_EVENT == 1)

            // clear the interrupt flag
            PortIOBufferClearUART2RXInterruptFlag();
        }

        return SwapTask;
    }

    OS_WORD *UART2InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        BYTE Data[UART_2_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        BOOL SwapTask = FALSE;

        #if(USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_ERROR_CALLBACK == 1)
            if(PortIOBufferGetUART2ErrorInterruptFlag())
            {
                #if (USING_UART_2_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(UART_2_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_UART_2_ERROR_EVENT == 1)

                #if (USING_UART_2_ERROR_CALLBACK == 1)
                    UART2InterruptErrorCallback((UART_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_UART_2_ID));
                #endif // end of #if (USING_UART_2_ERROR_CALLBACK == 1)

                PortIOBufferClearUART2ErrorInterruptFlag();
            }
        #endif // end of #if(USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_ERROR_CALLBACK == 1)

        SwapTask = UpdateUART2RXBuffer(Data, UART_2_ISR_DATA_BUFFER_SIZE_IN_BYTES);

        if(PortIOBufferGetUART2TXDoneInterruptFlag())
        {
            if(gUART2IOBuffer.TXState == IO_BUFFER_WRITE || gUART2IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
            {
                #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    // now if there is data to write out the port, do so
                    if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART2IOBuffer) != 0)
                    {
                        UINT32 BytesRead;

                        while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART2IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_2_ID) >= sizeof(Data))
                        {
                            // read 1 byte from the IO_BUFFER which will be TX'ed out!
                            BytesRead = OS_ReadFromIOBuffer(&gUART2IOBuffer, Data, sizeof(Data), PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_2_ID));

                            // otherwise write the data out the hardware
                            if(PortIOBufferWriteData(IO_BUFFER_UART_2_ID, Data, BytesRead) == FALSE)
                            {
                                gUART2IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                // turn interrupts off
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_2_ID, 0);
                            }
                            else
                            {
                                // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART2IOBuffer) != 0)
                                    PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_2_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART2IOBuffer));
                            }
                        }
                    }
                    else
                    {
                        // we need to stop the TX'ing
                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_2_ID, 0);

                        // only if the buffer was in non Async mode, do we add the TASK
                        // to the ready queue
                        if(gUART2IOBuffer.TXState == IO_BUFFER_WRITE)
                        {
                            // now add the TASK to the ready queue
                            if(OS_RemoveTaskFromIOBufferBlockedList(&gUART2IOBuffer) == TRUE)
                                SwapTask = TRUE;
                        }

                        // set the state to open now that the TX is done
                        gUART2IOBuffer.TXState = IO_BUFFER_OPEN;
                    }
                #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

                #if (USING_UART_2_TX_CALLBACK == 1)
                    UART2InterruptTXCallback();
                #endif // end of #if (USING_UART_2_TX_CALLBACK == 1)

                #if (USING_UART_2_TX_DONE_EVENT == 1)
                    if(OS_RaiseEvent(UART_2_TX_DONE_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if(USING_UART_2_TX_DONE_EVENT == 1)
            }

            PortIOBufferClearUART2TXInterruptFlag();
        }

        if(SwapTask == TRUE)
            CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);

        PortIOBufferClearUART2InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_RX_CALLBACK == 1 || USING_UART_2_TX_CALLBACK == 1 || USING_UART_2_ERROR_CALLBACK == 1)
        OS_WORD *UART2InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1)

            #if(USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_ERROR_CALLBACK == 1)
                if(PortIOBufferGetUART2ErrorInterruptFlag())
                {
                    #if (USING_UART_2_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(UART_2_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_UART_2_ERROR_EVENT == 1)
                        
                    #if (USING_UART_2_ERROR_CALLBACK == 1)
                        UART2InterruptErrorCallback();
                    #endif // end of #if (USING_UART_2_ERROR_CALLBACK == 1)
                    
                    PortIOBufferClearUART2ErrorInterruptFlag();
                }
            #endif // end of #if(USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_ERROR_CALLBACK == 1)

            #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_RX_CALLBACK == 1)
                if(PortIOBufferGetUART2RXDataInterruptFlag())
                {
                    #if (USING_UART_2_RX_CALLBACK == 1)
                        UART2InterruptRXCallback();
                    #endif // end of #if (USING_UART_2_RX_CALLBACK == 1)

                    #if (USING_UART_2_RX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_2_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_2_RX_DONE_EVENT == 1)

                    PortIOBufferClearUART2RXInterruptFlag();
                }
            #endif // end of #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_RX_CALLBACK == 1)

            #if (USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_TX_CALLBACK == 1)
                if(PortIOBufferGetUART2TXDoneInterruptFlag())
                {
                    #if (USING_UART_2_TX_CALLBACK == 1)
                        UART2InterruptTXCallback();
                    #endif // end of #if (USING_UART_2_TX_CALLBACK == 1)

                    #if (USING_UART_2_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_2_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_2_TX_DONE_EVENT == 1)

                    PortIOBufferClearUART2TXInterruptFlag();
                }
            #endif // end of #if (USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_TX_CALLBACK == 1)

            #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1)

            PortIOBufferClearUART2InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_UART_2_RX_DONE_EVENT == 1 || USING_UART_2_TX_DONE_EVENT == 1 || USING_UART_2_ERROR_EVENT == 1 || USING_UART_2_RX_CALLBACK == 1 || USING_UART_2_TX_CALLBACK == 1 || USING_UART_2_ERROR_CALLBACK == 1)
#endif // end of #if (USING_UART_2_IO_BUFFER == 1)

#if (USING_UART_3_IO_BUFFER == 1)
    extern IO_BUFFER gUART3IOBuffer;

    BOOL UpdateUART3RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        if(PortIOBufferDataAvailable(IO_BUFFER_UART_3_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            UINT32 BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_UART_3_ID, Data, DataBufferSize);

            // now write the data to the IO Buffer
            if(BytesRead != 0)
            {
                if(OS_WriteToIOBuffer(IO_BUFFER_UART_3_ID, Data, BytesRead) == TRUE)
                    SwapTask = TRUE;

                #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL
                    #define USE_RX_INTERRUPT_TRIGGER_LEVEL
                #endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

                // now update the RX interrupt trigger level
                OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_UART_3_ID);
            }

            #if (USING_UART_3_RX_CALLBACK == 1)
            {
                UINT32 i;

                for(i = 0; i < BytesRead; i++)
                    UART3InterruptRXCallback(Data[i]);
            }
            #endif // end of #if (USING_UART_3_RX_CALLBACK == 1)

            #if (USING_UART_3_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(UART_3_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_UART_3_RX_DONE_EVENT == 1)

            // clear the interrupt flag
            PortIOBufferClearUART3RXInterruptFlag();
        }

        return SwapTask;
    }

    OS_WORD *UART3InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        BYTE Data[UART_3_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        BOOL SwapTask = FALSE;

        #if(USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_ERROR_CALLBACK == 1)
            if(PortIOBufferGetUART3ErrorInterruptFlag())
            {
                #if (USING_UART_3_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(UART_3_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_UART_3_ERROR_EVENT == 1)

                #if (USING_UART_3_ERROR_CALLBACK == 1)
                    UART3InterruptErrorCallback((UART_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_UART_3_ID));
                #endif // end of #if (USING_UART_3_ERROR_CALLBACK == 1)

                PortIOBufferClearUART3ErrorInterruptFlag();
            }
        #endif // end of #if(USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_ERROR_CALLBACK == 1)

        SwapTask = UpdateUART3RXBuffer(Data, UART_3_ISR_DATA_BUFFER_SIZE_IN_BYTES);

        if(PortIOBufferGetUART3TXDoneInterruptFlag())
        {
            if(gUART3IOBuffer.TXState == IO_BUFFER_WRITE || gUART3IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
            {
                #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    // now if there is data to write out the port, do so
                    if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART3IOBuffer) != 0)
                    {
                        UINT32 BytesRead;

                        while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART3IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_3_ID) >= sizeof(Data))
                        {
                            // read 1 byte from the IO_BUFFER which will be TX'ed out!
                            BytesRead = OS_ReadFromIOBuffer(&gUART3IOBuffer, Data, sizeof(Data), PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_3_ID));

                            // otherwise write the data out the hardware
                            if(PortIOBufferWriteData(IO_BUFFER_UART_3_ID, Data, BytesRead) == FALSE)
                            {
                                gUART3IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                // turn interrupts off
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_3_ID, 0);
                            }
                            else
                            {
                                // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART3IOBuffer) != 0)
                                    PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_3_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART3IOBuffer));

                            }
                        }
                    }
                    else
                    {
                        // we need to stop the TX'ing
                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_3_ID, 0);

                        // only if the buffer was in non Async mode, do we add the TASK
                        // to the ready queue
                        if(gUART3IOBuffer.TXState == IO_BUFFER_WRITE)
                        {
                            // now add the TASK to the ready queue
                            if(OS_RemoveTaskFromIOBufferBlockedList(&gUART3IOBuffer) == TRUE)
                                SwapTask = TRUE;
                        }

                        // set the state to open now that the TX is done
                        gUART3IOBuffer.TXState = IO_BUFFER_OPEN;
                    }
                #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

                #if (USING_UART_3_TX_CALLBACK == 1)
                    UART3InterruptTXCallback();
                #endif // end of #if (USING_UART_3_TX_CALLBACK == 1)

                #if (USING_UART_3_TX_DONE_EVENT == 1)
                    if(OS_RaiseEvent(UART_3_TX_DONE_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if(USING_UART_3_TX_DONE_EVENT == 1)
            }

            PortIOBufferClearUART3TXInterruptFlag();
        }
        
        if(SwapTask == TRUE)
            CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);

        PortIOBufferClearUART3InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_RX_CALLBACK == 1 || USING_UART_3_TX_CALLBACK == 1 || USING_UART_3_ERROR_CALLBACK == 1)
        OS_WORD *UART3InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1)

            #if(USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_ERROR_CALLBACK == 1)
                if(PortIOBufferGetUART3ErrorInterruptFlag())
                {
                    #if (USING_UART_3_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(UART_3_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_UART_3_ERROR_EVENT == 1)
                        
                    #if (USING_UART_3_ERROR_CALLBACK == 1)
                        UART3InterruptErrorCallback();
                    #endif // end of #if (USING_UART_3_ERROR_CALLBACK == 1)
                    
                    PortIOBufferClearUART3ErrorInterruptFlag();
                }
            #endif // end of #if(USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_ERROR_CALLBACK == 1)

            #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_RX_CALLBACK == 1)
                if(PortIOBufferGetUART3RXDataInterruptFlag())
                {
                    #if (USING_UART_3_RX_CALLBACK == 1)
                        UART3InterruptRXCallback();
                    #endif // end of #if (USING_UART_3_RX_CALLBACK == 1)

                    #if (USING_UART_3_RX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_3_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_3_RX_DONE_EVENT == 1)

                    PortIOBufferClearUART3RXInterruptFlag();
                }
            #endif // end of #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_RX_CALLBACK == 1)

            #if (USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_TX_CALLBACK == 1)
                if(PortIOBufferGetUART3TXDoneInterruptFlag())
                {
                    #if (USING_UART_3_TX_CALLBACK == 1)
                        UART3InterruptTXCallback();
                    #endif // end of #if (USING_UART_3_TX_CALLBACK == 1)

                    #if (USING_UART_3_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_3_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_3_TX_DONE_EVENT == 1)

                    PortIOBufferClearUART3TXInterruptFlag();
                }
            #endif // end of #if (USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_TX_CALLBACK == 1)

            #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1)

            PortIOBufferClearUART3InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_UART_3_RX_DONE_EVENT == 1 || USING_UART_3_TX_DONE_EVENT == 1 || USING_UART_3_ERROR_EVENT == 1 || USING_UART_3_RX_CALLBACK == 1 || USING_UART_3_TX_CALLBACK == 1 || USING_UART_3_ERROR_CALLBACK == 1)
#endif // end of #if (USING_UART_3_IO_BUFFER == 1)

#if (USING_UART_4_IO_BUFFER == 1)
    extern IO_BUFFER gUART4IOBuffer;

    BOOL UpdateUART4RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        if(PortIOBufferDataAvailable(IO_BUFFER_UART_4_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            UINT32 BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_UART_4_ID, Data, DataBufferSize);

            // now write the data to the IO Buffer
            if(BytesRead != 0)
            {
                if(OS_WriteToIOBuffer(IO_BUFFER_UART_4_ID, Data, BytesRead) == TRUE)
                    SwapTask = TRUE;

                #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL
                    #define USE_RX_INTERRUPT_TRIGGER_LEVEL
                #endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

                // now update the RX interrupt trigger level
                OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_UART_4_ID);
            }

            #if (USING_UART_4_RX_CALLBACK == 1)
            {
                UINT32 i;

                for(i = 0; i < BytesRead; i++)
                    UART4InterruptRXCallback(Data[i]);
            }
            #endif // end of #if (USING_UART_4_RX_CALLBACK == 1)

            #if (USING_UART_4_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(UART_4_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_UART_4_RX_DONE_EVENT == 1)

            // clear the interrupt flag
            PortIOBufferClearUART4RXInterruptFlag();
        }

        return SwapTask;
    }

    OS_WORD *UART4InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        BYTE Data[UART_4_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        BOOL SwapTask = FALSE;
            
        #if(USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_ERROR_CALLBACK == 1)
            if(PortIOBufferGetUART4ErrorInterruptFlag())
            {
                #if (USING_UART_4_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(UART_4_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_UART_4_ERROR_EVENT == 1)

                #if (USING_UART_4_ERROR_CALLBACK == 1)
                    UART4InterruptErrorCallback((UART_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_UART_4_ID));
                #endif // end of #if (USING_UART_4_ERROR_CALLBACK == 1)

                PortIOBufferClearUART4ErrorInterruptFlag();
            }
        #endif // end of #if(USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_ERROR_CALLBACK == 1)

        SwapTask = UpdateUART4RXBuffer(Data, UART_4_ISR_DATA_BUFFER_SIZE_IN_BYTES);

        if(PortIOBufferGetUART4TXDoneInterruptFlag())
        {
            if(gUART4IOBuffer.TXState == IO_BUFFER_WRITE || gUART4IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
            {
                #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    // now if there is data to write out the port, do so
                    if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART4IOBuffer) != 0)
                    {
                        UINT32 BytesRead;

                        while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART4IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_4_ID) >= sizeof(Data))
                        {
                            // read 1 byte from the IO_BUFFER which will be TX'ed out!
                            BytesRead = OS_ReadFromIOBuffer(&gUART4IOBuffer, Data, sizeof(Data), PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_4_ID));

                            // otherwise write the data out the hardware
                            if(PortIOBufferWriteData(IO_BUFFER_UART_4_ID, Data, BytesRead) == FALSE)
                            {
                                gUART4IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                // turn interrupts off
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_4_ID, 0);
                            }
                            else
                            {
                                // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART4IOBuffer) != 0)
                                    PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_4_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART4IOBuffer));
                            }
                        }
                    }
                    else
                    {
                        // we need to stop the TX'ing
                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_4_ID, 0);

                        // only if the buffer was in non Async mode, do we add the TASK
                        // to the ready queue
                        if(gUART4IOBuffer.TXState == IO_BUFFER_WRITE)
                        {
                            // now add the TASK to the ready queue
                            if(OS_RemoveTaskFromIOBufferBlockedList(&gUART4IOBuffer) == TRUE)
                                SwapTask = TRUE;
                        }

                        // set the state to open now that the TX is done
                        gUART4IOBuffer.TXState = IO_BUFFER_OPEN;
                    }
                #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

                #if (USING_UART_4_TX_CALLBACK == 1)
                    UART4InterruptTXCallback();
                #endif // end of #if (USING_UART_4_TX_CALLBACK == 1)

                #if (USING_UART_4_TX_DONE_EVENT == 1)
                    if(OS_RaiseEvent(UART_4_TX_DONE_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if(USING_UART_4_TX_DONE_EVENT == 1)
            }

            PortIOBufferClearUART4TXInterruptFlag();
        }
        
        if(SwapTask == TRUE)
            CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);

        PortIOBufferClearUART4InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_RX_CALLBACK == 1 || USING_UART_4_TX_CALLBACK == 1 || USING_UART_4_ERROR_CALLBACK == 1)
        OS_WORD *UART4InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1)

            #if(USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_ERROR_CALLBACK == 1)
                if(PortIOBufferGetUART4ErrorInterruptFlag())
                {
                    #if (USING_UART_4_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(UART_4_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_UART_4_ERROR_EVENT == 1)
                        
                    #if (USING_UART_4_ERROR_CALLBACK == 1)
                        UART4InterruptErrorCallback();
                    #endif // end of #if (USING_UART_4_ERROR_CALLBACK == 1)
                    
                    PortIOBufferClearUART4ErrorInterruptFlag();
                }
            #endif // end of #if(USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_ERROR_CALLBACK == 1)

            #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_RX_CALLBACK == 1)
                if(PortIOBufferGetUART4RXDataInterruptFlag())
                {
                    #if (USING_UART_4_RX_CALLBACK == 1)
                        UART4InterruptRXCallback();
                    #endif // end of #if (USING_UART_4_RX_CALLBACK == 1)

                    #if (USING_UART_4_RX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_4_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_4_RX_DONE_EVENT == 1)

                    PortIOBufferClearUART4RXInterruptFlag();
                }
            #endif // end of #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_RX_CALLBACK == 1)

            #if (USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_TX_CALLBACK == 1)
                if(PortIOBufferGetUART4TXDoneInterruptFlag())
                {
                    #if (USING_UART_4_TX_CALLBACK == 1)
                        UART4InterruptTXCallback();
                    #endif // end of #if (USING_UART_4_TX_CALLBACK == 1)

                    #if (USING_UART_4_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_4_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_4_TX_DONE_EVENT == 1)

                    PortIOBufferClearUART4TXInterruptFlag();
                }
            #endif // end of #if (USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_TX_CALLBACK == 1)

            #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1)

            PortIOBufferClearUART4InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_UART_4_RX_DONE_EVENT == 1 || USING_UART_4_TX_DONE_EVENT == 1 || USING_UART_4_ERROR_EVENT == 1 || USING_UART_4_RX_CALLBACK == 1 || USING_UART_4_TX_CALLBACK == 1 || USING_UART_4_ERROR_CALLBACK == 1)
#endif // end of #if (USING_UART_4_IO_BUFFER)

#if (USING_UART_5_IO_BUFFER == 1)
    extern IO_BUFFER gUART5IOBuffer;

    BOOL UpdateUART5RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        if(PortIOBufferDataAvailable(IO_BUFFER_UART_5_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            UINT32 BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_UART_5_ID, Data, DataBufferSize);

            // now write the data to the IO Buffer
            if(BytesRead != 0)
            {
                if(OS_WriteToIOBuffer(IO_BUFFER_UART_5_ID, Data, BytesRead) == TRUE)
                    SwapTask = TRUE;

                #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL
                    #define USE_RX_INTERRUPT_TRIGGER_LEVEL
                #endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

                // now update the RX interrupt trigger level
                OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_UART_5_ID);
            }

            #if (USING_UART_5_RX_CALLBACK == 1)
            {
                UINT32 i;

                for(i = 0; i < BytesRead; i++)
                    UART5InterruptRXCallback(Data[i]);
            }
            #endif // end of #if (USING_UART_5_RX_CALLBACK == 1)

            #if (USING_UART_5_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(UART_5_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_UART_5_RX_DONE_EVENT == 1)

            // clear the interrupt flag
            PortIOBufferClearUART5RXInterruptFlag();
        }

        return SwapTask;
    }

    OS_WORD *UART5InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        BYTE Data[UART_5_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        BOOL SwapTask = FALSE;

        #if(USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_ERROR_CALLBACK == 1)
            if(PortIOBufferGetUART5ErrorInterruptFlag())
            {
                #if (USING_UART_5_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(UART_5_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_UART_5_ERROR_EVENT == 1)

                #if (USING_UART_5_ERROR_CALLBACK == 1)
                    UART5InterruptErrorCallback((UART_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_UART_5_ID));
                #endif // end of #if (USING_UART_5_ERROR_CALLBACK == 1)

                PortIOBufferClearUART5ErrorInterruptFlag();
            }
        #endif // end of #if(USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_ERROR_CALLBACK == 1)

        SwapTask = UpdateUART5RXBuffer(Data, UART_5_ISR_DATA_BUFFER_SIZE_IN_BYTES);

        if(PortIOBufferGetUART5TXDoneInterruptFlag())
        {
            if(gUART5IOBuffer.TXState == IO_BUFFER_WRITE || gUART5IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
            {
                #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    // now if there is data to write out the port, do so
                    if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART5IOBuffer) != 0)
                    {
                        UINT32 BytesRead;

                        while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART5IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_5_ID) >= sizeof(Data))
                        {
                            // read 1 byte from the IO_BUFFER which will be TX'ed out!
                            BytesRead = OS_ReadFromIOBuffer(&gUART5IOBuffer, Data, sizeof(Data), PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_5_ID));

                            // otherwise write the data out the hardware
                            if(PortIOBufferWriteData(IO_BUFFER_UART_5_ID, Data, BytesRead) == FALSE)
                            {
                                gUART5IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                // turn interrupts off
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_5_ID, 0);
                            }
                            else
                            {
                                // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART5IOBuffer) != 0)
                                    PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_5_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART5IOBuffer));
                            }
                        }
                    }
                    else
                    {
                        // we need to stop the TX'ing
                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_5_ID, 0);

                        // only if the buffer was in non Async mode, do we add the TASK
                        // to the ready queue
                        if(gUART5IOBuffer.TXState == IO_BUFFER_WRITE)
                        {
                            // now add the TASK to the ready queue
                            if(OS_RemoveTaskFromIOBufferBlockedList(&gUART5IOBuffer) == TRUE)
                                SwapTask = TRUE;
                        }

                        // set the state to open now that the TX is done
                        gUART5IOBuffer.TXState = IO_BUFFER_OPEN;
                    }
                #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

                #if (USING_UART_5_TX_CALLBACK == 1)
                    UART5InterruptTXCallback();
                #endif // end of #if (USING_UART_5_TX_CALLBACK == 1)

                #if (USING_UART_5_TX_DONE_EVENT == 1)
                    if(OS_RaiseEvent(UART_5_TX_DONE_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if(USING_UART_5_TX_DONE_EVENT == 1)
            }

            PortIOBufferClearUART5TXInterruptFlag();
        }

        if(SwapTask == TRUE)
            CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);

        PortIOBufferClearUART5InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_RX_CALLBACK == 1 || USING_UART_5_TX_CALLBACK == 1 || USING_UART_5_ERROR_CALLBACK == 1)
        OS_WORD *UART5InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1)

            #if(USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_ERROR_CALLBACK == 1)
                if(PortIOBufferGetUART5ErrorInterruptFlag())
                {
                    #if (USING_UART_5_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(UART_5_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_UART_5_ERROR_EVENT == 1)
                        
                    #if (USING_UART_5_ERROR_CALLBACK == 1)
                        UART5InterruptErrorCallback();
                    #endif // end of #if (USING_UART_5_ERROR_CALLBACK == 1)
                    
                    PortIOBufferClearUART5ErrorInterruptFlag();
                }
            #endif // end of #if(USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_ERROR_CALLBACK == 1)

            #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_RX_CALLBACK == 1)
                if(PortIOBufferGetUART5RXDataInterruptFlag())
                {
                    #if (USING_UART_5_RX_CALLBACK == 1)
                        UART5InterruptRXCallback();
                    #endif // end of #if (USING_UART_5_RX_CALLBACK == 1)

                    #if (USING_UART_5_RX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_5_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_5_RX_DONE_EVENT == 1)

                    PortIOBufferClearUART5RXInterruptFlag();
                }
            #endif // end of #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_RX_CALLBACK == 1)

            #if (USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_TX_CALLBACK == 1)
                if(PortIOBufferGetUART5TXDoneInterruptFlag())
                {
                    #if (USING_UART_5_TX_CALLBACK == 1)
                        UART5InterruptTXCallback();
                    #endif // end of #if (USING_UART_5_TX_CALLBACK == 1)

                    #if (USING_UART_5_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_5_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_5_TX_DONE_EVENT == 1)

                    PortIOBufferClearUART5TXInterruptFlag();
                }
            #endif // end of #if (USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_TX_CALLBACK == 1)

            #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1)

            PortIOBufferClearUART5InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_UART_5_RX_DONE_EVENT == 1 || USING_UART_5_TX_DONE_EVENT == 1 || USING_UART_5_ERROR_EVENT == 1 || USING_UART_5_RX_CALLBACK == 1 || USING_UART_5_TX_CALLBACK == 1 || USING_UART_5_ERROR_CALLBACK == 1)
#endif // end of #if (USING_UART_5_IO_BUFFER == 1)

#if (USING_UART_6_IO_BUFFER == 1)
    extern IO_BUFFER gUART6IOBuffer;

    BOOL UpdateUART6RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        if(PortIOBufferDataAvailable(IO_BUFFER_UART_6_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            UINT32 BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_UART_6_ID, Data, DataBufferSize);

            // now write the data to the IO Buffer
            if(BytesRead != 0)
            {
                if(OS_WriteToIOBuffer(IO_BUFFER_UART_6_ID, Data, BytesRead) == TRUE)
                    SwapTask = TRUE;

                #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL
                    #define USE_RX_INTERRUPT_TRIGGER_LEVEL
                #endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

                // now update the RX interrupt trigger level
                OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_UART_6_ID);
            }

            #if (USING_UART_6_RX_CALLBACK == 1)
            {
                UINT32 i;

                for(i = 0; i < BytesRead; i++)
                    UART6InterruptRXCallback(Data[i]);
            }
            #endif // end of #if (USING_UART_6_RX_CALLBACK == 1)

            #if (USING_UART_6_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(UART_6_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_UART_6_RX_DONE_EVENT == 1)

            // clear the interrupt flag
            PortIOBufferClearUART6RXInterruptFlag();
        }

        return SwapTask;
    }

    OS_WORD *UART6InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        BYTE Data[UART_6_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        BOOL SwapTask = FALSE;

        #if(USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_ERROR_CALLBACK == 1)
            if(PortIOBufferGetUART6ErrorInterruptFlag())
            {
                #if (USING_UART_6_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(UART_6_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_UART_6_ERROR_EVENT == 1)

                #if (USING_UART_6_ERROR_CALLBACK == 1)
                    UART6InterruptErrorCallback((UART_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_UART_6_ID));
                #endif // end of #if (USING_UART_6_ERROR_CALLBACK == 1)

                PortIOBufferClearUART6ErrorInterruptFlag();
            }
        #endif // end of #if(USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_ERROR_CALLBACK == 1)

        SwapTask = UpdateUART6RXBuffer(Data, UART_6_ISR_DATA_BUFFER_SIZE_IN_BYTES);

        if(PortIOBufferGetUART6TXDoneInterruptFlag())
        {
            if(gUART6IOBuffer.TXState == IO_BUFFER_WRITE || gUART6IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
            {
                #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    // now if there is data to write out the port, do so
                    if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART6IOBuffer) != 0)
                    {
                        UINT32 BytesRead;

                        while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART6IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_6_ID) >= sizeof(Data))
                        {
                            // read 1 byte from the IO_BUFFER which will be TX'ed out!
                            BytesRead = OS_ReadFromIOBuffer(&gUART6IOBuffer, Data, sizeof(Data), PortIOBufferGetTXSpaceAvailable(IO_BUFFER_UART_6_ID));

                            // otherwise write the data out the hardware
                            if(PortIOBufferWriteData(IO_BUFFER_UART_6_ID, Data, BytesRead) == FALSE)
                            {
                                gUART6IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                // turn interrupts off
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_6_ID, 0);
                            }
                            else
                            {
                                // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART6IOBuffer) != 0)
                                    PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_6_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gUART6IOBuffer));
                            }
                        }
                    }
                    else
                    {
                        // we need to stop the TX'ing
                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_UART_6_ID, 0);

                        // only if the buffer was in non Async mode, do we add the TASK
                        // to the ready queue
                        if(gUART6IOBuffer.TXState == IO_BUFFER_WRITE)
                        {
                            // now add the TASK to the ready queue
                            if(OS_RemoveTaskFromIOBufferBlockedList(&gUART6IOBuffer) == TRUE)
                                SwapTask = TRUE;
                        }

                        // set the state to open now that the TX is done
                        gUART6IOBuffer.TXState = IO_BUFFER_OPEN;
                    }
                #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

                #if (USING_UART_6_TX_CALLBACK == 1)
                    UART6InterruptTXCallback();
                #endif // end of #if (USING_UART_6_TX_CALLBACK == 1)

                #if (USING_UART_6_TX_DONE_EVENT == 1)
                    if(OS_RaiseEvent(UART_6_TX_DONE_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if(USING_UART_6_TX_DONE_EVENT == 1)
            }

            PortIOBufferClearUART6TXInterruptFlag();
        }

        if(SwapTask == TRUE)
            CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);

        PortIOBufferClearUART6InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_RX_CALLBACK == 1 || USING_UART_6_TX_CALLBACK == 1 || USING_UART_6_ERROR_CALLBACK == 1)
        OS_WORD *UART6InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1)

            #if(USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_ERROR_CALLBACK == 1)
                if(PortIOBufferGetUART6ErrorInterruptFlag())
                {
                    #if (USING_UART_6_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(UART_6_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_UART_6_ERROR_EVENT == 1)
                        
                    #if (USING_UART_6_ERROR_CALLBACK == 1)
                        UART6InterruptErrorCallback();
                    #endif // end of #if (USING_UART_6_ERROR_CALLBACK == 1)
                    
                    PortIOBufferClearUART6ErrorInterruptFlag();
                }
            #endif // end of #if(USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_ERROR_CALLBACK == 1)

            #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_RX_CALLBACK == 1)
                if(PortIOBufferGetUART6RXDataInterruptFlag())
                {
                    #if (USING_UART_6_RX_CALLBACK == 1)
                        UART6InterruptRXCallback();
                    #endif // end of #if (USING_UART_6_RX_CALLBACK == 1)

                    #if (USING_UART_6_RX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_6_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_6_RX_DONE_EVENT == 1)

                    PortIOBufferClearUART6RXInterruptFlag();
                }
            #endif // end of #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_RX_CALLBACK == 1)

            #if (USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_TX_CALLBACK == 1)
                if(PortIOBufferGetUART6TXDoneInterruptFlag())
                {
                    #if (USING_UART_6_TX_CALLBACK == 1)
                        UART6InterruptTXCallback();
                    #endif // end of #if (USING_UART_6_TX_CALLBACK == 1)

                    #if (USING_UART_6_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(UART_6_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_UART_6_TX_DONE_EVENT == 1)

                    PortIOBufferClearUART6TXInterruptFlag();
                }
            #endif // end of #if (USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_TX_CALLBACK == 1)

            #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1)

            PortIOBufferClearUART6InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_UART_6_RX_DONE_EVENT == 1 || USING_UART_6_TX_DONE_EVENT == 1 || USING_UART_6_ERROR_EVENT == 1 || USING_UART_6_RX_CALLBACK == 1 || USING_UART_6_TX_CALLBACK == 1 || USING_UART_6_ERROR_CALLBACK == 1)
#endif // end of #if (USING_UART_6_IO_BUFFER == 1)

#ifdef USE_RX_INTERRUPT_TRIGGER_LEVEL
    void OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_ID IOBufferID)
    {
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        // update the RX trigger point for the interrupt
        switch(IOBuffer->RXState)
        {
            case IO_BUFFER_CLOSED:
            case IO_BUFFER_OVERFLOW:
            case IO_BUFFER_HARDWARE_ERROR:
            {
                PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, 0);

                break;
            }

            case IO_BUFFER_WRITE:
            case IO_BUFFER_ASYNC_WRITE:
            case IO_BUFFER_OPEN:
            {
                PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL);

                break;
            }

            case IO_BUFFER_READ_NUMBER_OF_BYTES:
            {
                PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, IOBuffer->BytesLeftToExchange);

                break;
            }

            case IO_BUFFER_READ_UNTIL_BYTE:
            case IO_BUFFER_READ_UNTIL_SEQUENCE:
            {
                PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL);

                break;
            }

            default:
            {
                PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, 0);

                break;
            }
        }
    }
#endif // end of #ifndef USE_RX_INTERRUPT_TRIGGER_LEVEL

#if (USING_CAN_1_IO_BUFFER == 1)
    void CAN1InterruptTXCallback(void);
    void CAN1InterruptRXCallback(BYTE *Buffer, UINT32 DataSizeInBytes);
    void CAN1InterruptErrorCallback(CAN_ERROR Error, UINT16 RXErrorCount, UINT16 TXErrorCount);

    #if(USING_CAN_1_IO_BUFFER == 1)
        extern IO_BUFFER gCAN1IOBuffer;
    #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

    BOOL UpdateCAN1RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_CALLBACK == 1)
            UINT32 BytesRead;
        #endif // end of #if (USING_UART_1_IO_BUFFER == 1 || USING_UART_1_RX_CALLBACK == 1)

        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        while(PortIOBufferDataAvailable(IO_BUFFER_CAN_1_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_CALLBACK == 1)
                BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_CAN_1_ID, Data, DataBufferSize);
            #endif // end of #if (USING_UART_1_IO_BUFFER == 1 || USING_UART_1_RX_CALLBACK == 1)

            #if (USING_CAN_1_IO_BUFFER == 1)
                // now write the data to the IO Buffer
                if(BytesRead != 0)
                {
                    if(OS_WriteToIOBuffer(IO_BUFFER_CAN_1_ID, Data, BytesRead) == TRUE)
                        SwapTask = TRUE;

                    // now update the RX interrupt trigger level
                    OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_CAN_1_ID);
                }
            #endif // end of #if (USING_UART_1_IO_BUFFER == 1)

            #if (USING_CAN_1_RX_CALLBACK == 1)
                CAN1InterruptRXCallback(Data, BytesRead);
            #endif // end of #if (USING_CAN_1_RX_CALLBACK == 1)

            #if (USING_CAN_1_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(CAN_1_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_CAN_1_RX_DONE_EVENT == 1)
        }

        // clear the interrupt flag
        PortIOBufferClearCAN1RXInterruptFlag();

        return SwapTask;
    }

    OS_WORD *CAN1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_CALLBACK == 1)
            BYTE Data[CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        #endif // end of #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_CALLBACK == 1)

        #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_DONE_EVENT == 1 || USING_CAN_1_TX_DONE_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)
            BOOL SwapTask = FALSE;
        #endif // end of #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_DONE_EVENT == 1 || USING_CAN_1_TX_DONE_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)

        #if(USING_CAN_1_ERROR_EVENT == 1 || USING_CAN_1_ERROR_CALLBACK == 1)
            if(PortIOBufferGetCAN1ErrorInterruptFlag())
            {
                #if (USING_CAN_1_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(CAN_1_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_CAN_1_ERROR_EVENT == 1)

                #if (USING_CAN_1_ERROR_CALLBACK == 1)
                    CAN1InterruptErrorCallback((CAN_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_CAN_1_ID), (UINT16)CANGetRxErrorCount(CAN1), (UINT16)CANGetTxErrorCount(CAN1));
                #endif // end of #if (USING_CAN_1_ERROR_CALLBACK == 1)

                PortIOBufferClearCAN1ErrorInterruptFlag();
            }
        #endif // end of #if(USING_CAN_1_ERROR_EVENT == 1 || USING_CAN_1_ERROR_CALLBACK == 1)

        #if (USING_CAN_1_RX_DONE_EVENT == 1 || USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_CALLBACK == 1)
            SwapTask = UpdateCAN1RXBuffer(Data, CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES);
        #endif // end of #if (USING_CAN_1_RX_DONE_EVENT == 1 || USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_CALLBACK == 1)

        #if (USING_CAN_1_TX_DONE_EVENT == 1 || USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_TX_CALLBACK == 1)
            if(PortIOBufferGetCAN1TXDoneInterruptFlag())
            {
                if(gCAN1IOBuffer.TXState == IO_BUFFER_WRITE || gCAN1IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
                {
                    #if (USING_CAN_1_IO_BUFFER == 1)
                        #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                            // now if there is data to write out the port, do so
                            if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN1IOBuffer) != 0)
                            {
                                UINT32 BytesRead;

                                while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN1IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_CAN_1_ID) >= sizeof(Data))
                                {
                                    // read 1 byte from the IO_BUFFER which will be TX'ed out!
                                    BytesRead = OS_ReadFromIOBuffer(&gCAN1IOBuffer, Data, sizeof(Data), sizeof(Data));

                                    // otherwise write the data out the hardware
                                    if(PortIOBufferWriteData(IO_BUFFER_CAN_1_ID, Data, BytesRead) == FALSE)
                                    {
                                        gCAN1IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                        // turn interrupts off
                                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_CAN_1_ID, 0);
                                    }
                                    else
                                    {
                                        // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                        if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN1IOBuffer) != 0)
                                            PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_CAN_1_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN1IOBuffer));
                                    }
                                }
                            }
                            else
                            {
                                // we need to stop the TX'ing
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_CAN_1_ID, 0);

                                // only if the buffer was in non Async mode, do we add the TASK
                                // to the ready queue
                                if(gCAN1IOBuffer.TXState == IO_BUFFER_WRITE)
                                {
                                    // now add the TASK to the ready queue
                                    if(OS_RemoveTaskFromIOBufferBlockedList(&gCAN1IOBuffer) == TRUE)
                                        SwapTask = TRUE;
                                }

                                // set the state to open now that the TX is done
                                gCAN1IOBuffer.TXState = IO_BUFFER_OPEN;
                            }
                        #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    #endif // end of #if (USING_CAN_1_IO_BUFFER == 1)

                    #if (USING_CAN_1_TX_CALLBACK == 1)
                        CAN1InterruptTXCallback();
                    #endif // end of #if (USING_CAN_1_TX_CALLBACK == 1)

                    #if (USING_CAN_1_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(CAN_1_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_CAN_1_TX_DONE_EVENT == 1)
                }

                PortIOBufferClearCAN1TXInterruptFlag();
            }
        #endif // end of #if (USING_CAN_1_TX_DONE_EVENT == 1 || USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_TX_CALLBACK == 1)

        #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_DONE_EVENT == 1 || USING_CAN_1_TX_DONE_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)
            if(SwapTask == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if (USING_CAN_1_IO_BUFFER == 1 || USING_CAN_1_RX_DONE_EVENT == 1 || USING_CAN_1_TX_DONE_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)

        PortIOBufferClearCAN1InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1 || USING_CAN_1_RX_CALLBACK == 1 || USING_CAN_1_TX_CALLBACK == 1 || USING_CAN_1_ERROR_CALLBACK == 1)
        void CAN1InterruptTXCallback(void);
        void CAN1InterruptRXCallback(void);
        void CAN1InterruptErrorCallback(void);
        
        OS_WORD *CAN1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)

            #if(USING_CAN_1_ERROR_EVENT == 1 || USING_CAN_1_ERROR_CALLBACK == 1)
                if(PortIOBufferGetCAN1ErrorInterruptFlag())
                {
                    #if (USING_CAN_1_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(CAN_1_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_CAN_1_ERROR_EVENT == 1)
                        
                    #if (USING_CAN_1_ERROR_CALLBACK == 1)
                        CAN1InterruptErrorCallback();
                    #endif // end of #if (USING_CAN_1_ERROR_CALLBACK == 1)
                    
                    PortIOBufferClearCAN1ErrorInterruptFlag();
                }
            #endif // end of #if(USING_CAN_1_ERROR_EVENT == 1 || USING_CAN_1_ERROR_CALLBACK == 1)

            #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_RX_CALLBACK == 1)
                if(PortIOBufferGetCAN1RXInterruptFlag())
                {
                    #if (USING_CAN_1_RX_CALLBACK == 1)
                        CAN1InterruptRXCallback();
                    #endif // end of #if (USING_CAN_1_RX_CALLBACK == 1)

                    #if (USING_CAN_1_RX_EVENT == 1)
                        if(OS_RaiseEvent(CAN_1_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_CAN_1_RX_EVENT == 1)

                    PortIOBufferClearCAN1RXInterruptFlag();
                }
            #endif // end of #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_RX_CALLBACK == 1)

            #if (USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_TX_CALLBACK == 1)
                if(PortIOBufferGetCAN1TXDoneInterruptFlag())
                {
                    #if (USING_CAN_1_TX_CALLBACK == 1)
                        CAN1InterruptTXCallback();
                    #endif // end of #if (USING_CAN_1_TX_CALLBACK == 1)

                    #if (USING_CAN_1_TX_EVENT == 1)
                        if(OS_RaiseEvent(CAN_1_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_CAN_1_TX_EVENT == 1)

                    PortIOBufferClearCAN1TXInterruptFlag();
                }
            #endif // end of #if (USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_TX_CALLBACK == 1)

            #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1)

            PortIOBufferClearCAN1InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_CAN_1_RX_EVENT == 1 || USING_CAN_1_TX_EVENT == 1 || USING_CAN_1_ERROR_EVENT == 1 || USING_CAN_1_RX_CALLBACK == 1 || USING_CAN_1_TX_CALLBACK == 1 || USING_CAN_1_ERROR_CALLBACK == 1)
#endif // end of #if (USING_CAN_1_IO_BUFFER == 1)

#if (USING_CAN_2_IO_BUFFER == 1)
    void CAN2InterruptTXCallback(void);
    void CAN2InterruptRXCallback(BYTE *Buffer, UINT32 DataSizeInBytes);
    void CAN2InterruptErrorCallback(CAN_ERROR Error, UINT16 RXErrorCount, UINT16 TXErrorCount);

    #if(USING_CAN_2_IO_BUFFER == 1)
        extern IO_BUFFER gCAN2IOBuffer;
    #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

    BOOL UpdateCAN2RXBuffer(BYTE *Data, UINT32 DataBufferSize)
    {
        #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_CALLBACK == 1)
            UINT32 BytesRead;
        #endif // end of #if (USING_UART_1_IO_BUFFER == 1 || USING_UART_1_RX_CALLBACK == 1)

        BOOL SwapTask = FALSE;

        // if there is data in the hardware FIFO, read it into the IO_BUFFER,
        // or the user buffer if a read is pending.
        while(PortIOBufferDataAvailable(IO_BUFFER_CAN_2_ID) == TRUE)
        {
            // read the data in from the outside world, and then call appropriate methods or callbacks
            #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_CALLBACK == 1)
                BytesRead = PortIOBufferReadAvailableData(IO_BUFFER_CAN_2_ID, Data, DataBufferSize);
            #endif // end of #if (USING_UART_1_IO_BUFFER == 1 || USING_UART_1_RX_CALLBACK == 1)

            #if (USING_CAN_2_IO_BUFFER == 1)
                // now write the data to the IO Buffer
                if(BytesRead != 0)
                {
                    if(OS_WriteToIOBuffer(IO_BUFFER_CAN_2_ID, Data, BytesRead) == TRUE)
                        SwapTask = TRUE;

                    // now update the RX interrupt trigger level
                    OS_UpdateIOBufferRXInterruptTriggerLevel(IO_BUFFER_CAN_2_ID);
                }
            #endif // end of #if (USING_UART_1_IO_BUFFER == 1)

            #if (USING_CAN_2_RX_CALLBACK == 1)
                CAN2InterruptRXCallback(Data, BytesRead);
            #endif // end of #if (USING_CAN_2_RX_CALLBACK == 1)

            #if (USING_CAN_2_RX_DONE_EVENT == 1)
                if(OS_RaiseEvent(CAN_2_RX_DONE_EVENT) == TRUE)
                    SwapTask = TRUE;
            #endif // end of #if(USING_CAN_2_RX_DONE_EVENT == 1)
        }

        // clear the interrupt flag
        PortIOBufferClearCAN2RXInterruptFlag();

        return SwapTask;
    }

    OS_WORD *CAN2InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_CALLBACK == 1)
            BYTE Data[CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES];
        #endif // end of #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_CALLBACK == 1)

        #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_DONE_EVENT == 1 || USING_CAN_2_TX_DONE_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)
            BOOL SwapTask = FALSE;
        #endif // end of #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_DONE_EVENT == 1 || USING_CAN_2_TX_DONE_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)

        #if(USING_CAN_2_ERROR_EVENT == 1 || USING_CAN_2_ERROR_CALLBACK == 1)
            if(PortIOBufferGetCAN2ErrorInterruptFlag())
            {
                #if (USING_CAN_2_ERROR_EVENT == 1)
                    if(OS_RaiseEvent(CAN_2_ERROR_EVENT) == TRUE)
                        SwapTask = TRUE;
                #endif // end of #if (USING_CAN_2_ERROR_EVENT == 1)

                #if (USING_CAN_2_ERROR_CALLBACK == 1)
                    CAN2InterruptErrorCallback((CAN_ERROR)PortIOBufferGetErrorStatus(IO_BUFFER_CAN_2_ID), (UINT16)CANGetRxErrorCount(CAN2), (UINT16)CANGetTxErrorCount(CAN2));
                #endif // end of #if (USING_CAN_2_ERROR_CALLBACK == 1)

                PortIOBufferClearCAN2ErrorInterruptFlag();
            }
        #endif // end of #if(USING_CAN_2_ERROR_EVENT == 1 || USING_CAN_2_ERROR_CALLBACK == 1)

        #if (USING_CAN_2_RX_DONE_EVENT == 1 || USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_CALLBACK == 1)
            SwapTask = UpdateCAN2RXBuffer(Data, CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES);
        #endif // end of #if (USING_CAN_2_RX_DONE_EVENT == 1 || USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_CALLBACK == 1)

        #if (USING_CAN_2_TX_DONE_EVENT == 1 || USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_TX_CALLBACK == 1)
            if(PortIOBufferGetCAN2TXDoneInterruptFlag())
            {
                if(gCAN2IOBuffer.TXState == IO_BUFFER_WRITE || gCAN2IOBuffer.TXState == IO_BUFFER_ASYNC_WRITE)
                {
                    #if (USING_CAN_2_IO_BUFFER == 1)
                        #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                            // now if there is data to write out the port, do so
                            if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN2IOBuffer) != 0)
                            {
                                UINT32 BytesRead;

                                while(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN2IOBuffer) != 0 && PortIOBufferGetTXSpaceAvailable(IO_BUFFER_CAN_2_ID) >= sizeof(Data))
                                {
                                    // read 1 byte from the IO_BUFFER which will be TX'ed out!
                                    BytesRead = OS_ReadFromIOBuffer(&gCAN2IOBuffer, Data, sizeof(Data), sizeof(Data));

                                    // otherwise write the data out the hardware
                                    if(PortIOBufferWriteData(IO_BUFFER_CAN_2_ID, Data, BytesRead) == FALSE)
                                    {
                                        gCAN2IOBuffer.TXState = IO_BUFFER_HARDWARE_ERROR;

                                        // turn interrupts off
                                        PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_CAN_2_ID, 0);
                                    }
                                    else
                                    {
                                        // if it equals zero we don't want to change the level yet, we need the ISR to fire one more time
                                        if(OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN2IOBuffer) != 0)
                                            PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_CAN_2_ID, OS_IOBufferGetNumberOfBytesLeftToTransmit(&gCAN2IOBuffer));
                                    }
                                }
                            }
                            else
                            {
                                // we need to stop the TX'ing
                                PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_CAN_2_ID, 0);

                                // only if the buffer was in non Async mode, do we add the TASK
                                // to the ready queue
                                if(gCAN2IOBuffer.TXState == IO_BUFFER_WRITE)
                                {
                                    // now add the TASK to the ready queue
                                    if(OS_RemoveTaskFromIOBufferBlockedList(&gCAN2IOBuffer) == TRUE)
                                        SwapTask = TRUE;
                                }

                                // set the state to open now that the TX is done
                                gCAN2IOBuffer.TXState = IO_BUFFER_OPEN;
                            }
                        #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
                    #endif // end of #if (USING_CAN_2_IO_BUFFER == 1)

                    #if (USING_CAN_2_TX_CALLBACK == 1)
                        CAN2InterruptTXCallback();
                    #endif // end of #if (USING_CAN_2_TX_CALLBACK == 1)

                    #if (USING_CAN_2_TX_DONE_EVENT == 1)
                        if(OS_RaiseEvent(CAN_2_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_CAN_2_TX_DONE_EVENT == 1)
                }

                PortIOBufferClearCAN2TXInterruptFlag();
            }
        #endif // end of #if (USING_CAN_2_TX_DONE_EVENT == 1 || USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_TX_CALLBACK == 1)

        #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_DONE_EVENT == 1 || USING_CAN_2_TX_DONE_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)
            if(SwapTask == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if (USING_CAN_2_IO_BUFFER == 1 || USING_CAN_2_RX_DONE_EVENT == 1 || USING_CAN_2_TX_DONE_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)

        PortIOBufferClearCAN2InterruptFlag();

        return CurrentTaskStackPointer;
    }
#else
    #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1 || USING_CAN_2_RX_CALLBACK == 1 || USING_CAN_2_TX_CALLBACK == 1 || USING_CAN_2_ERROR_CALLBACK == 1)
        void CAN2InterruptTXCallback(void);
        void CAN2InterruptRXCallback(void);
        void CAN2InterruptErrorCallback(void);
        
        OS_WORD *CAN2InterruptHandler(OS_WORD *CurrentTaskStackPointer)
        {
            #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)
                BOOL SwapTask = FALSE;
            #endif // end of #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)

            #if(USING_CAN_2_ERROR_EVENT == 1 || USING_CAN_2_ERROR_CALLBACK == 1)
                if(PortIOBufferGetCAN2ErrorInterruptFlag())
                {
                    #if (USING_CAN_2_ERROR_EVENT == 1)
                        if(OS_RaiseEvent(CAN_2_ERROR_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if (USING_CAN_2_ERROR_EVENT == 1)
                        
                    #if (USING_CAN_2_ERROR_CALLBACK == 1)
                        CAN2InterruptErrorCallback();
                    #endif // end of #if (USING_CAN_2_ERROR_CALLBACK == 1)
                    
                    PortIOBufferClearCAN2ErrorInterruptFlag();
                }
            #endif // end of #if(USING_CAN_2_ERROR_EVENT == 1 || USING_CAN_2_ERROR_CALLBACK == 1)

            #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_RX_CALLBACK == 1)
                if(PortIOBufferGetCAN2RXInterruptFlag())
                {
                    #if (USING_CAN_2_RX_CALLBACK == 1)
                        CAN2InterruptRXCallback();
                    #endif // end of #if (USING_CAN_2_RX_CALLBACK == 1)

                    #if (USING_CAN_2_RX_EVENT == 1)
                        if(OS_RaiseEvent(CAN_2_RX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_CAN_2_RX_EVENT == 1)

                    PortIOBufferClearCAN2RXInterruptFlag();
                }
            #endif // end of #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_RX_CALLBACK == 1)

            #if (USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_TX_CALLBACK == 1)
                if(PortIOBufferGetCAN2TXDoneInterruptFlag())
                {
                    #if (USING_CAN_2_TX_CALLBACK == 1)
                        CAN2InterruptTXCallback();
                    #endif // end of #if (USING_CAN_2_TX_CALLBACK == 1)

                    #if (USING_CAN_2_TX_EVENT == 1)
                        if(OS_RaiseEvent(CAN_2_TX_DONE_EVENT) == TRUE)
                            SwapTask = TRUE;
                    #endif // end of #if(USING_CAN_2_TX_EVENT == 1)

                    PortIOBufferClearCAN2TXInterruptFlag();
                }
            #endif // end of #if (USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_TX_CALLBACK == 1)

            #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)
                if(SwapTask == TRUE)
                    CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
            #endif // end of #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1)

            PortIOBufferClearCAN2InterruptFlag();

            return CurrentTaskStackPointer;
        }
    #endif // end of #if (USING_CAN_2_RX_EVENT == 1 || USING_CAN_2_TX_EVENT == 1 || USING_CAN_2_ERROR_EVENT == 1 || USING_CAN_2_RX_CALLBACK == 1 || USING_CAN_2_TX_CALLBACK == 1 || USING_CAN_2_ERROR_CALLBACK == 1)
#endif // end of #if (USING_CAN_2_IO_BUFFER == 1)
