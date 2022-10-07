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

#include <string.h>

#include "../NexOS/IOBuffer/IOBuffer.h"
#include "IOBufferPort.h"

#if (USING_IO_BUFFERS == 1)
    #if (USING_UART_1_IO_BUFFER == 1)
        UART_FIFO_MODE gUART1FifoMode = UART_INTERRUPT_ON_RX_FULL | UART_INTERRUPT_ON_TX_NOT_FULL;
    #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

    #if (USING_UART_2_IO_BUFFER == 1)
        UART_FIFO_MODE gUART2FifoMode = UART_INTERRUPT_ON_RX_FULL | UART_INTERRUPT_ON_TX_NOT_FULL;
    #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

    #if (USING_UART_3_IO_BUFFER == 1)
        UART_FIFO_MODE gUART3FifoMode = UART_INTERRUPT_ON_RX_FULL | UART_INTERRUPT_ON_TX_NOT_FULL;
    #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

    #if (USING_UART_4_IO_BUFFER == 1)
        UART_FIFO_MODE gUART4FifoMode = UART_INTERRUPT_ON_RX_FULL | UART_INTERRUPT_ON_TX_NOT_FULL;
    #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

    #if (USING_UART_5_IO_BUFFER == 1)
        UART_FIFO_MODE gUART5FifoMode = UART_INTERRUPT_ON_RX_FULL | UART_INTERRUPT_ON_TX_NOT_FULL;
    #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

    #if (USING_UART_6_IO_BUFFER == 1)
        UART_FIFO_MODE gUART6FifoMode = UART_INTERRUPT_ON_RX_FULL | UART_INTERRUPT_ON_TX_NOT_FULL;
    #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

    BOOL PortIOBufferInitializeInterrupt(IO_BUFFER_ID IOBufferID)
    {
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    // configure the core UART1 interrupt
                    INTSetVectorPriority(INT_UART_1_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_UART_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_U1TX, INT_DISABLED);
                    
                    // clear all three interrupt sources
                    PortIOBufferClearUART1RXInterruptFlag();
                    PortIOBufferClearUART1TXInterruptFlag();
                    PortIOBufferClearUART1ErrorInterruptFlag();
                    
                    // now only enable the RX and error interrupts
                    INTEnable(INT_U1RX, INT_ENABLED);
                    INTEnable(INT_U1E, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    // configure the core UART2 interrupt
                    INTSetVectorPriority(INT_UART_2_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_UART_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_U2TX, INT_DISABLED);
                    
                    // clear all three interrupt sources
                    PortIOBufferClearUART2RXInterruptFlag();
                    PortIOBufferClearUART2TXInterruptFlag();
                    PortIOBufferClearUART2ErrorInterruptFlag();
                    
                    // now only enable the RX and error interrupts
                    INTEnable(INT_U2RX, INT_ENABLED);
                    INTEnable(INT_U2E, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    // configure the core UART3 interrupt
                    INTSetVectorPriority(INT_UART_3_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_UART_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_U3TX, INT_DISABLED);
                    
                    // clear all three interrupt sources
                    PortIOBufferClearUART3RXInterruptFlag();
                    PortIOBufferClearUART3TXInterruptFlag();
                    PortIOBufferClearUART3ErrorInterruptFlag();
                    
                    // now only enable the RX and error interrupts
                    INTEnable(INT_U3RX, INT_ENABLED);
                    INTEnable(INT_U3E, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    // configure the core UART4 interrupt
                    INTSetVectorPriority(INT_UART_4_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_UART_4_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_U4TX, INT_DISABLED);
                    
                    // clear all three interrupt sources
                    PortIOBufferClearUART4RXInterruptFlag();
                    PortIOBufferClearUART4TXInterruptFlag();
                    PortIOBufferClearUART4ErrorInterruptFlag();
                    
                    // now only enable the RX and error interrupts
                    INTEnable(INT_U4RX, INT_ENABLED);
                    INTEnable(INT_U4E, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    // configure the core UART5 interrupt
                    INTSetVectorPriority(INT_UART_5_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_UART_5_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_U5TX, INT_DISABLED);
                    
                    // clear all three interrupt sources
                    PortIOBufferClearUART5RXInterruptFlag();
                    PortIOBufferClearUART5TXInterruptFlag();
                    PortIOBufferClearUART5ErrorInterruptFlag();
                    
                    // now only enable the RX and error interrupts
                    INTEnable(INT_U5RX, INT_ENABLED);
                    INTEnable(INT_U5E, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    // configure the core UART6 interrupt
                    INTSetVectorPriority(INT_UART_6_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_UART_6_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_U6TX, INT_DISABLED);
                    
                    // clear all three interrupt sources
                    PortIOBufferClearUART6RXInterruptFlag();
                    PortIOBufferClearUART6TXInterruptFlag();
                    PortIOBufferClearUART6ErrorInterruptFlag();
                    
                    // now only enable the RX and error interrupts
                    INTEnable(INT_U6RX, INT_ENABLED);
                    INTEnable(INT_U6E, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

            #if (USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    // configure the core CAN 1 interrupt
                    INTSetVectorPriority(INT_CAN_1_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_CAN_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_CAN1, INT_DISABLED);
                    
                    // now clear the interrupt flag
                    PortIOBufferClearCAN1ErrorInterruptFlag();
                    PortIOBufferClearCAN1RXInterruptFlag();
                    PortIOBufferClearCAN1TXInterruptFlag();
                    PortIOBufferClearCAN1InterruptFlag();
                    
                    // setup the interrupt events the RX channel error
                    CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_OVERFLOW, TRUE);
                    
                    // now set the events to interrupt on
                    CANEnableModuleEvent(CAN1, CAN_RX_OVERFLOW_EVENT | CAN_SYSTEM_ERROR_EVENT | CAN_BUS_ERROR_EVENT, TRUE);
                    
                    // now enable the CAN 1 interrupt
                    INTEnable(INT_CAN1, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if (USING_CAN_1_IO_BUFFER == 1)

            #if (USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    // configure the core CAN 2 interrupt
                    INTSetVectorPriority(INT_CAN_2_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_CAN_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);

                    INTEnable(INT_CAN2, INT_DISABLED);
                    
                    // now clear the interrupt flag
                    PortIOBufferClearCAN2ErrorInterruptFlag();
                    PortIOBufferClearCAN2RXInterruptFlag();
                    PortIOBufferClearCAN2TXInterruptFlag();
                    PortIOBufferClearCAN2InterruptFlag();
                    
                    // setup the interrupt events the RX channel error
                    CANEnableChannelEvent(CAN2, CAN_CHANNEL1, CAN_RX_CHANNEL_OVERFLOW, TRUE);
                    
                    // now set the events to interrupt on
                    CANEnableModuleEvent(CAN2, CAN_RX_OVERFLOW_EVENT | CAN_SYSTEM_ERROR_EVENT | CAN_BUS_ERROR_EVENT, TRUE);
                    
                    // now enable the CAN 1 interrupt
                    INTEnable(INT_CAN2, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if (USING_CAN_2_IO_BUFFER == 1)

            #if (USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    // configure the core ADC 1 interrupt
                    INTSetVectorPriority(INT_ADC_VECTOR, IO_BUFFER_INTERRUPT_HANDLER_PRIORITY);
                    INTSetVectorSubPriority(INT_ADC_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
                    
                    // now clear the interrupt flag
                    PortIOBufferClearADC1InterruptFlag();

                    // now enable the ADC 1 interrupt
                    INTEnable(INT_AD1, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)
                
            default:
            {
                return FALSE;
            }
        }
        
        return TRUE;
    }

    BOOL PortIOBufferDataAvailable(IO_BUFFER_ID IOBufferID)
    {
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    return (BOOL)UARTReceivedDataIsAvailable(UART1);
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    return (BOOL)UARTReceivedDataIsAvailable(UART2);
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    return (BOOL)UARTReceivedDataIsAvailable(UART3);
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    return (BOOL)UARTReceivedDataIsAvailable(UART4);
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    return (BOOL)UARTReceivedDataIsAvailable(UART5);
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    return (BOOL)UARTReceivedDataIsAvailable(UART6);
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

            #if(USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    CAN_MODULE_EVENT RXEvent = CANGetModuleEvent(CAN1);
                    
                    return (BOOL)((RXEvent & CAN_RX_EVENT) != 0);
                }
            #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

            #if(USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    CAN_MODULE_EVENT RXEvent = CANGetModuleEvent(CAN2);
                    
                    return (BOOL)((RXEvent & CAN_RX_EVENT) != 0);
                }
            #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

            #if(USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    return (BOOL)(PortIOBufferGetADC1InterruptFlag());
                }
            #endif // end of #if(USING_ADC_1_IO_BUFFER == 1)
                
            default:
            {
                return FALSE;
            }
        }
    }

    UINT32 PortIOBufferReadAvailableData(IO_BUFFER_ID IOBufferID, BYTE *DataBuffer, UINT32 DataBufferSize)
    {
        // this method needs to read the data into DataBuffer, and return
        // how many bytes were read.  DataBuffer is UART_1_ISR_DATA_BUFFER_SIZE_IN_BYTES large.
        UINT32 BytesRead = 0;
        
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    while(UARTReceivedDataIsAvailable(UART1))
                    {
                        DataBuffer[BytesRead++] = UARTGetDataByte(UART1);
                    }

                    break;
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    while(UARTReceivedDataIsAvailable(UART2))
                    {
                        DataBuffer[BytesRead++] = UARTGetDataByte(UART2);
                    }

                    break;
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    while(UARTReceivedDataIsAvailable(UART3))
                    {
                        DataBuffer[BytesRead++] = UARTGetDataByte(UART3);
                    }

                    break;
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    while(UARTReceivedDataIsAvailable(UART4))
                    {
                        DataBuffer[BytesRead++] = UARTGetDataByte(UART4);
                    }

                    break;
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    while(UARTReceivedDataIsAvailable(UART5))
                    {
                        DataBuffer[BytesRead++] = UARTGetDataByte(UART5);
                    }

                    break;
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    while(UARTReceivedDataIsAvailable(UART6))
                    {
                        DataBuffer[BytesRead++] = UARTGetDataByte(UART6);
                    }

                    break;
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

            #if(USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    // now go through each channel and see if a message is present
                    CANRxMessageBuffer *CANPacket;
                    
                    // while the channel has data in it, read it in
                    while(CANGetChannelEvent(CAN1, CAN_CHANNEL1) & CAN_RX_CHANNEL_NOT_EMPTY && DataBufferSize >= sizeof(CANRxMessageBuffer))
                    {
                        // get a handle on the data
                        CANPacket = CANGetRxMessage(CAN1, CAN_CHANNEL1);

                        // update the CAN hardware buffer
                        CANUpdateChannel(CAN1, CAN_CHANNEL1);

                        // copy the data over to the temp buffer
                        memcpy((void*)DataBuffer, (const void*)CANPacket, sizeof(CANRxMessageBuffer));

                        // increase our pointer, and bytes read
                        DataBuffer += sizeof(CANRxMessageBuffer);
                        BytesRead += sizeof(CANRxMessageBuffer);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

            #if(USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    // now go through each channel and see if a message is present
                    CANRxMessageBuffer *CANPacket;
                    
                    // while the channel has data in it, read it in
                    while(CANGetChannelEvent(CAN2, CAN_CHANNEL1) & CAN_RX_CHANNEL_NOT_EMPTY && DataBufferSize >= sizeof(CANRxMessageBuffer))
                    {
                        // get a handle on the data
                        CANPacket = CANGetRxMessage(CAN2, CAN_CHANNEL1);

                        // update the CAN hardware buffer
                        CANUpdateChannel(CAN2, CAN_CHANNEL1);

                        // copy the data over to the temp buffer
                        memcpy((void*)DataBuffer, (const void*)CANPacket, sizeof(CANRxMessageBuffer));

                        // increase our pointer, and bytes read
                        DataBuffer += sizeof(CANRxMessageBuffer);
                        BytesRead += sizeof(CANRxMessageBuffer);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

            #if (USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    UINT16 *ADCDataBuffer = (UINT16*)DataBuffer;
                    
                    // we will always configure up the ADC to run 16 samples
                    *ADCDataBuffer++ = ADC1BUF0;
                    *ADCDataBuffer++ = ADC1BUF1;
                    *ADCDataBuffer++ = ADC1BUF2;
                    *ADCDataBuffer++ = ADC1BUF3;
                    *ADCDataBuffer++ = ADC1BUF4;
                    *ADCDataBuffer++ = ADC1BUF5;
                    *ADCDataBuffer++ = ADC1BUF6;
                    *ADCDataBuffer++ = ADC1BUF7;
                    *ADCDataBuffer++ = ADC1BUF8;
                    *ADCDataBuffer++ = ADC1BUF9;
                    *ADCDataBuffer++ = ADC1BUFA;
                    *ADCDataBuffer++ = ADC1BUFB;
                    *ADCDataBuffer++ = ADC1BUFC;
                    *ADCDataBuffer++ = ADC1BUFD;
                    *ADCDataBuffer++ = ADC1BUFE;
                    *ADCDataBuffer = ADC1BUFF;
                    
                    BytesRead = 32;
                    
                    break;
                }
            #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)

            default:
            {
                return 0;
            }
        }

        return BytesRead;
    }
    
    UINT32 PortIOBufferGetErrorStatus(IO_BUFFER_ID IOBufferID)
    {
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    UART_ERROR Error = NO_UART_ERROR;
                    UART_LINE_STATUS Status = UARTGetLineStatus(UART1);
                    
                    // get the error bits
                    if(Status & UART_PARITY_ERROR)
                        Error |= PARITY_ERROR;
                    
                    if(Status & UART_FRAMING_ERROR)
                        Error |= FRAMING_ERROR;
                    
                    if(Status & UART_OVERRUN_ERROR)
                        Error |= OVERRUN_ERROR;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    UART_ERROR Error = NO_UART_ERROR;
                    UART_LINE_STATUS Status = UARTGetLineStatus(UART2);
                    
                    // get the error bits
                    if(Status & UART_PARITY_ERROR)
                        Error |= PARITY_ERROR;
                    
                    if(Status & UART_FRAMING_ERROR)
                        Error |= FRAMING_ERROR;
                    
                    if(Status & UART_OVERRUN_ERROR)
                        Error |= OVERRUN_ERROR;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    UART_ERROR Error = NO_UART_ERROR;
                    UART_LINE_STATUS Status = UARTGetLineStatus(UART3);
                    
                    // get the error bits
                    if(Status & UART_PARITY_ERROR)
                        Error |= PARITY_ERROR;
                    
                    if(Status & UART_FRAMING_ERROR)
                        Error |= FRAMING_ERROR;
                    
                    if(Status & UART_OVERRUN_ERROR)
                        Error |= OVERRUN_ERROR;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    UART_ERROR Error = NO_UART_ERROR;
                    UART_LINE_STATUS Status = UARTGetLineStatus(UART4);
                    
                    // get the error bits
                    if(Status & UART_PARITY_ERROR)
                        Error |= PARITY_ERROR;
                    
                    if(Status & UART_FRAMING_ERROR)
                        Error |= FRAMING_ERROR;
                    
                    if(Status & UART_OVERRUN_ERROR)
                        Error |= OVERRUN_ERROR;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    UART_ERROR Error = NO_UART_ERROR;
                    UART_LINE_STATUS Status = UARTGetLineStatus(UART5);
                    
                    // get the error bits
                    if(Status & UART_PARITY_ERROR)
                        Error |= PARITY_ERROR;
                    
                    if(Status & UART_FRAMING_ERROR)
                        Error |= FRAMING_ERROR;
                    
                    if(Status & UART_OVERRUN_ERROR)
                        Error |= OVERRUN_ERROR;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    UART_ERROR Error = NO_UART_ERROR;
                    UART_LINE_STATUS Status = UARTGetLineStatus(UART6);
                    
                    // get the error bits
                    if(Status & UART_PARITY_ERROR)
                        Error |= PARITY_ERROR;
                    
                    if(Status & UART_FRAMING_ERROR)
                        Error |= FRAMING_ERROR;
                    
                    if(Status & UART_OVERRUN_ERROR)
                        Error |= OVERRUN_ERROR;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

            #if(USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    CAN_ERROR Error = NO_CAN_ERROR;
                    CAN_MODULE_EVENT Status = CANGetModuleEvent(CAN1);
                    CAN_ERROR_STATE ErrorState = CANGetErrorState(CAN1);
                    
                    // get the error bits
                    if(Status & CAN_RX_OVERFLOW_EVENT)
                        Error |= CAN_RX_OVERFLOW_EVENT_ERROR;
                    
                    if(Status & CAN_SYSTEM_ERROR_EVENT)
                        Error |= CAN_SYSTEM_ERROR_EVENT_ERROR;
                    
                    if(Status & CAN_BUS_ERROR_EVENT)
                        Error |= CAN_BUS_ERROR_EVENT_ERROR;
                    
                    if(Status & CAN_INVALID_RX_MESSAGE_EVENT)
                        Error |= CAN_INVALID_RX_MESSAGE_EVENT_ERROR;
                    
                    if(ErrorState & CAN_RX_BUS_PASSIVE_STATE)
                        Error |= CAN_RX_PASSIVE_ERROR_STATE;
                    
                    if(ErrorState & CAN_TX_BUS_PASSIVE_STATE)
                        Error |= CAN_TX_PASSIVE_ERROR_STATE;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

            #if(USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    CAN_ERROR Error = NO_CAN_ERROR;
                    CAN_MODULE_EVENT Status = CANGetModuleEvent(CAN2);
                    CAN_ERROR_STATE ErrorState = CANGetErrorState(CAN2);
                    
                    // get the error bits
                    if(Status & CAN_RX_OVERFLOW_EVENT)
                        Error |= CAN_RX_OVERFLOW_EVENT_ERROR;
                    
                    if(Status & CAN_SYSTEM_ERROR_EVENT)
                        Error |= CAN_SYSTEM_ERROR_EVENT_ERROR;
                    
                    if(Status & CAN_BUS_ERROR_EVENT)
                        Error |= CAN_BUS_ERROR_EVENT_ERROR;
                    
                    if(Status & CAN_INVALID_RX_MESSAGE_EVENT)
                        Error |= CAN_INVALID_RX_MESSAGE_EVENT_ERROR;
                    
                    if(ErrorState & CAN_RX_BUS_PASSIVE_STATE)
                        Error |= CAN_RX_PASSIVE_ERROR_STATE;
                    
                    if(ErrorState & CAN_TX_BUS_PASSIVE_STATE)
                        Error |= CAN_TX_PASSIVE_ERROR_STATE;
                    
                    return (UINT32)Error;
                }
            #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

            #if (USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    return 0;
                }
            #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)

            default:
            {
                return 0;
            }
        }
    }
    
    UINT32 PortIOBufferGetTXSpaceAvailable(IO_BUFFER_ID IOBufferID)
    {
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    if(UARTTransmissionHasCompleted(UART1) == TRUE)
                        return UART_1_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                    else
                        return 1; // we can only say for sure that there is 1 byte available
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    if(UARTTransmissionHasCompleted(UART2) == TRUE)
                        return UART_2_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                    else
                        return 1; // we can only say for sure that there is 1 byte available
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    if(UARTTransmissionHasCompleted(UART3) == TRUE)
                        return UART_3_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                    else
                        return 1; // we can only say for sure that there is 1 byte available
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    if(UARTTransmissionHasCompleted(UART4) == TRUE)
                        return UART_4_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                    else
                        return 1; // we can only say for sure that there is 1 byte available
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    if(UARTTransmissionHasCompleted(UART5) == TRUE)
                        return UART_5_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                    else
                        return 1; // we can only say for sure that there is 1 byte available
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    if(UARTTransmissionHasCompleted(UART6) == TRUE)
                        return UART_6_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                    else
                        return 1; // we can only say for sure that there is 1 byte available
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

            #if(USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    CAN_CHANNEL_EVENT CANChannelEvent = CANGetChannelEvent(CAN1, CAN_CHANNEL0);
                    
                    if(CANChannelEvent & CAN_TX_CHANNEL_EMPTY)
                    {
                        // it is empty
                        return CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES * CAN_MAXIMUM_PACKETS_PER_CHANNEL;
                    }
                    else
                    {
                        if(CANChannelEvent & CAN_TX_CHANNEL_HALF_EMPTY)
                        {
                            // it is half empty
                            return CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES * (CAN_MAXIMUM_PACKETS_PER_CHANNEL / 2);
                        }
                        else
                        {
                            if(CANChannelEvent & CAN_TX_CHANNEL_NOT_FULL)
                            {
                                // we can only say for certain that there is 1 empty spot
                                return CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

            #if(USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    CAN_CHANNEL_EVENT CANChannelEvent = CANGetChannelEvent(CAN2, CAN_CHANNEL0);
                    
                    if(CANChannelEvent & CAN_TX_CHANNEL_EMPTY)
                    {
                        // it is empty
                        return CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES * CAN_MAXIMUM_PACKETS_PER_CHANNEL;
                    }
                    else
                    {
                        if(CANChannelEvent & CAN_TX_CHANNEL_HALF_EMPTY)
                        {
                            // it is half empty
                            return CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES * (CAN_MAXIMUM_PACKETS_PER_CHANNEL / 2);
                        }
                        else
                        {
                            if(CANChannelEvent & CAN_TX_CHANNEL_NOT_FULL)
                            {
                                // we can only say for certain that there is 1 empty spot
                                return CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES;
                            }
                            else
                            {
                                return 0;
                            }
                        }
                    }
                }
            #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

            #if (USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    return 0;
                }
            #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)

            default:
            {
                return 0;
            }
        }
    }
    
    BOOL PortIOBufferWriteData(IO_BUFFER_ID IOBufferID, BYTE *Data, UINT32 NumberOfBytesToWrite)
    {
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    // while there is still data to send, send it!
                    while(NumberOfBytesToWrite != 0)
                    {
                        // wait until bytes can be TX'ed
                        while(UARTTransmitterIsReady(UART1) == FALSE);

                        // transmit the data
                        UARTSendDataByte(UART1, *Data++);
                        
                        // decrement how many bytes we are looking to send
                        NumberOfBytesToWrite--;
                    }

                    break;
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    // while there is still data to send, send it!
                    while(NumberOfBytesToWrite != 0)
                    {
                        // wait until bytes can be TX'ed
                        while(UARTTransmitterIsReady(UART2) == FALSE);

                        // transmit the data
                        UARTSendDataByte(UART2, *Data++);
                        
                        // decrement how many bytes we are looking to send
                        NumberOfBytesToWrite--;
                    }

                    break;
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    // while there is still data to send, send it!
                    while(NumberOfBytesToWrite != 0)
                    {
                        // wait until bytes can be TX'ed
                        while(UARTTransmitterIsReady(UART3) == FALSE);

                        // transmit the data
                        UARTSendDataByte(UART3, *Data++);
                        
                        // decrement how many bytes we are looking to send
                        NumberOfBytesToWrite--;
                    }

                    break;
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    // while there is still data to send, send it!
                    while(NumberOfBytesToWrite != 0)
                    {
                        // wait until bytes can be TX'ed
                        while(UARTTransmitterIsReady(UART4) == FALSE);

                        // transmit the data
                        UARTSendDataByte(UART4, *Data++);
                        
                        // decrement how many bytes we are looking to send
                        NumberOfBytesToWrite--;
                    }

                    break;
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    // while there is still data to send, send it!
                    while(NumberOfBytesToWrite != 0)
                    {
                        // wait until bytes can be TX'ed
                        while(UARTTransmitterIsReady(UART5) == FALSE);

                        // transmit the data
                        UARTSendDataByte(UART5, *Data++);
                        
                        // decrement how many bytes we are looking to send
                        NumberOfBytesToWrite--;
                    }

                    break;
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    // while there is still data to send, send it!
                    while(NumberOfBytesToWrite != 0)
                    {
                        // wait until bytes can be TX'ed
                        while(UARTTransmitterIsReady(UART6) == FALSE);

                        // transmit the data
                        UARTSendDataByte(UART6, *Data++);
                        
                        // decrement how many bytes we are looking to send
                        NumberOfBytesToWrite--;
                    }

                    break;
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

            #if(USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    CANTxMessageBuffer *Packet;
                    
                    do
                    {
                        Packet = CANGetTxMessageBuffer(CAN1, CAN_CHANNEL0);
                        
                        // did we get it?  We should have, but check anyways
                        if(Packet == (CANTxMessageBuffer*)NULL)
                            return FALSE;

                        // copy the packet over and transmit it
                        memcpy((void*)Packet, (const void*)Data, sizeof(CANTxMessageBuffer));

                        CANUpdateChannel(CAN1, CAN_CHANNEL0);

                        CANFlushTxChannel(CAN1, CAN_CHANNEL0);
                        
                        NumberOfBytesToWrite -= sizeof(CANTxMessageBuffer);
                    }
                    while(NumberOfBytesToWrite);
                    
                    break;
                }
            #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

            #if(USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    CANTxMessageBuffer *Packet;
                    
                    do
                    {
                        Packet = CANGetTxMessageBuffer(CAN2, CAN_CHANNEL0);
                        
                        // did we get it?  We should have, but check anyways
                        if(Packet == (CANTxMessageBuffer*)NULL)
                            return FALSE;

                        // copy the packet over and transmit it
                        memcpy((void*)Packet, (const void*)Data, sizeof(CANTxMessageBuffer));

                        CANUpdateChannel(CAN2, CAN_CHANNEL0);

                        CANFlushTxChannel(CAN2, CAN_CHANNEL0);
                        
                        NumberOfBytesToWrite -= sizeof(CANTxMessageBuffer);
                    }
                    while(NumberOfBytesToWrite);
                    
                    break;
                }
            #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

            #if (USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    break;
                }
            #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)

            default:
            {
                return FALSE;
            }
        }

        return TRUE;
    }
    
    void PortIOBufferSetRXInterruptTriggerLevel(IO_BUFFER_ID IOBufferID, UINT32 NumberOfBytesExpectedToBeRead)
    {
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    // first disable the interrupt and then clear the flag
                    INTEnable(INT_U1RX, INT_DISABLED);
                    
                    // clear out any current RX mode
                    gUART1FifoMode &= ~(UART_INTERRUPT_ON_RX_HALF_FULL | UART_INTERRUPT_ON_RX_3_QUARTER_FULL | UART_INTERRUPT_ON_RX_FULL);
                    
                    switch(NumberOfBytesExpectedToBeRead)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                            // trigger on 1 byte
                            gUART1FifoMode |= UART_INTERRUPT_ON_RX_NOT_EMPTY;
                            
                            break;
                        }
                        
                        case 4:
                        case 5:
                        {
                            // this is for 1/2 full (4 bytes or more)
                            gUART1FifoMode |= UART_INTERRUPT_ON_RX_HALF_FULL;
                            
                            break;
                        }
                        
                        case 6:
                        case 7:
                        {
                            // this is for 3/4 full (6 bytes or more)
                            gUART1FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                            
                            break;
                        }
                        
                        case 8:
                        {
                            // this is for full 8 bytes only
                            gUART1FifoMode |= UART_INTERRUPT_ON_RX_FULL;
                                                        
                            break;
                        }
                        
                        case IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        case IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        default:
                        {
                            // this is for full (9 bytes or more)
                            gUART1FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                                                        
                            break;
                        }
                    }
                    
                    // now set the mode
                    UARTSetFifoMode(UART1, gUART1FifoMode);

                    // clear out the RX interrupt flag if set
                    INTClearFlag(INT_U1RX);
                    
                    // now re enable the interrupt
                    INTEnable(INT_U1RX, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    // first disable the interrupt and then clear the flag
                    INTEnable(INT_U2RX, INT_DISABLED);
                    
                    // clear out any current RX mode
                    gUART2FifoMode &= ~(UART_INTERRUPT_ON_RX_HALF_FULL | UART_INTERRUPT_ON_RX_3_QUARTER_FULL | UART_INTERRUPT_ON_RX_FULL);
                    
                    switch(NumberOfBytesExpectedToBeRead)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                            // trigger on 1 byte
                            gUART2FifoMode |= UART_INTERRUPT_ON_RX_NOT_EMPTY;
                            
                            break;
                        }
                        
                        case 4:
                        case 5:
                        {
                            // this is for 1/2 full (4 bytes or more)
                            gUART2FifoMode |= UART_INTERRUPT_ON_RX_HALF_FULL;
                            
                            break;
                        }
                        
                        case 6:
                        case 7:
                        {
                            // this is for 3/4 full (6 bytes or more)
                            gUART2FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                            
                            break;
                        }
                        
                        case 8:
                        {
                            // this is for full 8 bytes only
                            gUART2FifoMode |= UART_INTERRUPT_ON_RX_FULL;
                                                        
                            break;
                        }
                        
                        case IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        case IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        default:
                        {
                            // this is for full (9 bytes or more)
                            gUART2FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                                                        
                            break;
                        }
                    }
                    
                    // now set the mode
                    UARTSetFifoMode(UART2, gUART2FifoMode);

                    // clear out the RX interrupt flag if set
                    INTClearFlag(INT_U2RX);
                    
                    // now re enable the interrupt
                    INTEnable(INT_U2RX, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    // first disable the interrupt and then clear the flag
                    INTEnable(INT_U3RX, INT_DISABLED);
                    
                    // clear out any current RX mode
                    gUART3FifoMode &= ~(UART_INTERRUPT_ON_RX_HALF_FULL | UART_INTERRUPT_ON_RX_3_QUARTER_FULL | UART_INTERRUPT_ON_RX_FULL);
                    
                    switch(NumberOfBytesExpectedToBeRead)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                            // trigger on 1 byte
                            gUART3FifoMode |= UART_INTERRUPT_ON_RX_NOT_EMPTY;
                            
                            break;
                        }
                        
                        case 4:
                        case 5:
                        {
                            // this is for 1/2 full (4 bytes or more)
                            gUART3FifoMode |= UART_INTERRUPT_ON_RX_HALF_FULL;
                            
                            break;
                        }
                        
                        case 6:
                        case 7:
                        {
                            // this is for 3/4 full (6 bytes or more)
                            gUART3FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                            
                            break;
                        }
                        
                        case 8:
                        {
                            // this is for full 8 bytes only
                            gUART3FifoMode |= UART_INTERRUPT_ON_RX_FULL;
                                                        
                            break;
                        }
                        
                        case IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        case IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        default:
                        {
                            // this is for full (9 bytes or more)
                            gUART3FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                                                        
                            break;
                        }
                    }
                    
                    // now set the mode
                    UARTSetFifoMode(UART3, gUART3FifoMode);

                    // clear out the RX interrupt flag if set
                    INTClearFlag(INT_U3RX);
                    
                    // now re enable the interrupt
                    INTEnable(INT_U3RX, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    // first disable the interrupt and then clear the flag
                    INTEnable(INT_U4RX, INT_DISABLED);
                    
                    // clear out any current RX mode
                    gUART4FifoMode &= ~(UART_INTERRUPT_ON_RX_HALF_FULL | UART_INTERRUPT_ON_RX_3_QUARTER_FULL | UART_INTERRUPT_ON_RX_FULL);
                    
                    switch(NumberOfBytesExpectedToBeRead)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                            // trigger on 1 byte
                            gUART4FifoMode |= UART_INTERRUPT_ON_RX_NOT_EMPTY;
                            
                            break;
                        }
                        
                        case 4:
                        case 5:
                        {
                            // this is for 1/2 full (4 bytes or more)
                            gUART4FifoMode |= UART_INTERRUPT_ON_RX_HALF_FULL;
                            
                            break;
                        }
                        
                        case 6:
                        case 7:
                        {
                            // this is for 3/4 full (6 bytes or more)
                            gUART4FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                            
                            break;
                        }
                        
                        case 8:
                        {
                            // this is for full 8 bytes only
                            gUART4FifoMode |= UART_INTERRUPT_ON_RX_FULL;
                                                        
                            break;
                        }
                        
                        case IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        case IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        default:
                        {
                            // this is for full (9 bytes or more)
                            gUART4FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                                                        
                            break;
                        }
                    }
                    
                    // now set the mode
                    UARTSetFifoMode(UART4, gUART4FifoMode);

                    // clear out the RX interrupt flag if set
                    INTClearFlag(INT_U4RX);
                    
                    // now re enable the interrupt
                    INTEnable(INT_U4RX, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    // first disable the interrupt and then clear the flag
                    INTEnable(INT_U5RX, INT_DISABLED);
                    
                    // clear out any current RX mode
                    gUART5FifoMode &= ~(UART_INTERRUPT_ON_RX_HALF_FULL | UART_INTERRUPT_ON_RX_3_QUARTER_FULL | UART_INTERRUPT_ON_RX_FULL);
                    
                    switch(NumberOfBytesExpectedToBeRead)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                            // trigger on 1 byte
                            gUART5FifoMode |= UART_INTERRUPT_ON_RX_NOT_EMPTY;
                            
                            break;
                        }
                        
                        case 4:
                        case 5:
                        {
                            // this is for 1/2 full (4 bytes or more)
                            gUART5FifoMode |= UART_INTERRUPT_ON_RX_HALF_FULL;
                            
                            break;
                        }
                        
                        case 6:
                        case 7:
                        {
                            // this is for 3/4 full (6 bytes or more)
                            gUART5FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                            
                            break;
                        }
                        
                        case 8:
                        {
                            // this is for full 8 bytes only
                            gUART5FifoMode |= UART_INTERRUPT_ON_RX_FULL;
                                                        
                            break;
                        }
                        
                        case IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        case IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        default:
                        {
                            // this is for full (9 bytes or more)
                            gUART5FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                                                        
                            break;
                        }
                    }
                    
                    // now set the mode
                    UARTSetFifoMode(UART5, gUART5FifoMode);

                    // clear out the RX interrupt flag if set
                    INTClearFlag(INT_U5RX);
                    
                    // now re enable the interrupt
                    INTEnable(INT_U5RX, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    // first disable the interrupt and then clear the flag
                    INTEnable(INT_U6RX, INT_DISABLED);
                    
                    // clear out any current RX mode
                    gUART6FifoMode &= ~(UART_INTERRUPT_ON_RX_HALF_FULL | UART_INTERRUPT_ON_RX_3_QUARTER_FULL | UART_INTERRUPT_ON_RX_FULL);
                    
                    switch(NumberOfBytesExpectedToBeRead)
                    {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        {
                            // trigger on 1 byte
                            gUART6FifoMode |= UART_INTERRUPT_ON_RX_NOT_EMPTY;
                            
                            break;
                        }
                        
                        case 4:
                        case 5:
                        {
                            // this is for 1/2 full (4 bytes or more)
                            gUART6FifoMode |= UART_INTERRUPT_ON_RX_HALF_FULL;
                            
                            break;
                        }
                        
                        case 6:
                        case 7:
                        {
                            // this is for 3/4 full (6 bytes or more)
                            gUART6FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                            
                            break;
                        }
                        
                        case 8:
                        {
                            // this is for full 8 bytes only
                            gUART6FifoMode |= UART_INTERRUPT_ON_RX_FULL;
                                                        
                            break;
                        }
                        
                        case IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        case IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL:
                        default:
                        {
                            // this is for full (9 bytes or more)
                            gUART6FifoMode |= UART_INTERRUPT_ON_RX_3_QUARTER_FULL;
                                                        
                            break;
                        }
                    }
                    
                    // now set the mode
                    UARTSetFifoMode(UART6, gUART6FifoMode);

                    // clear out the RX interrupt flag if set
                    INTClearFlag(INT_U6RX);
                    
                    // now re enable the interrupt
                    INTEnable(INT_U6RX, INT_ENABLED);
                    
                    break;
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)
            
            #if(USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    // first disable the interrupt and then clear the flag
                    CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
                    CANEnableModuleEvent(CAN1, CAN_RX_EVENT, FALSE);
                    
                    if(NumberOfBytesExpectedToBeRead <= CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES)
                    {
                        // do interrupt on rx fifo not empty
                        CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
                    }
                    else
                    {
                        if(NumberOfBytesExpectedToBeRead <= CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES * (CAN_MAXIMUM_PACKETS_PER_CHANNEL / 2))
                        {
                            // do interrupt on rx fifo half full
                            CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_HALF_FULL, TRUE);
                        }
                        else
                        {
                            if(NumberOfBytesExpectedToBeRead == CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES * CAN_MAXIMUM_PACKETS_PER_CHANNEL)
                            {
                                // do interrupt on rx fifo full
                                CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_FULL, TRUE);
                            }
                            else
                            {
                                // do interrupt on rx fifo half full since there is no other 3/4 or anything
                                CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_HALF_FULL, TRUE);
                            }
                        }
                    }
                    
                    // now clear out the flag
                    PortIOBufferClearCAN1RXInterruptFlag();
                    
                    // now re-enable the general RX interrupt
                    CANEnableModuleEvent(CAN1, CAN_RX_EVENT, TRUE);
                    
                    break;
                }
            #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

            #if(USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    // first disable the interrupt and then clear the flag
                    CANEnableChannelEvent(CAN2, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
                    CANEnableModuleEvent(CAN2, CAN_RX_EVENT, FALSE);
                    
                    if(NumberOfBytesExpectedToBeRead <= CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES)
                    {
                        // do interrupt on rx fifo not empty
                        CANEnableChannelEvent(CAN2, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
                    }
                    else
                    {
                        if(NumberOfBytesExpectedToBeRead <= CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES * (CAN_MAXIMUM_PACKETS_PER_CHANNEL / 2))
                        {
                            // do interrupt on rx fifo half full
                            CANEnableChannelEvent(CAN2, CAN_CHANNEL1, CAN_RX_CHANNEL_HALF_FULL, TRUE);
                        }
                        else
                        {
                            if(NumberOfBytesExpectedToBeRead == CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES * CAN_MAXIMUM_PACKETS_PER_CHANNEL)
                            {
                                // do interrupt on rx fifo full
                                CANEnableChannelEvent(CAN2, CAN_CHANNEL1, CAN_RX_CHANNEL_FULL, TRUE);
                            }
                            else
                            {
                                // do interrupt on rx fifo half full since there is no other 3/4 or anything
                                CANEnableChannelEvent(CAN2, CAN_CHANNEL1, CAN_RX_CHANNEL_HALF_FULL, TRUE);
                            }
                        }
                    }
                    
                    // now clear out the flag
                    PortIOBufferClearCAN2RXInterruptFlag();
                    
                    // now re-enable the general RX interrupt
                    CANEnableModuleEvent(CAN2, CAN_RX_EVENT, TRUE);
                    
                    break;
                }
            #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

            #if (USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    if(NumberOfBytesExpectedToBeRead == 0)
                    {
                        INTEnable(INT_AD1, INT_DISABLED);
                        
                        PortIOBufferClearADC1InterruptFlag();
                    }
                    else
                    {
                        PortIOBufferClearADC1InterruptFlag();
                        
                        INTEnable(INT_AD1, INT_ENABLED);
                    }
                    
                    break;
                }
            #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)

            default:
            {
                return;
            }
        }
    }

    BOOL PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_ID IOBufferID, UINT32 NumberOfBytesToTransmit)
    {
        switch(IOBufferID)
        {
            #if(USING_UART_1_IO_BUFFER == 1)
                case IO_BUFFER_UART_1_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        INTEnable(INT_U1TX, INT_DISABLED);

                        // clear out the transmit interrupt flag if set
                        INTClearFlag(INT_U1TX);
                    }
                    else
                    {
                        // clear out the current TX interrupt setting
                        gUART1FifoMode &= ~(UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_TX_BUFFER_EMPTY);
                        
                        // if data is already being tx'ed, we will trigger on tx done,
                        // otherwise to get things started we will trigger on not full
                        // this way is more efficient than doing UART_INTERRUPT_ON_TX_NOT_FULL all the time
                        if(UARTTransmissionHasCompleted(UART1) == TRUE)
                            gUART1FifoMode |= UART_INTERRUPT_ON_TX_NOT_FULL;
                        else
                            gUART1FifoMode |= UART_INTERRUPT_ON_TX_BUFFER_EMPTY;
                            
                        UARTSetFifoMode(UART1, gUART1FifoMode);
                        
                        // now re enable the interrupt
                        INTEnable(INT_U1TX, INT_ENABLED);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_UART_1_IO_BUFFER == 1)

            #if(USING_UART_2_IO_BUFFER == 1)
                case IO_BUFFER_UART_2_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        INTEnable(INT_U2TX, INT_DISABLED);

                        // clear out the transmit interrupt flag if set
                        INTClearFlag(INT_U2TX);
                    }
                    else
                    {
                        // clear out the current TX interrupt setting
                        gUART2FifoMode &= ~(UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_TX_BUFFER_EMPTY);
                        
                        // if data is already being tx'ed, we will trigger on tx done,
                        // otherwise to get things started we will trigger on not full
                        // this way is more efficient than doing UART_INTERRUPT_ON_TX_NOT_FULL all the time
                        if(UARTTransmissionHasCompleted(UART2) == TRUE)
                            gUART2FifoMode |= UART_INTERRUPT_ON_TX_NOT_FULL;
                        else
                            gUART2FifoMode |= UART_INTERRUPT_ON_TX_BUFFER_EMPTY;
                            
                        UARTSetFifoMode(UART2, gUART2FifoMode);
                        
                        // now re enable the interrupt
                        INTEnable(INT_U2TX, INT_ENABLED);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

            #if(USING_UART_3_IO_BUFFER == 1)
                case IO_BUFFER_UART_3_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        INTEnable(INT_U3TX, INT_DISABLED);

                        // clear out the transmit interrupt flag if set
                        INTClearFlag(INT_U3TX);
                    }
                    else
                    {
                        // clear out the current TX interrupt setting
                        gUART3FifoMode &= ~(UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_TX_BUFFER_EMPTY);
                        
                        // if data is already being tx'ed, we will trigger on tx done,
                        // otherwise to get things started we will trigger on not full
                        // this way is more efficient than doing UART_INTERRUPT_ON_TX_NOT_FULL all the time
                        if(UARTTransmissionHasCompleted(UART3) == TRUE)
                            gUART3FifoMode |= UART_INTERRUPT_ON_TX_NOT_FULL;
                        else
                            gUART3FifoMode |= UART_INTERRUPT_ON_TX_BUFFER_EMPTY;
                            
                        UARTSetFifoMode(UART3, gUART3FifoMode);
                        
                        // now re enable the interrupt
                        INTEnable(INT_U3TX, INT_ENABLED);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

            #if(USING_UART_4_IO_BUFFER == 1)
                case IO_BUFFER_UART_4_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        INTEnable(INT_U4TX, INT_DISABLED);

                        // clear out the transmit interrupt flag if set
                        INTClearFlag(INT_U4TX);
                    }
                    else
                    {
                        // clear out the current TX interrupt setting
                        gUART4FifoMode &= ~(UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_TX_BUFFER_EMPTY);
                        
                        // if data is already being tx'ed, we will trigger on tx done,
                        // otherwise to get things started we will trigger on not full
                        // this way is more efficient than doing UART_INTERRUPT_ON_TX_NOT_FULL all the time
                        if(UARTTransmissionHasCompleted(UART4) == TRUE)
                            gUART4FifoMode |= UART_INTERRUPT_ON_TX_NOT_FULL;
                        else
                            gUART4FifoMode |= UART_INTERRUPT_ON_TX_BUFFER_EMPTY;
                            
                        UARTSetFifoMode(UART4, gUART4FifoMode);
                        
                        // now re enable the interrupt
                        INTEnable(INT_U4TX, INT_ENABLED);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

            #if(USING_UART_5_IO_BUFFER == 1)
                case IO_BUFFER_UART_5_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        INTEnable(INT_U5TX, INT_DISABLED);

                        // clear out the transmit interrupt flag if set
                        INTClearFlag(INT_U5TX);
                    }
                    else
                    {
                        // clear out the current TX interrupt setting
                        gUART5FifoMode &= ~(UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_TX_BUFFER_EMPTY);
                        
                        // if data is already being tx'ed, we will trigger on tx done,
                        // otherwise to get things started we will trigger on not full
                        // this way is more efficient than doing UART_INTERRUPT_ON_TX_NOT_FULL all the time
                        if(UARTTransmissionHasCompleted(UART5) == TRUE)
                            gUART5FifoMode |= UART_INTERRUPT_ON_TX_NOT_FULL;
                        else
                            gUART5FifoMode |= UART_INTERRUPT_ON_TX_BUFFER_EMPTY;
                            
                        UARTSetFifoMode(UART5, gUART5FifoMode);
                        
                        // now re enable the interrupt
                        INTEnable(INT_U5TX, INT_ENABLED);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

            #if(USING_UART_6_IO_BUFFER == 1)
                case IO_BUFFER_UART_6_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        INTEnable(INT_U6TX, INT_DISABLED);

                        // clear out the transmit interrupt flag if set
                        INTClearFlag(INT_U6TX);
                    }
                    else
                    {
                        // clear out the current TX interrupt setting
                        gUART6FifoMode &= ~(UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_TX_BUFFER_EMPTY);
                        
                        // if data is already being tx'ed, we will trigger on tx done,
                        // otherwise to get things started we will trigger on not full
                        // this way is more efficient than doing UART_INTERRUPT_ON_TX_NOT_FULL all the time
                        if(UARTTransmissionHasCompleted(UART6) == TRUE)
                            gUART6FifoMode |= UART_INTERRUPT_ON_TX_NOT_FULL;
                        else
                            gUART6FifoMode |= UART_INTERRUPT_ON_TX_BUFFER_EMPTY;
                            
                        UARTSetFifoMode(UART6, gUART6FifoMode);
                        
                        // now re enable the interrupt
                        INTEnable(INT_U6TX, INT_ENABLED);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

            #if(USING_CAN_1_IO_BUFFER == 1)
                case IO_BUFFER_CAN_1_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        CANEnableModuleEvent(CAN1, CAN_TX_EVENT, FALSE);
                        
                        CANEnableChannelEvent(CAN1, CAN_CHANNEL0, CAN_TX_CHANNEL_EMPTY, FALSE);

                        // clear out the transmit interrupt flag if set
                        PortIOBufferClearCAN1TXInterruptFlag();
                    }
                    else
                    {
                        // clear out the transmit interrupt flag if set
                        PortIOBufferClearCAN1TXInterruptFlag();
                        
                        CANEnableChannelEvent(CAN1, CAN_CHANNEL0, CAN_TX_CHANNEL_EMPTY, TRUE);
                        
                        // now re enable the interrupt
                        CANEnableModuleEvent(CAN1, CAN_TX_EVENT, TRUE);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

            #if(USING_CAN_2_IO_BUFFER == 1)
                case IO_BUFFER_CAN_2_ID:
                {
                    if(NumberOfBytesToTransmit == 0)
                    {
                        // first disable the interrupt and then clear the flag
                        CANEnableModuleEvent(CAN2, CAN_TX_EVENT, FALSE);
                        
                        CANEnableChannelEvent(CAN2, CAN_CHANNEL0, CAN_TX_CHANNEL_EMPTY, FALSE);

                        // clear out the transmit interrupt flag if set
                        PortIOBufferClearCAN2TXInterruptFlag();
                    }
                    else
                    {
                        // clear out the transmit interrupt flag if set
                        PortIOBufferClearCAN2TXInterruptFlag();
                        
                        CANEnableChannelEvent(CAN2, CAN_CHANNEL0, CAN_TX_CHANNEL_EMPTY, TRUE);
                        
                        // now re enable the interrupt
                        CANEnableModuleEvent(CAN2, CAN_TX_EVENT, TRUE);
                    }
                    
                    break;
                }
            #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)

            #if (USING_ADC_1_IO_BUFFER == 1)
                case IO_BUFFER_ADC_1_ID:
                {
                    break;
                }
            #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)

            default:
            {
                return FALSE;
            }
        }
        
        return TRUE;
    }
#endif // end of #if (USING_UART_1_IO_BUFFER == 1)
