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

#include <string.h>

#include "RTOSConfig.h"
#include "IOBuffer.h"
#include "CriticalSection.h"
#include "IOBufferPort.h"

#if (USING_IO_BUFFER_OVERFLOW_EVENT == 1)
    #include "../Event/Event.h"
#endif // end of #if (USING_IO_BUFFER_OVERFLOW_EVENT == 1)

#if (USING_IO_BUFFER_OVERFLOW_CALLBACK == 1)
    void IOBufferOverflowCallback(IO_BUFFER_ID IOBufferID);
#endif // end of #if (USING_IO_BUFFER_OVERFLOW_CALLBACK == 1)

extern TASK *gCurrentTask;
extern DOUBLE_LINKED_LIST_HEAD gMiscellaneousBlockedQueueHead;

BOOL UpdateUART1RXBuffer(BYTE *Data, UINT32 DataBufferSize);
BOOL UpdateUART2RXBuffer(BYTE *Data, UINT32 DataBufferSize);
BOOL UpdateUART3RXBuffer(BYTE *Data, UINT32 DataBufferSize);
BOOL UpdateUART4RXBuffer(BYTE *Data, UINT32 DataBufferSize);
BOOL UpdateUART5RXBuffer(BYTE *Data, UINT32 DataBufferSize);
BOOL UpdateUART6RXBuffer(BYTE *Data, UINT32 DataBufferSize);
BOOL UpdateCAN1RXBuffer(BYTE *Data, UINT32 DataBufferSize);
BOOL UpdateCAN2RXBuffer(BYTE *Data, UINT32 DataBufferSize);

const BYTE gDefaultNewLineSequence[] = "\r\n";

#if(USING_UART_1_IO_BUFFER == 1)
    IO_BUFFER gUART1IOBuffer;
#endif // end of #if(USING_UART_1_IO_BUFFER == 1)
    
#if(USING_UART_2_IO_BUFFER == 1)
    IO_BUFFER gUART2IOBuffer;
#endif // end of #if(USING_UART_2_IO_BUFFER == 1)
    
#if(USING_UART_3_IO_BUFFER == 1)
    IO_BUFFER gUART3IOBuffer;
#endif // end of #if(USING_UART_3_IO_BUFFER == 1)
    
#if(USING_UART_4_IO_BUFFER == 1)
    IO_BUFFER gUART4IOBuffer;
#endif // end of #if(USING_UART_4_IO_BUFFER == 1)
    
#if(USING_UART_5_IO_BUFFER == 1)
    IO_BUFFER gUART5IOBuffer;
#endif // end of #if(USING_UART_5_IO_BUFFER == 1)
    
#if(USING_UART_6_IO_BUFFER == 1)
    IO_BUFFER gUART6IOBuffer;
#endif // end of #if(USING_UART_6_IO_BUFFER == 1)
    
#if(USING_CAN_1_IO_BUFFER == 1)
    IO_BUFFER gCAN1IOBuffer;
#endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

#if(USING_CAN_2_IO_BUFFER == 1)
    IO_BUFFER gCAN2IOBuffer;
#endif // end of #if(USING_CAN_2_IO_BUFFER == 1)
    
#if(USING_ADC_1_IO_BUFFER == 1)
    IO_BUFFER gADC1IOBuffer;
#endif // end of #if(USING_ADC_1_IO_BUFFER == 1)
            
static void OS_PutIOBufferInOpenMode(IO_BUFFER *IOBuffer, BOOL ResetTXBuffer)
{
    // reset the counters and buffer pointer
    IOBuffer->TaskBuffer = (BYTE*)NULL;

    // set the state to open
    if(ResetTXBuffer == TRUE)
    {
        IOBuffer->TXState = IO_BUFFER_OPEN;
    }
    else
    {
        IOBuffer->RXState = IO_BUFFER_OPEN;
        
        #if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
            // clear the read until byte
            IOBuffer->ByteToReadUntil = 0;
        #endif // end of #if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
    }
}

#if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
    static BOOL CopyRemainingDataToIOBuffer(IO_BUFFER *IOBuffer, BYTE *Data, UINT32 DataLength, BOOL InitializeBufferAndReadyTask)
    {
        // copy the leftovers into the IO Buffer
        if(DataLength != 0)
            GenericBufferWrite(&IOBuffer->RXGenericBuffer, DataLength, Data);

        if(InitializeBufferAndReadyTask == TRUE)
        {
            // reset the IO Buffer back to OPEN mode
            OS_PutIOBufferInOpenMode(IOBuffer, FALSE);

            // now add the TASK to the ready queue
            return OS_RemoveTaskFromIOBufferBlockedList(IOBuffer);
        }
        else
        {
            return FALSE;
        }
    }
#endif // end of #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)

