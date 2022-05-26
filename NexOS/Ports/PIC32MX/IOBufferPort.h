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

#ifndef IO_BUFFER_PORT_H
    #define IO_BUFFER_PORT_H

#include "Port.h"

#define IO_BUFFER_INTERRUPT_HANDLER_PRIORITY                                INT_PRIORITY_LEVEL_1

#define PortIOBufferGetUART1RXDataInterruptFlag()                           INTGetFlag(INT_U1RX)
#define PortIOBufferGetUART1TXDoneInterruptFlag()                           INTGetFlag(INT_U1TX)
#define PortIOBufferGetUART1ErrorInterruptFlag()                            INTGetFlag(INT_U1E)
#define PortIOBufferClearUART1RXInterruptFlag()                             INTClearFlag(INT_U1RX)
#define PortIOBufferClearUART1TXInterruptFlag()                             INTClearFlag(INT_U1TX)
#define PortIOBufferClearUART1ErrorInterruptFlag()                          INTClearFlag(INT_U1E)
#define PortIOBufferClearUART1InterruptFlag()                               ;
    
#define PortIOBufferGetUART2RXDataInterruptFlag()                           INTGetFlag(INT_U2RX)
#define PortIOBufferGetUART2TXDoneInterruptFlag()                           INTGetFlag(INT_U2TX)
#define PortIOBufferGetUART2ErrorInterruptFlag()                            INTGetFlag(INT_U2E)
#define PortIOBufferClearUART2RXInterruptFlag()                             INTClearFlag(INT_U2RX)
#define PortIOBufferClearUART2TXInterruptFlag()                             INTClearFlag(INT_U2TX)
#define PortIOBufferClearUART2ErrorInterruptFlag()                          INTClearFlag(INT_U2E)
#define PortIOBufferClearUART2InterruptFlag()                               ;

#define PortIOBufferGetUART3RXDataInterruptFlag()                           INTGetFlag(INT_U3RX)
#define PortIOBufferGetUART3TXDoneInterruptFlag()                           INTGetFlag(INT_U3TX)
#define PortIOBufferGetUART3ErrorInterruptFlag()                            INTGetFlag(INT_U3E)
#define PortIOBufferClearUART3RXInterruptFlag()                             INTClearFlag(INT_U3RX)
#define PortIOBufferClearUART3TXInterruptFlag()                             INTClearFlag(INT_U3TX)
#define PortIOBufferClearUART3ErrorInterruptFlag()                          INTClearFlag(INT_U3E)
#define PortIOBufferClearUART3InterruptFlag()                               ;

#define PortIOBufferGetUART4RXDataInterruptFlag()                           INTGetFlag(INT_U4RX)
#define PortIOBufferGetUART4TXDoneInterruptFlag()                           INTGetFlag(INT_U4TX)
#define PortIOBufferGetUART4ErrorInterruptFlag()                            INTGetFlag(INT_U4E)
#define PortIOBufferClearUART4RXInterruptFlag()                             INTClearFlag(INT_U4RX)
#define PortIOBufferClearUART4TXInterruptFlag()                             INTClearFlag(INT_U4TX)
#define PortIOBufferClearUART4ErrorInterruptFlag()                          INTClearFlag(INT_U4E)
#define PortIOBufferClearUART4InterruptFlag()                               ;

#define PortIOBufferGetUART5RXDataInterruptFlag()                           INTGetFlag(INT_U5RX)
#define PortIOBufferGetUART5TXDoneInterruptFlag()                           INTGetFlag(INT_U5TX)
#define PortIOBufferGetUART5ErrorInterruptFlag()                            INTGetFlag(INT_U5E)
#define PortIOBufferClearUART5RXInterruptFlag()                             INTClearFlag(INT_U5RX)
#define PortIOBufferClearUART5TXInterruptFlag()                             INTClearFlag(INT_U5TX)
#define PortIOBufferClearUART5ErrorInterruptFlag()                          INTClearFlag(INT_U5E)
#define PortIOBufferClearUART5InterruptFlag()                               ;