BOOL OSUpdateIOBuffer(IO_BUFFER_ID BufferID)
{
    switch(BufferID)
    {
        #if (USING_UART_1_IO_BUFFER == 1)
            case IO_BUFFER_UART_1_ID:
            {
                BYTE Data[UART_1_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateUART1RXBuffer(Data, UART_1_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_UART_1_IO_BUFFER == 1)

        #if (USING_UART_2_IO_BUFFER == 1)
            case IO_BUFFER_UART_2_ID:
            {
                BYTE Data[UART_2_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateUART2RXBuffer(Data, UART_2_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_UART_2_IO_BUFFER == 1)

        #if (USING_UART_3_IO_BUFFER == 1)
            case IO_BUFFER_UART_3_ID:
            {
                BYTE Data[UART_3_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateUART3RXBuffer(Data, UART_3_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_UART_3_IO_BUFFER == 1)

            
        #if (USING_UART_4_IO_BUFFER == 1)
            case IO_BUFFER_UART_4_ID:
            {
                BYTE Data[UART_4_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateUART4RXBuffer(Data, UART_4_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_UART_4_IO_BUFFER == 1)

        #if (USING_UART_5_IO_BUFFER == 1)
            case IO_BUFFER_UART_5_ID:
            {
                BYTE Data[UART_5_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateUART5RXBuffer(Data, UART_5_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_UART_5_IO_BUFFER == 1)

        #if (USING_UART_6_IO_BUFFER == 1)
            case IO_BUFFER_UART_6_ID:
            {
                BYTE Data[UART_6_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateUART6RXBuffer(Data, UART_6_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_UART_6_IO_BUFFER == 1)

        #if (USING_CAN_1_IO_BUFFER == 1)
            case IO_BUFFER_CAN_1_ID:
            {
                BYTE Data[CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateCAN1RXBuffer(Data, CAN_1_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_CAN_1_IO_BUFFER == 1)

        #if (USING_CAN_2_IO_BUFFER == 1)
            case IO_BUFFER_CAN_2_ID:
            {
                BYTE Data[CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES];
                
                return UpdateCAN2RXBuffer(Data, CAN_2_ISR_DATA_BUFFER_SIZE_IN_BYTES);
            }
        #endif // end of #if (USING_CAN_2_IO_BUFFER == 1)    
        
        #if (USING_ADC_1_IO_BUFFER == 1)
            case IO_BUFFER_ADC_1_ID:
            {
                return FALSE;
            }
        #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)    

        default:
        {
            return FALSE;
        }
    }
}

BOOL OS_RemoveTaskFromIOBufferBlockedList(IO_BUFFER *IOBuffer)
{
    TASK *TaskHandle;
    UINT32 i;
    
    // get a handle on the TASK which owns the IO Buffer
    TaskHandle = OS_GetMutexOwner(&IOBuffer->IOBufferMutex);

    // remove all the nodes from any lists they were on
    for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
        OS_RemoveTaskFromList(&TaskHandle->TaskNodeArray[i]);

    // now add the task to the ready queue
    return OS_AddTaskToReadyQueue(TaskHandle);
}

/*
 * This is data that will be going from the IO_BUFFER to the outside world.
 */
#if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
    UINT32 OS_ReadFromIOBuffer(IO_BUFFER *IOBuffer, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, UINT32 DataLength)
    {
        UINT32 DataRead;

        // based on the current state, execute accordingly
        switch(IOBuffer->TXState)
        {
            case IO_BUFFER_WRITE:
            {
                UINT32 i;

                // get the length of how long we will write
                if(DataLength < IOBuffer->BytesLeftToExchange)
                    DataRead = DataLength;
                else
                    DataRead = IOBuffer->BytesLeftToExchange;

                // copy over the data from the user buffer to the temp buffer.
                // This also advances the user buffer each byte
                for(i = 0; i < DataRead; i++)
                {
                    DestinationBuffer[i] = IOBuffer->TaskBuffer[0];

                    IOBuffer->TaskBuffer++;
                }

                // update the number of bytes to send
                IOBuffer->BytesLeftToExchange -= DataRead;

                break;
            }

            #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                case IO_BUFFER_ASYNC_WRITE:
                {
                    // we will either get DataLength bytes, or less
                    DataRead = GenericBufferRead(IOBuffer->TXGenericBuffer, DataLength, DestinationBuffer, DestinationBufferSize, FALSE);

                    break;
                }
            #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

            case IO_BUFFER_OPEN:
            case IO_BUFFER_READ_NUMBER_OF_BYTES:
            case IO_BUFFER_READ_UNTIL_BYTE:
            case IO_BUFFER_CLOSED:
            case IO_BUFFER_OVERFLOW:
            case IO_BUFFER_HARDWARE_ERROR:
            default:
            {
                DataRead = 0;

                break;
            }
        }

        return DataRead;
    }
#endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

/*
 * This is data that came in from the outside world, and we are writing it
 * to the IO_BUFFER so that the user can read it later.
 */
BOOL OS_WriteToIOBuffer(IO_BUFFER_ID IOBufferID, BYTE *Data, UINT32 DataLength)
{
    IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
    
    // based on the current state, execute accordingly
    switch(IOBuffer->RXState)
    {
        // let these fall through to IO_BUFFER_OPEN
        #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
            case IO_BUFFER_WRITE:
                
            #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                case IO_BUFFER_ASYNC_WRITE:
            #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
            
        case IO_BUFFER_OPEN:
        {
            #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
                // just write the data to the buffer
                if(GenericBufferWrite(&IOBuffer->RXGenericBuffer, DataLength, Data) < DataLength)
                {
                    // there wasn't enough space to store the data, go into overflow mode
                    IOBuffer->RXState = IO_BUFFER_OVERFLOW;
  
                    #if (USING_IO_BUFFER_OVERFLOW_CALLBACK == 1)
                        IOBufferOverflowCallback(IOBufferID);
                    #endif // end of #if (USING_IO_BUFFER_OVERFLOW_CALLBACK == 1)

                    #if (USING_IO_BUFFER_OVERFLOW_EVENT == 1)
                        if(OS_RaiseEvent(IO_BUFFER_OVERFLOW_EVENT) == TRUE)
                            return TRUE;
                    #endif // end of #if(USING_IO_BUFFER_OVERFLOW_EVENT == 1)
                }
            #endif // end of #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
            
            return FALSE;
        }
        
        #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1)
            case IO_BUFFER_READ_NUMBER_OF_BYTES:
            {
                while(DataLength != 0)
                {
                    // are we at the user buffer size?
                    if(IOBuffer->BytesLeftToExchange == 0)
                    {
                        // the user buffer has no more space
                        break;
                    }
                    
                    // copy it over to the user buffer
                    *IOBuffer->TaskBuffer++ = *Data++;
                    
                    // we wrote a byte, so decrease the size
                    DataLength--;
                    IOBuffer->BytesLeftToExchange--;
                }

                return CopyRemainingDataToIOBuffer(IOBuffer, &Data[IOBuffer->BytesLeftToExchange], DataLength, (BOOL)(IOBuffer->BytesLeftToExchange == 0));
            }
        #endif // end of #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1)
        
        #if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
            case IO_BUFFER_READ_UNTIL_BYTE:
            {
                BOOL ReadyTask = FALSE;
                
                while(DataLength != 0)
                {
                    // are we at the user buffer size?
                    if(IOBuffer->BytesLeftToExchange == IOBuffer->UserBufferSizeInBytes)
                    {
                        ReadyTask = TRUE;
                        
                        // the user buffer has no more space
                        break;
                    }
                    
                    // copy it over to the user buffer
                    IOBuffer->TaskBuffer[IOBuffer->BytesLeftToExchange++] = *Data;
                    
                    // we wrote a byte, so decrease the size
                    DataLength--;
                    
                    // did we read in the final byte?
                    if(*Data++ == IOBuffer->ByteToReadUntil)
                    {
                        ReadyTask = TRUE;
                        
                        break;
                    }
                }
                
                return CopyRemainingDataToIOBuffer(IOBuffer, Data, DataLength, ReadyTask);
            }
        #endif // end of #if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
        
        #if (USING_IO_BUFFER_READ_LINE_METHOD == 1)
            case IO_BUFFER_READ_UNTIL_SEQUENCE:
            {
                BOOL ReadyTask = FALSE;
                
                while(DataLength != 0)
                {
                    // are we at the user buffer size?
                    if(IOBuffer->BytesLeftToExchange == IOBuffer->UserBufferSizeInBytes)
                    {
                        ReadyTask = TRUE;
                        
                        // the user buffer has no more space
                        break;
                    }
                    
                    // copy it over to the user buffer
                    *IOBuffer->TaskBuffer++ = *Data++;
                    IOBuffer->BytesLeftToExchange++;
                    
                    // we wrote a byte, so decrease the size
                    DataLength--;
                    
                    // did we read in the final byte for the sequence?
                    if(IOBuffer->NewLineSequenceSizeInBytes <= IOBuffer->BytesLeftToExchange)
                    {
                        // did they get the new line sequence at the end?
                        if(memcmp((const void*)&IOBuffer->TaskBuffer[(INT32)0 - (INT32)IOBuffer->NewLineSequenceSizeInBytes], (const void*)IOBuffer->NewLineSequence, IOBuffer->NewLineSequenceSizeInBytes) == 0)
                        {
                            ReadyTask = TRUE;

                            break;
                        }
                    }
                }
                
                return CopyRemainingDataToIOBuffer(IOBuffer, Data, DataLength, ReadyTask);
            }
        #endif // end of #if (USING_IO_BUFFER_READ_LINE_METHOD == 1)
        
        case IO_BUFFER_CLOSED:
        case IO_BUFFER_OVERFLOW:
        case IO_BUFFER_HARDWARE_ERROR:
        default:
        {
            return FALSE;
        }
    }
}

#if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
    UINT32 OS_IOBufferGetNumberOfBytesLeftToTransmit(IO_BUFFER *IOBuffer)
    {
        // is the IO Buffer in write mode?
        switch(IOBuffer->TXState)
        {
            case IO_BUFFER_WRITE:
            {
                return IOBuffer->BytesLeftToExchange;
            }

            #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                case IO_BUFFER_ASYNC_WRITE:
                {
                    return GenericBufferGetSize(IOBuffer->TXGenericBuffer);
                }
            #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

            case IO_BUFFER_OPEN:  
            case IO_BUFFER_READ_NUMBER_OF_BYTES:
            case IO_BUFFER_READ_UNTIL_BYTE:
            case IO_BUFFER_CLOSED:
            case IO_BUFFER_OVERFLOW:
            case IO_BUFFER_HARDWARE_ERROR:
            default:
            {
                return 0;
            }
        }
    }
#endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

IO_BUFFER *OS_GetIOBufferHandle(IO_BUFFER_ID IOBufferID)
{
    switch(IOBufferID)
    {
        #if(USING_UART_1_IO_BUFFER == 1)
            case IO_BUFFER_UART_1_ID:
            {
                return &gUART1IOBuffer;
            }
        #endif // end of #if(USING_UART_1_IO_BUFFER == 1)
        
        #if(USING_UART_2_IO_BUFFER == 1)
            case IO_BUFFER_UART_2_ID:
            {
                return &gUART2IOBuffer;
            }
        #endif // end of #if(USING_UART_2_IO_BUFFER == 1)

        #if(USING_UART_3_IO_BUFFER == 1)
            case IO_BUFFER_UART_3_ID:
            {
                return &gUART3IOBuffer;
            }
        #endif // end of #if(USING_UART_3_IO_BUFFER == 1)

        #if(USING_UART_4_IO_BUFFER == 1)
            case IO_BUFFER_UART_4_ID:
            {
                return &gUART4IOBuffer;
            }
        #endif // end of #if(USING_UART_4_IO_BUFFER == 1)

        #if(USING_UART_5_IO_BUFFER == 1)
            case IO_BUFFER_UART_5_ID:
            {
                return &gUART5IOBuffer;
            }
        #endif // end of #if(USING_UART_5_IO_BUFFER == 1)

        #if(USING_UART_6_IO_BUFFER == 1)
            case IO_BUFFER_UART_6_ID:
            {
                return &gUART6IOBuffer;
            }
        #endif // end of #if(USING_UART_6_IO_BUFFER == 1)

        #if(USING_CAN_1_IO_BUFFER == 1)
            case IO_BUFFER_CAN_1_ID:
            {
                return &gCAN1IOBuffer;
            }
        #endif // end of #if(USING_CAN_1_IO_BUFFER == 1)

        #if(USING_CAN_2_IO_BUFFER == 1)
            case IO_BUFFER_CAN_2_ID:
            {
                return &gCAN2IOBuffer;
            }
        #endif // end of #if(USING_CAN_2_IO_BUFFER == 1)
        
        #if(USING_ADC_1_IO_BUFFER == 1)
            case IO_BUFFER_ADC_1_ID:
            {
                return &gADC1IOBuffer;
            }
        #endif // end of #if(USING_ADC_1_IO_BUFFER == 1)
    
        default:
        {
            return (IO_BUFFER*)NULL;
        }
    }
}

OS_RESULT InitIOBuffer(IO_BUFFER_ID IOBufferID, BYTE *RXBuffer, UINT32 RXBufferSizeInBytes

                       #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                           , BYTE *TXBuffer
                           , UINT32 TXBufferSizeInBytes
                       #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                       )
{
    IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
    
    #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
        if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
            return OS_INVALID_ARGUMENT;

        if(RXBuffer != (BYTE*)NULL)
            if (RAMAddressValid((OS_WORD)RXBuffer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

        if(RXBufferSizeInBytes == 0)
            return OS_INVALID_ARGUMENT;

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
            if(TXBuffer != (BYTE*)NULL)
                if (RAMAddressValid((OS_WORD)TXBuffer) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

            if(TXBuffer != (BYTE*)NULL && TXBufferSizeInBytes == 0)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
    #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
    
    // we're good to make the buffer now
    memset((void*)IOBuffer, 0, sizeof(IO_BUFFER));
    
    #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
        // initialize the GENERIC_BUFFER for the RX
        if(CreateGenericBuffer(&IOBuffer->RXGenericBuffer, RXBufferSizeInBytes, RXBuffer) == (GENERIC_BUFFER*)NULL)
            return OS_ALLOCATE_MEMORY_FAILED;
    #endif // end of #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
    
    #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
            // initialize the GENERIC_BUFFER for the TX
            if(TXBufferSizeInBytes != 0)
            {
                if(CreateGenericBuffer(IOBuffer->TXGenericBuffer, TXBufferSizeInBytes, TXBuffer) == (GENERIC_BUFFER*)NULL)
                    return OS_ALLOCATE_MEMORY_FAILED;
            }
            else
            {
                IOBuffer->TXGenericBuffer = (GENERIC_BUFFER*)NULL;
            }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
    #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
    
    // initialize the MUTEX
    if(CreateMutex(&IOBuffer->IOBufferMutex, FALSE) == (MUTEX*)NULL)
        return OS_ALLOCATE_MEMORY_FAILED;        

    // now we can say that it is open
    OS_PutIOBufferInOpenMode(IOBuffer, TRUE);
    OS_PutIOBufferInOpenMode(IOBuffer, FALSE);
    
    // set the size of the sequence and the sequence
    #if (USING_IO_BUFFER_WRITE_LINE_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
        IOBuffer->NewLineSequenceSizeInBytes = sizeof(gDefaultNewLineSequence) - 1;
        IOBuffer->NewLineSequence = (BYTE*)gDefaultNewLineSequence;
    #endif // end of #if (USING_IO_BUFFER_WRITE_LINE_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
    
    // set the RX trigger level
    PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL);
    
    // set the TX trigger level
    PortIOBufferSetTXInterruptTriggerLevel(IOBufferID, 0);
    
    // now enable the interrupts for the IO Buffer
    PortIOBufferInitializeInterrupt(IOBufferID);
    
    return OS_SUCCESS;
}

OS_RESULT GetIOBuffer(  IO_BUFFER_ID IOBufferID
        
                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
						, INT32 TimeoutInTicks
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    
                        )
{
    IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
    
    #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
        if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
            return OS_INVALID_ARGUMENT;
    #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
    
    return GetMutex(&IOBuffer->IOBufferMutex
            
                    #if (USING_TASK_DELAY_TICKS_METHOD == 1)
						, TimeoutInTicks
                    #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    
                    );
}

#if (USING_IO_BUFFER_READ_BYTES_METHOD == 1)
    OS_RESULT IOBufferReadBytes(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 NumberOfBytesToRead, UINT32 *BytesRead

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                )
    {
        UINT32 TempBytesRead;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
        OS_RESULT Result = OS_SUCCESS;

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if(BytesRead != (UINT32*)NULL)
            {
                if (RAMAddressValid((OS_WORD)BytesRead) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                *BytesRead = 0;
            }

            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;

            if (RAMAddressValid((OS_WORD)UserBuffer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(UserBufferSizeInBytes == 0 || NumberOfBytesToRead == 0 || NumberOfBytesToRead > UserBufferSizeInBytes)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        // are we the owner?
        if(GetMutexOwner(&IOBuffer->IOBufferMutex) != gCurrentTask)
            return OS_RESOURCE_INVALID_OWNER;

        EnterCritical();

        // first read in any data from the hardware buffer
        OSUpdateIOBuffer(IOBufferID);

        // read out the data from the buffer
        TempBytesRead = GenericBufferRead(&IOBuffer->RXGenericBuffer, NumberOfBytesToRead, UserBuffer, UserBufferSizeInBytes, FALSE);

        // was there enough?
        if(TempBytesRead != NumberOfBytesToRead)
        {
            // there wasn't enough data in the buffer

            // if they had a delay of 0, we need to leave
            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                if (TimeoutInTicks == 0)
                {
                    Result = OS_RESOURCE_INSUFFICIENT_DATA;
                }
                else
            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                {
                    // they need to block on the data (wait for it to arrive)
                    IOBuffer->RXState = IO_BUFFER_READ_NUMBER_OF_BYTES;

                    // this is how many bytes need to be read
                    IOBuffer->BytesLeftToExchange = NumberOfBytesToRead - TempBytesRead;

                    // now point to where the next byte will go
                    IOBuffer->TaskBuffer = UserBuffer + TempBytesRead;

                    // remove the TASK from the ready queue and put it on the blocked list
                    OS_PlaceTaskOnBlockedList(gCurrentTask, &gMiscellaneousBlockedQueueHead, &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], BLOCKED, TRUE);

                    #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                        if (TimeoutInTicks > 0)
                        {
                            // place on timer list, this isn't a hard time operation but it is better than nothing
                            OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], TimeoutInTicks, FALSE);
                        }
                    #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                    // set the trigger level
                    PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, IOBuffer->BytesLeftToExchange);

                    // now block the task
                    SurrenderCPU();

                    // when we wake up, did we get all the data?
                    if(IOBuffer->BytesLeftToExchange == 0)
                    {
                        // this was a success

                        // we read all the bytes
                        TempBytesRead = NumberOfBytesToRead;
                    }
                    else
                    {
                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            // we did not get all the data
                            if (gCurrentTask->DelayInTicks == TASK_TIMEOUT_DONE_VALUE)
                                Result = OS_RESOURCE_INSUFFICIENT_SPACE;
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                        // this is how many bytes we read
                        TempBytesRead = NumberOfBytesToRead - IOBuffer->BytesLeftToExchange;
                    }
                    
                    #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                        // put the IO Buffer back into OPEN mode
                        OS_PutIOBufferInOpenMode(IOBuffer, FALSE);

                        gCurrentTask->DelayInTicks = 0;
                    #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                }
        }

        // if they passed in a pointer, set the value
        if(BytesRead != (UINT32*)NULL)
            *BytesRead = TempBytesRead;

        ExitCritical();

        return Result;
    }
#endif // end of #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1)

#if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
    OS_RESULT IOBufferReadUntil(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, BYTE ReadUntil, UINT32 *BytesRead

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                )
    {
        UINT32 BytePosition, TempBytesRead;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
        OS_RESULT Result = OS_SUCCESS;

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if(BytesRead != (UINT32*)NULL)
            {
                if (RAMAddressValid((OS_WORD)BytesRead) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                *BytesRead = 0;
            }

            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;

            if (RAMAddressValid((OS_WORD)UserBuffer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(UserBufferSizeInBytes == 0)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        // are we the owner?
        if(GetMutexOwner(&IOBuffer->IOBufferMutex) != gCurrentTask)
            return OS_RESOURCE_INVALID_OWNER;

        EnterCritical();

        // first read in any data from the hardware buffer
        OSUpdateIOBuffer(IOBufferID);
        
        // see if the terminating byte exists
        BytePosition = GenericBufferSeek(&IOBuffer->RXGenericBuffer, ReadUntil);
        
        // if the data didn't exist, get how many bytes exist in the buffer
        if(BytePosition == 0)
            BytePosition = GenericBufferGetSize(&IOBuffer->RXGenericBuffer);

        // read in as much valid data as possible with the number of bytes now BytePosition
        TempBytesRead = GenericBufferRead(&IOBuffer->RXGenericBuffer, BytePosition, UserBuffer, UserBufferSizeInBytes, FALSE);
        
        // if they passed in a pointer, set the value
        if(BytesRead != (UINT32*)NULL)
            *BytesRead = TempBytesRead;
        
        // did we max out the user buffer size?
        if(BytePosition > UserBufferSizeInBytes)
        {
            // this is an error
            Result = OS_RESOURCE_INSUFFICIENT_SPACE;
        }
        else
        {
            // did they get the data?
            if(UserBuffer[TempBytesRead - 1] == ReadUntil)
            {
                // we got the data
                Result = OS_SUCCESS;
            }
            else
            {
                // there was more user space left, put the buffer into read until mode

                // if they had a delay of 0, we need to leave
                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    if (TimeoutInTicks == 0)
                    {
                        Result = OS_RESOURCE_INSUFFICIENT_DATA;
                    }
                    else
                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    {
                        // they need to block on the data (wait for it to arrive)
                        IOBuffer->RXState = IO_BUFFER_READ_UNTIL_BYTE;

                        // set the read until byte so that we know when we reached the end
                        IOBuffer->ByteToReadUntil = ReadUntil;

                        // this is how many bytes need to be read
                        IOBuffer->BytesLeftToExchange = TempBytesRead;

                        // now point to where the next byte will go
                        IOBuffer->TaskBuffer = UserBuffer;

                        // remove the TASK from the ready queue and put it on the blocked list
                        OS_PlaceTaskOnBlockedList(gCurrentTask, &gMiscellaneousBlockedQueueHead, &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], BLOCKED, TRUE);

                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            if (TimeoutInTicks > 0)
                            {
                                // place on timer list, this isn't a hard time operation but it is better than nothing
                                OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], TimeoutInTicks, FALSE);
                            }
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                        // set the RX trigger level
                        PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL);

                        // now block the task
                        SurrenderCPU();

                        // when we wake up, did we get all the data?
                        // the last byte written to the user buffer should have the ReadUntil
                        if(UserBuffer[IOBuffer->BytesLeftToExchange - 1] == ReadUntil)
                        {
                            // we got all the data
                            Result = OS_SUCCESS;
                        }
                        else
                        {
                            // they did not get the ReadUntil character
                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                if (gCurrentTask->DelayInTicks == TASK_TIMEOUT_DONE_VALUE)
                                    Result = OS_TASK_TIMEOUT;
                                else
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                    Result = OS_RESOURCE_INSUFFICIENT_SPACE;
                        }

                        // if they passed in a pointer, set the value
                        if(BytesRead != (UINT32*)NULL)
                            *BytesRead = IOBuffer->BytesLeftToExchange;

                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            // put the IO Buffer back into OPEN mode
                            OS_PutIOBufferInOpenMode(IOBuffer, FALSE);

                            gCurrentTask->DelayInTicks = 0;
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    }
            }
        }

        ExitCritical();

        return Result;
    }
#endif // end of #if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)

#if (USING_IO_BUFFER_READ_LINE_METHOD == 1)
    static OS_RESULT OS_IOBufferReadLinePresent(IO_BUFFER *IOBuffer, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesRead)
    {
        UINT32 TempBytesRead;
        OS_RESULT Result;

        // we will assume failure for now
        Result = OS_RESOURCE_INSUFFICIENT_SPACE;

        // read in the number of bytes for the sequence size first
        TempBytesRead = GenericBufferRead(&IOBuffer->RXGenericBuffer, IOBuffer->NewLineSequenceSizeInBytes, UserBuffer, UserBufferSizeInBytes, FALSE);
        
        UserBufferSizeInBytes -= IOBuffer->NewLineSequenceSizeInBytes;

        // we can safely read the data out and get it as long as the 
        // user buffer is long enough to hold the data.
        while(UserBufferSizeInBytes != 0)
        {
            // now check if the sequence is in the user buffer
            if(memcmp(&UserBuffer[TempBytesRead - IOBuffer->NewLineSequenceSizeInBytes], IOBuffer->NewLineSequence, IOBuffer->NewLineSequenceSizeInBytes) == 0)
            {
                // the sequence did exist
                break;
            }

            // read another byte into the user buffer
            TempBytesRead += GenericBufferRead(&IOBuffer->RXGenericBuffer, 1, &UserBuffer[TempBytesRead], UserBufferSizeInBytes, FALSE);

            UserBufferSizeInBytes--;
        }

        // now check if the sequence is in the user buffer, we could have gotten out here from the line while(UserBufferSizeInBytes != 0)
        if(memcmp(&UserBuffer[TempBytesRead - IOBuffer->NewLineSequenceSizeInBytes], IOBuffer->NewLineSequence, IOBuffer->NewLineSequenceSizeInBytes) == 0)
        {
            // the sequence did exist
            Result = OS_SUCCESS;
        }

        // update their bytes read pointer if passed in
        if(BytesRead != (UINT32*)NULL)
            *BytesRead = TempBytesRead;

        return Result;
    }

    OS_RESULT IOBufferReadLine( IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesRead

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                )
    {
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
        OS_RESULT Result = NUMBER_OF_OS_RESULTS;

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if(BytesRead != (UINT32*)NULL)
            {
                if (RAMAddressValid((OS_WORD)BytesRead) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                *BytesRead = 0;
            }

            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;

            if (RAMAddressValid((OS_WORD)UserBuffer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            // their buffer has to at least be the size of the new line sequence
            if(UserBufferSizeInBytes < IOBuffer->NewLineSequenceSizeInBytes)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        // are we the owner?
        if(GetMutexOwner(&IOBuffer->IOBufferMutex) != gCurrentTask)
            return OS_RESOURCE_INVALID_OWNER;

        EnterCritical();

        // first read in any data from the hardware buffer
        OSUpdateIOBuffer(IOBufferID);

        // check, is the sequence in there already?
        if(GenericBufferContainsSequence(&IOBuffer->RXGenericBuffer, IOBuffer->NewLineSequence, IOBuffer->NewLineSequenceSizeInBytes) == TRUE)
        {
            // the sequence does exist, read it
            Result = OS_IOBufferReadLinePresent(IOBuffer, UserBuffer, UserBufferSizeInBytes, BytesRead);
        }
        else
        {
            // the sequence is not in there, is the buffer size bigger than the 
            // user buffer plus the new line sequence?
            if(GenericBufferGetSize(&IOBuffer->RXGenericBuffer) + IOBuffer->NewLineSequenceSizeInBytes > UserBufferSizeInBytes)
            {
                // we cannot read the line successfully no matter what
                // due to the users buffer size being too small
                Result = OS_RESOURCE_INSUFFICIENT_SPACE;
            }
            else
            {
                // if they do not want to block, leave
                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    if(TimeoutInTicks == 0)
                    {
                        // there wasn't enough data and the user did not want to wait
                        Result = OS_RESOURCE_INSUFFICIENT_DATA;
                    }
                    else
                #endif // #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    {
                        // read in everything and block the TASK

                        // read in everything to the user buffer
                        IOBuffer->BytesLeftToExchange = GenericBufferRead(&IOBuffer->RXGenericBuffer, GenericBufferGetSize(&IOBuffer->RXGenericBuffer), UserBuffer, UserBufferSizeInBytes, FALSE);

                        // copy over the user buffer size
                        IOBuffer->UserBufferSizeInBytes = UserBufferSizeInBytes;
                        
                        // now point to where the next byte will go
                        IOBuffer->TaskBuffer = UserBuffer;

                        // set the state to read until sequence
                        IOBuffer->RXState = IO_BUFFER_READ_UNTIL_SEQUENCE;

                        // remove the TASK from the ready queue and put it on the blocked list
                        OS_PlaceTaskOnBlockedList(gCurrentTask, &gMiscellaneousBlockedQueueHead, &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], BLOCKED, TRUE);

                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            if (TimeoutInTicks > 0)
                            {
                                // place on timer list, this isn't a hard time operation but it is better than nothing
                                OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], TimeoutInTicks, FALSE);
                            }
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                        // set the RX trigger level
                        PortIOBufferSetRXInterruptTriggerLevel(IOBufferID, IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL);

                        // now block the task
                        SurrenderCPU();
                        
                        // did we read enough to do a comparison, and did we get the sequence?
                        if(IOBuffer->NewLineSequenceSizeInBytes <= IOBuffer->BytesLeftToExchange && memcmp((const void*)&UserBuffer[(INT32)IOBuffer->BytesLeftToExchange - (INT32)IOBuffer->NewLineSequenceSizeInBytes], (const void*)IOBuffer->NewLineSequence, IOBuffer->NewLineSequenceSizeInBytes) == 0)
                        {
                            Result = OS_SUCCESS;
                        }
                        else
                        {
                            // we didn't
                            // they did not get the sequence
                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                if (gCurrentTask->DelayInTicks == TASK_TIMEOUT_DONE_VALUE)
                                    Result = OS_TASK_TIMEOUT;
                                else
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                    Result = OS_RESOURCE_INSUFFICIENT_SPACE;
                        }
                        
                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            // put the IO Buffer back into OPEN mode
                            OS_PutIOBufferInOpenMode(IOBuffer, FALSE);

                            gCurrentTask->DelayInTicks = 0;
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                        // if they passed this in populate it, this is populated the same way no matter if the method was a success or failure.
                        if(BytesRead != (UINT32*)NULL)
                            *BytesRead = IOBuffer->BytesLeftToExchange;
                    }
            }
        }

        ExitCritical();

        return Result;
    }
#endif // end of #if (USING_IO_BUFFER_READ_LINE_METHOD == 1)

#if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1)
    OS_RESULT IOBufferWriteBytes(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 NumberOfBytesToWrite, UINT32 *BytesWritten

                                #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                    , BOOL AsyncWrite
                                #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                    , INT32 TimeoutInTicks
                                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                )
    {
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
        OS_RESULT Result = OS_SUCCESS;

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            // check the bytes written parameter first so we can set it to zero
            if(BytesWritten != (UINT32*)NULL)
            {
                if (RAMAddressValid((OS_WORD)BytesWritten) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                *BytesWritten = 0;
            }  

            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;

            if (RAMAddressValid((OS_WORD)UserBuffer) == FALSE && ProgramAddressValid((OS_WORD)UserBuffer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(NumberOfBytesToWrite == 0)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
            if(IOBuffer->TXGenericBuffer == (GENERIC_BUFFER*)NULL)
                return OS_INVALID_OBJECT_USED;
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        
        // are we the owner?
        if(GetMutexOwner(&IOBuffer->IOBufferMutex) != gCurrentTask)
            return OS_RESOURCE_INVALID_OWNER;

        EnterCritical();

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
            // are we in an error state?

            // we cannot swap from Async to non Async or vice versa
            if(AsyncWrite == FALSE && IOBuffer->TXState == IO_BUFFER_ASYNC_WRITE)
            {
                ExitCritical();

                return OS_IO_BUFFER_INVALID_STATE;
            }
        
            if(AsyncWrite == TRUE && IOBuffer->TXState == IO_BUFFER_WRITE)
            {
                ExitCritical();

                return OS_IO_BUFFER_INVALID_STATE;
            }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        // we are clear to proceed

        // now proceed to queue up the data for write
        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
            if(AsyncWrite == TRUE)
            {
                UINT32 TempBytesWritten;
                
                // set the buffer state
                IOBuffer->TXState = IO_BUFFER_ASYNC_WRITE;

                // fill up the buffer with as much data as possible
                TempBytesWritten = GenericBufferWrite(IOBuffer->TXGenericBuffer, NumberOfBytesToWrite, UserBuffer);

                // set the TX interrupt level
                if(TempBytesWritten != 0)
                    PortIOBufferSetTXInterruptTriggerLevel(IOBufferID, TempBytesWritten);

                // write the bytes written in the user pointer if it is valid
                if(BytesWritten != (UINT32*)NULL)
                    *BytesWritten = TempBytesWritten;
            }
            else
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
            {
                // set the buffer state
                IOBuffer->TXState = IO_BUFFER_WRITE;

                // just queue up the user buffer for transmission
                IOBuffer->TaskBuffer = UserBuffer;

                IOBuffer->BytesLeftToExchange = NumberOfBytesToWrite;

                // add the task to the blocked list
                OS_PlaceTaskOnBlockedList(gCurrentTask, &gMiscellaneousBlockedQueueHead, &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], BLOCKED, TRUE);

                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    if (TimeoutInTicks > 0)
                    {
                        // place on timer list, this isn't a hard time operation but it is better than nothing
                        OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], TimeoutInTicks, FALSE);
                    }
                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                // set the TX interrupt level
                PortIOBufferSetTXInterruptTriggerLevel(IOBufferID, NumberOfBytesToWrite);

                // give up the CPU to the next TASK
                SurrenderCPU();

                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    gCurrentTask->DelayInTicks = 0;

                    // so we woke up, did all the data get sent?
                    if(IOBuffer->BytesLeftToExchange != 0)
                    {
                        // it was only a partial before the timeout
                        if(BytesWritten != (UINT32*)NULL)
                            *BytesWritten = NumberOfBytesToWrite - IOBuffer->BytesLeftToExchange;

                        Result = OS_TASK_TIMEOUT;
                    }
                    else
                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    {
                        // all data was written
                        if(BytesWritten != (UINT32*)NULL)
                            *BytesWritten = NumberOfBytesToWrite;
                    }

                // put the IO Buffer back into OPEN mode
                OS_PutIOBufferInOpenMode(IOBuffer, TRUE);
            }

        ExitCritical();

        return Result;
    }
#endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1)

#if (USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1)
    OS_RESULT IOBufferWriteBytesUntil(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesWritten, BYTE WriteUntilByte

                                #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                    , BOOL AsyncWrite
                                #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                )
    {
        UINT32 i;

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            // check the bytes written parameter first so we can set it to zero
            if(BytesWritten != (UINT32*)NULL)
            {
                if (RAMAddressValid((OS_WORD)BytesWritten) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                *BytesWritten = 0;
            }  

            if (RAMAddressValid((OS_WORD)UserBuffer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(UserBufferSizeInBytes == 0)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        // now search for the value they want to write
        for(i = 0; i < UserBufferSizeInBytes; i++)
        {
            if(UserBuffer[i] == WriteUntilByte)
            {
                return IOBufferWriteBytes(IOBufferID, UserBuffer, i, BytesWritten

                        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                            , AsyncWrite
                        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            ,TimeoutInTicks
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                        );
            }
        }

        return OS_INVALID_ARGUMENT;
    }
#endif // end of #if (USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1)

#if (USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
    OS_RESULT IOBufferWriteLine(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesWritten

                                #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                    , BOOL AsyncWrite
                                #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                                #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                )
    {
        OS_RESULT Result;
        UINT32 TempBytesWritten;

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            // check the bytes written parameter first so we can set it to zero
            if(BytesWritten != (UINT32*)NULL)
            {
                if (RAMAddressValid((OS_WORD)BytesWritten) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                *BytesWritten = 0;
            }

            // check that their buffer is within the limits of the string
            if(strlen((const char*)UserBuffer) > UserBufferSizeInBytes)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        // write until a zero is found
        Result = IOBufferWriteBytes(IOBufferID, UserBuffer, (UINT32)strlen((const char*)UserBuffer), &TempBytesWritten

                        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                            , AsyncWrite
                        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            ,TimeoutInTicks
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                        );

        // update the byte count if necessary 
        if(BytesWritten != (UINT32*)NULL)
            *BytesWritten = TempBytesWritten;

        // if the call succeeded, write the end of line sequence
        if(Result == OS_SUCCESS)
        {
            // did we write all the data from the user?
            if(TempBytesWritten == (UINT32)strlen((const char*)UserBuffer))
            {
                // we wrote all the data from the user buffer

                // this is valid because IOBufferWriteBytes() already asserted that
                IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);
                
                // wait until we can fit in the new line sequence
                #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                    while(GenericBufferGetRemainingBytes(IOBuffer->TXGenericBuffer) < strlen((const char*)IOBuffer->NewLineSequence));
                #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                // now write the line ending sequence
                Result = IOBufferWriteBytes(IOBufferID, IOBuffer->NewLineSequence, (UINT32)strlen((const char*)IOBuffer->NewLineSequence), &TempBytesWritten

                    #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                        , AsyncWrite
                    #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                    #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                        ,WAIT_FOREVER
                    #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                    );

                // update the byte count if necessary 
                if(BytesWritten != (UINT32*)NULL)
                    *BytesWritten = *BytesWritten + TempBytesWritten;
            }
        }

        return Result;
    }
#endif // end of #if (USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

#if (USING_IO_BUFFER_RELEASE_METHOD == 1)
    OS_RESULT IOBufferRelease(IO_BUFFER_ID IOBufferID)
    {
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        return ReleaseMutex(&IOBuffer->IOBufferMutex);
    }
#endif // end of #if (USING_IO_BUFFER_RELEASE_METHOD == 1)

#if (USING_IO_BUFFER_CONTAINS_METHOD == 1)
    INT32 IOBufferContains(IO_BUFFER_ID IOBufferID, BYTE Value

                              #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                  , BOOL TXBuffer
                              #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                              )
    {
        INT32 Result;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return -1;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        EnterCritical();
        
        // first read in any data from the hardware buffer
        OSUpdateIOBuffer(IOBufferID);

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        {
            GENERIC_BUFFER *GenericBuffer;

            // do they want to look in the TX or RX buffer?
            if(TXBuffer == TRUE)
                GenericBuffer = IOBuffer->TXGenericBuffer;
            else
                GenericBuffer = &IOBuffer->RXGenericBuffer;

            // perform the search
            Result = (INT32)GenericBufferContains(GenericBuffer, Value);
        }
        #else
        {
            // perform the search
            Result = (INT32)GenericBufferContains(&IOBuffer->RXGenericBuffer, Value);
        }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        ExitCritical();

        return Result;
    }
#endif // end of #if (USING_IO_BUFFER_CONTAINS_METHOD == 1)

#if (USING_IO_BUFFER_CONTAINS_SEQUENCE_METHOD == 1)
    OS_RESULT IOBufferContainsSequence(IO_BUFFER_ID IOBufferID, BYTE *Sequence, UINT32 SequenceSizeInBytes

                                      #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                          , BOOL TXBuffer
                                      #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                                      )
    {
        BOOL Result;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;

            if (RAMAddressValid((OS_WORD)Sequence) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(SequenceSizeInBytes == 0)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        EnterCritical();
        
        // first read in any data from the hardware buffer
        OSUpdateIOBuffer(IOBufferID);

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        {
            GENERIC_BUFFER *GenericBuffer;

            // do they want to look in the TX or RX buffer?
            if(TXBuffer == TRUE)
                GenericBuffer = IOBuffer->TXGenericBuffer;
            else
                GenericBuffer = &IOBuffer->RXGenericBuffer;

            // perform the search
            Result = GenericBufferContainsSequence(GenericBuffer, Sequence, SequenceSizeInBytes);
        }
        #else
        {
            // perform the search
            Result = GenericBufferContainsSequence(&IOBuffer->RXGenericBuffer, Sequence, SequenceSizeInBytes);
        }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        ExitCritical();

        if(Result == TRUE)
            return OS_SUCCESS;
        else
            return OS_FALSE;
    }
#endif // end of #if (USING_IO_BUFFER_CONTAINS_SEQUENCE_METHOD == 1)

#if (USING_IO_BUFFER_PEEK_METHOD == 1)
    OS_RESULT IOBufferPeek(IO_BUFFER_ID IOBufferID, UINT32 NumberOfBytesToPeekAt, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesPeekedAt

                          #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                              , BOOL TXBuffer
                          #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                          )
    {
        UINT32 TempBytesRead;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if(BytesPeekedAt != (UINT32*)NULL)
            {
                if (RAMAddressValid((OS_WORD)BytesPeekedAt) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                *BytesPeekedAt = 0;
            }

            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;

            if (RAMAddressValid((OS_WORD)UserBuffer) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            if(UserBufferSizeInBytes == 0 || NumberOfBytesToPeekAt == 0 || NumberOfBytesToPeekAt > UserBufferSizeInBytes)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        EnterCritical();

        // first read in any data from the hardware buffer
        OSUpdateIOBuffer(IOBufferID);

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        {
            GENERIC_BUFFER *GenericBuffer;

            // do they want to look in the TX or RX buffer?
            if(TXBuffer == TRUE)
                GenericBuffer = IOBuffer->TXGenericBuffer;
            else
                GenericBuffer = &IOBuffer->RXGenericBuffer;

            // now perform the peek
            TempBytesRead = GenericBufferPeek(GenericBuffer, NumberOfBytesToPeekAt, UserBuffer, UserBufferSizeInBytes, FALSE);
        }
        #else
        {
            // now perform the peek
            TempBytesRead = GenericBufferPeek(&IOBuffer->RXGenericBuffer, NumberOfBytesToPeekAt, UserBuffer, UserBufferSizeInBytes, FALSE);
        }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        if(BytesPeekedAt != (UINT32*)NULL)
            *BytesPeekedAt = TempBytesRead;

        ExitCritical();

        return OS_SUCCESS;
    }
#endif // end of #if (USING_IO_BUFFER_PEEK_METHOD == 1)

#if(USING_IO_BUFFER_FLUSH_METHOD == 1)
    OS_RESULT IOBufferFlush(IO_BUFFER_ID IOBufferID

                           #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                               , BOOL TXBuffer
                           #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                           )
    {
        OS_RESULT Result;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        // are we the owner?
        if(GetMutexOwner(&IOBuffer->IOBufferMutex) != gCurrentTask)
            return OS_RESOURCE_INVALID_OWNER;

        EnterCritical();

        // first read in any data from the hardware buffer
        // this is in case of an RX flush, but it doesn't
        // hurt to read this if we are doing a TX flush
        OSUpdateIOBuffer(IOBufferID);

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        {
            GENERIC_BUFFER *GenericBuffer;

            // do they want to look in the TX or RX buffer?
            if(TXBuffer == TRUE)
                GenericBuffer = IOBuffer->TXGenericBuffer;
            else
                GenericBuffer = &IOBuffer->RXGenericBuffer;

            // flush the buffer
            if(GenericBufferFlush(GenericBuffer) == TRUE)
                Result = OS_SUCCESS;
            else
                Result = OS_INVALID_OBJECT_USED;

            // if we are flushing the TX buffer and we're in an Async Write, clear it
            if(IOBuffer->TXState == IO_BUFFER_ASYNC_WRITE && TXBuffer == TRUE)
            {
                // set the interrupt triggering level to zero since there are no more bytes to write
                PortIOBufferSetTXInterruptTriggerLevel(IOBufferID, 0);

                // now put it back to open mode (default mode)
                OS_PutIOBufferInOpenMode(IOBuffer, TXBuffer);
            }
        }
        #else
        {
            // flush the buffer
            if(GenericBufferFlush(&IOBuffer->RXGenericBuffer) == TRUE)
                Result = OS_SUCCESS;
            else
                Result = OS_INVALID_OBJECT_USED;
        }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        ExitCritical();

        return Result;
    }
#endif // end of #if(USING_IO_BUFFER_FLUSH_METHOD == 1)

#if (USING_IO_BUFFER_GET_SIZE_METHOD == 1)
    UINT32 IOBufferGetSize(IO_BUFFER_ID IOBufferID

                          #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                              , BOOL TXBuffer
                          #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                          )
    {
        UINT32 Size;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        EnterCritical();

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        {
            GENERIC_BUFFER *GenericBuffer;

            // do they want to look in the TX or RX buffer?
            if(TXBuffer == TRUE)
                GenericBuffer = IOBuffer->TXGenericBuffer;
            else
                GenericBuffer = &IOBuffer->RXGenericBuffer;

            // get how many bytes are written to the buffer
            Size = GenericBufferGetSize(GenericBuffer);
        }
        #else
        {
            // get how many bytes are written to the buffer
            Size = GenericBufferGetSize(&IOBuffer->RXGenericBuffer);
        }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        ExitCritical();

        return Size;
    }
#endif // end of #if (USING_IO_BUFFER_GET_SIZE_METHOD == 1)

#if (USING_IO_BUFFER_GET_REMAINING_BYTES_METHOD == 1)
    UINT32 IOBufferGetRemainingBytes(IO_BUFFER_ID IOBufferID

                                    #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                        , BOOL TXBuffer
                                    #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                                    )
    {
        UINT32 Size;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        EnterCritical();

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        {
            GENERIC_BUFFER *GenericBuffer;

            // do they want to look in the TX or RX buffer?
            if(TXBuffer == TRUE)
                GenericBuffer = IOBuffer->TXGenericBuffer;
            else
                GenericBuffer = &IOBuffer->RXGenericBuffer;

            // get the remaining size
            Size = GenericBufferGetRemainingBytes(GenericBuffer);
        }
        #else
        {
            // get the remaining size
            Size = GenericBufferGetRemainingBytes(&IOBuffer->RXGenericBuffer);
        }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        ExitCritical();

        return Size;
    }
#endif // end of #if (USING_IO_BUFFER_GET_REMAINING_BYTES_METHOD == 1)

#if (USING_IO_BUFFER_GET_CAPACITY_METHOD == 1)
    UINT32 IOBufferGetCapacity(IO_BUFFER_ID IOBufferID

                              #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                  , BOOL TXBuffer
                              #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                              )
    {
        UINT32 Size;
        IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

        #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                return OS_INVALID_ARGUMENT;
        #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

        EnterCritical();

        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        {
            GENERIC_BUFFER *GenericBuffer;

            // do they want to look in the TX or RX buffer?
            if(TXBuffer == TRUE)
                GenericBuffer = IOBuffer->TXGenericBuffer;
            else
                GenericBuffer = &IOBuffer->RXGenericBuffer;

            // get the buffer capacity
            Size = GenericBufferGetCapacity(GenericBuffer);
        }
        #else
        {
            // get the buffer capacity
            Size = GenericBufferGetCapacity(&IOBuffer->RXGenericBuffer);
        }
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

        ExitCritical();

        return Size;
    }
#endif // end of #if (USING_IO_BUFFER_GET_CAPACITY_METHOD == 1)
    
#if (USING_IO_BUFFER_READ_LINE_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
    #if(USING_IO_BUFFER_GET_NEW_LINE_METHOD == 1)
        BYTE *IOBufferGetNewLine(IO_BUFFER_ID IOBufferID)
        {
            IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

            #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
                if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                    return (BYTE*)NULL;
            #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

            return IOBuffer->NewLineSequence;
        }
    #endif // end of #if(USING_IO_BUFFER_GET_NEW_LINE_METHOD == 1)

    #if(USING_IO_BUFFER_SET_NEW_LINE_METHOD == 1)
        OS_RESULT IOBufferSetNewLine(IO_BUFFER_ID IOBufferID, BYTE *NewLine, UINT32 NewLineSequenceSizeInBytes)
        {
            OS_RESULT Result;
            IO_BUFFER *IOBuffer = OS_GetIOBufferHandle(IOBufferID);

            #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)
                if (RAMAddressValid((OS_WORD)IOBuffer) == FALSE)
                    return OS_INVALID_ARGUMENT;

                if (RAMAddressValid((OS_WORD)NewLine) == FALSE && ProgramAddressValid((OS_WORD)NewLine) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                if(NewLineSequenceSizeInBytes == 0)
                    return OS_INVALID_ARGUMENT;
            #endif // end of #if (USING_CHECK_IO_BUFFER_PARAMETERS == 1)

            // are we the owner?
            if(GetMutexOwner(&IOBuffer->IOBufferMutex) != gCurrentTask)
                return OS_RESOURCE_INVALID_OWNER;

            // are we currently transmitting?
            EnterCritical();

            if(IOBuffer->TXState != IO_BUFFER_OPEN || IOBuffer->RXState != IO_BUFFER_OPEN)
            {
                // we can only change the new line sequence if we are in open
                // mode only.
                Result = OS_IO_BUFFER_INVALID_STATE;
            }
            else
            {
                // now set the new line sequence
                IOBuffer->NewLineSequence = NewLine;
                IOBuffer->NewLineSequenceSizeInBytes = NewLineSequenceSizeInBytes;

                Result = OS_SUCCESS;
            }

            ExitCritical();

            return Result;
        }
    #endif // end of #if(USING_IO_BUFFER_SET_NEW_LINE_METHOD == 1)
#endif // end of #if (USING_IO_BUFFER_READ_LINE_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