#define PortIOBufferGetUART6RXDataInterruptFlag()                           INTGetFlag(INT_U6RX)
#define PortIOBufferGetUART6TXDoneInterruptFlag()                           INTGetFlag(INT_U6TX)
#define PortIOBufferGetUART6ErrorInterruptFlag()                            INTGetFlag(INT_U6E)
#define PortIOBufferClearUART6RXInterruptFlag()                             INTClearFlag(INT_U6RX)
#define PortIOBufferClearUART6TXInterruptFlag()                             INTClearFlag(INT_U6TX)
#define PortIOBufferClearUART6ErrorInterruptFlag()                          INTClearFlag(INT_U6E)
#define PortIOBufferClearUART6InterruptFlag()                               ;

#define PortIOBufferClearCAN1InterruptFlag()                                mCAN1ClearIntFlag()
#define PortIOBufferGetCAN1ErrorInterruptFlag()                             C1INT & (UINT32)0x0000B800
#define PortIOBufferGetCAN1RXInterruptFlag()                                C1INT & (UINT32)0x00000002
#define PortIOBufferGetCAN1TXDoneInterruptFlag()                            C1INT & (UINT32)0x00000001
#define PortIOBufferClearCAN1ErrorInterruptFlag()                           C1INTCLR = (UINT32)0x0000B800
#define PortIOBufferClearCAN1RXInterruptFlag()                              C1INTCLR = (UINT32)0x00000002
#define PortIOBufferClearCAN1TXInterruptFlag()                              C1INTCLR = (UINT32)0x00000001

#define PortIOBufferClearCAN2InterruptFlag()                                mCAN2ClearIntFlag()
#define PortIOBufferGetCAN2ErrorInterruptFlag()                             C2INT & (UINT32)0x0000B800
#define PortIOBufferGetCAN2RXInterruptFlag()                                C2INT & (UINT32)0x00000002
#define PortIOBufferGetCAN2TXDoneInterruptFlag()                            C2INT & (UINT32)0x00000001
#define PortIOBufferClearCAN2ErrorInterruptFlag()                           C2INTCLR = (UINT32)0x0000B800
#define PortIOBufferClearCAN2RXInterruptFlag()                              C2INTCLR = (UINT32)0x00000002
#define PortIOBufferClearCAN2TXInterruptFlag()                              C2INTCLR = (UINT32)0x00000001

#define PortIOBufferClearADC1InterruptFlag()                                mAD1ClearIntFlag()
#define PortIOBufferGetADC1InterruptFlag()                                  mAD1GetIntFlag()

// These are how big the hardware data buffers are expected to
// be on the silicon of the microcontroller.
#define UART_1_ISR_DATA_BUFFER_SIZE_IN_BYTES                                8
#define UART_2_ISR_DATA_BUFFER_SIZE_IN_BYTES                                8
#define UART_3_ISR_DATA_BUFFER_SIZE_IN_BYTES                                8
#define UART_4_ISR_DATA_BUFFER_SIZE_IN_BYTES                                8
#define UART_5_ISR_DATA_BUFFER_SIZE_IN_BYTES                                8
#define UART_6_ISR_DATA_BUFFER_SIZE_IN_BYTES                                8
#define CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES                                 sizeof(CANRxMessageBuffer)
#define CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES                                 sizeof(CANRxMessageBuffer)
#define ADC_1_ISR_DATA_BUFFER_SIZE_IN_BYTES                                 16
#define CAN_MAXIMUM_PACKETS_PER_CHANNEL                                     32

BOOL PortIOBufferInitializeInterrupt(IO_BUFFER_ID IOBufferID);

BOOL PortIOBufferDataAvailable(IO_BUFFER_ID IOBufferID);

UINT32 PortIOBufferReadAvailableData(IO_BUFFER_ID IOBufferID, BYTE *DataBuffer, UINT32 DataBufferSize);

UINT32 PortIOBufferGetErrorStatus(IO_BUFFER_ID IOBufferID);

UINT32 PortIOBufferGetTXSpaceAvailable(IO_BUFFER_ID IOBufferID);

BOOL PortIOBufferWriteData(IO_BUFFER_ID IOBufferID, BYTE *Data, UINT32 NumberOfBytesToWrite);

void PortIOBufferSetRXInterruptTriggerLevel(IO_BUFFER_ID IOBufferID, UINT32 NumberOfBytesExpectedToBeRead);

BOOL PortIOBufferSetTXInterruptTriggerLevel(IO_BUFFER_ID IOBufferID, UINT32 NumberOfBytesToTransmit);

#endif // end of #ifndef IO_BUFFER_PORT_H