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
	IO_BUFFER Description:
		An IO_BUFFER can be used to accept incoming data from a peripheral.  This
        allows a TASK to either block waiting for the data to come in, or for a
        TASK to do other meaningful things while data accumulates in the IO_BUFFER.

        A TASK can also write data to an IO_BUFFER that will then be sent out the
        associated peripheral.  This can be done on a blocking or asynchronous basis.

		The files IOBufferPort.h/c must contain the code needed for the reading and
        writing of data to/from the actual peripheral.  Therefore a port for the 
        microcontroller being used must be done.
*/

#ifndef IO_BUFFER_H
    #define IO_BUFFER_H

#include "RTOSConfig.h"
#include "../../Generic Libraries/Generic Buffer/GenericBuffer.h"
#include "../Semaphore/Mutex/Mutex.h"

#if (USING_IO_BUFFERS != 1)
	#error "USING_IO_BUFFERS must be defined as a 1 in RTOSConfig.h to use IO Buffers"
#endif // end of #if (USING_IO_BUFFERS != 1)

#define IO_BUFFER_HIGHEST_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL             0xFFFFFFFF
#define IO_BUFFER_UNKNOWN_NUMBER_OF_BYTES_FOR_TRIGGER_LEVEL             0xFFFFFFFE

/*
 * These are the different error states the UART is assumed to support.
 * These error states are supposed to be hardware independent.
 */
typedef enum
{
    NO_UART_ERROR = 0,
    PARITY_ERROR = 1,
    FRAMING_ERROR = 2,
    OVERRUN_ERROR = 4,
}UART_ERROR;

/*
 * These are the different error states the CAN modules are assumed to support.
 * These error states are supposed to be hardware independent.
 */
typedef enum
{
    NO_CAN_ERROR = 0,
    CAN_RX_OVERFLOW_EVENT_ERROR = 1,
    CAN_SYSTEM_ERROR_EVENT_ERROR = 2,
    CAN_BUS_ERROR_EVENT_ERROR = 4,
    CAN_INVALID_RX_MESSAGE_EVENT_ERROR = 8,
    CAN_RX_PASSIVE_ERROR_STATE = 16,
    CAN_TX_PASSIVE_ERROR_STATE = 32
}CAN_ERROR;

/*
 * These are the different error states the RX or TX buffer inside the IO_BUFFER can have.
 */
typedef enum
{
    IO_BUFFER_CLOSED = 0,
    IO_BUFFER_OPEN, // the Buffer is open and currently has no data to read or write
    IO_BUFFER_READ_NUMBER_OF_BYTES,
    IO_BUFFER_READ_UNTIL_BYTE,
    IO_BUFFER_READ_UNTIL_SEQUENCE,
    IO_BUFFER_WRITE,
    IO_BUFFER_ASYNC_WRITE,
    IO_BUFFER_OVERFLOW,
    IO_BUFFER_HARDWARE_ERROR,
    NUMBER_OF_IO_BUFFER_STATES
}IO_BUFFER_STATE;

typedef enum
{
    INVALID_IO_BUFFER = 0,
    
    #if (USING_UART_1_IO_BUFFER == 1)
        IO_BUFFER_UART_1_ID,
    #endif // end of #if (USING_UART_1_IO_BUFFER == 1)
            
    #if (USING_UART_2_IO_BUFFER == 1)
        IO_BUFFER_UART_2_ID,
    #endif // end of #if (USING_UART_2_IO_BUFFER == 1)

    #if (USING_UART_3_IO_BUFFER == 1)
        IO_BUFFER_UART_3_ID,
    #endif // end of #if (USING_UART_3_IO_BUFFER == 1)

    #if (USING_UART_4_IO_BUFFER == 1)
        IO_BUFFER_UART_4_ID,
    #endif // end of #if (USING_UART_4_IO_BUFFER == 1)

    #if (USING_UART_5_IO_BUFFER == 1)
        IO_BUFFER_UART_5_ID,
    #endif // end of #if (USING_UART_5_IO_BUFFER == 1)

    #if (USING_UART_6_IO_BUFFER == 1)
        IO_BUFFER_UART_6_ID,
    #endif // end of #if (USING_UART_6_IO_BUFFER == 1)

    #if (USING_CAN_1_IO_BUFFER == 1)
        IO_BUFFER_CAN_1_ID,
    #endif // end of #if (USING_CAN_1_IO_BUFFER == 1)

    #if (USING_CAN_2_IO_BUFFER == 1)
        IO_BUFFER_CAN_2_ID,
    #endif // end of #if (USING_CAN_2_IO_BUFFER == 1)

    #if (USING_ADC_1_IO_BUFFER == 1)
        IO_BUFFER_ADC_1_ID,
    #endif // end of #if (USING_ADC_1_IO_BUFFER == 1)

    NUMBER_OF_BUFFER_IDS
}IO_BUFFER_ID;

typedef struct
{
    BYTE *TaskBuffer; // this is a pointer to the users buffer should they be in a read operation, in IO_BUFFER_READ_NUMBER_OF_BYTES this buffer advances
    
    #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
        GENERIC_BUFFER RXGenericBuffer;
    #endif // end of #if (USING_IO_BUFFER_READ_BYTES_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1 || USING_IO_BUFFER_READ_LINE_METHOD == 1)
    
    #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
        #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
            GENERIC_BUFFER *TXGenericBuffer;
        #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
    #endif // end of #if (USING_IO_BUFFER_WRITE_BYTES_METHOD == 1 || USING_IO_BUFFER_WRITE_UNTIL_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)

    MUTEX IOBufferMutex;
    IO_BUFFER_STATE RXState;
    IO_BUFFER_STATE TXState;
    
    /*
     * IO_BUFFER_READ_NUMBER_OF_BYTES: BytesLeftToExchange is how many more bytes to read.
     * IO_BUFFER_READ_UNTIL_BYTE: BytesLeftToExchange is how many bytes have been currently read.
     * IO_BUFFER_READ_UNTIL_SEQUENCE: BytesLeftToExchange is how many bytes have been currently read.
     * IO_BUFFER_WRITE: BytesLeftToExchange is the number of bytes left to write.
     */
    UINT32 BytesLeftToExchange; // this holds how many remaining bytes need to be TX or RX
    
    #if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
        // only required for read until method
        BYTE ByteToReadUntil;
    #endif // end of #if (USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
    
    #if (USING_IO_BUFFER_READ_LINE_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
        BYTE *NewLineSequence;
        UINT32 NewLineSequenceSizeInBytes;
    #endif // end of #if (USING_IO_BUFFER_READ_LINE_METHOD == 1 || USING_IO_BUFFER_WRITE_LINE_METHOD == 1)
        
    #if (USING_IO_BUFFER_READ_LINE_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
        // only required for read until sequence method
        UINT32 UserBufferSizeInBytes;
    #endif // end of #if (USING_IO_BUFFER_READ_LINE_METHOD == 1 || USING_IO_BUFFER_READ_UNTIL_METHOD == 1)
    
}IO_BUFFER;

// None of the below methods encapsulated between the comments are user callable
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL OSUpdateIOBuffer(IO_BUFFER_ID BufferID);

BOOL OS_RemoveTaskFromIOBufferBlockedList(IO_BUFFER *IOBuffer);

UINT32 OS_ReadFromIOBuffer(IO_BUFFER *IOBuffer, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, UINT32 DataLength);

BOOL OS_WriteToIOBuffer(IO_BUFFER_ID IOBufferID, BYTE *Data, UINT32 DataLength);

UINT32 OS_IOBufferGetNumberOfBytesLeftToTransmit(IO_BUFFER *IOBuffer);

IO_BUFFER *OS_GetIOBufferHandle(IO_BUFFER_ID IOBufferID);
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
    OS_RESULT InitIOBuffer(IO_BUFFER_ID IOBufferID, BYTE *RXBuffer, UINT32 RXBufferSize, BYTE *TXBuffer, UINT32 TXBufferSize)

	Description: This method creates an IO_BUFFER which can be used to communicate
    data in our out of the CPU through peripherals.  This method should only be 
    called once for any IO_BUFFER_ID.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to initialize.

        BYTE *RXBuffer - A pointer to a valid BYTE *buffer where the incoming data
        will be stored.
 
        UINT32 RXBufferSizeInBytes - The size in bytes of the buffer that RXBuffer is pointing
        to.  This size must be at least 3 bytes large.
  
        BYTE *TXBuffer - A pointer to a valid BYTE *buffer where the outgoing data
        will be stored before transmit.  If (BYTE*)NULL is passed in then TXBufferSizeInBytes
        specifies the size of a buffer to allocate on the OS Heap.
  
        UINT32 TXBufferSizeInBytes - The size in bytes of the buffer that TXBuffer is pointing
        to.  If a value of 0 is specified no TX buffer is created, otherwise a buffer
        that is TXBufferSizeInBytes bytes large will be created on the OS Heap for the TX buffer.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned on success, an error code otherwise.

	Notes:
		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must be defined as 
          a 1 to use the TX buffer functionality.
 
        - To guarantee there are no issues with the IO_BUFFER, it must be initialized
          after calling InitOS() and before calling StartOSScheduler().

	See Also:
		- None
*/
OS_RESULT InitIOBuffer(IO_BUFFER_ID IOBufferID, BYTE *RXBuffer, UINT32 RXBufferSizeInBytes                      
                       #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                           , BYTE *TXBuffer
                           , UINT32 TXBufferSizeInBytes
                       #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                       );

/*
	OS_RESULT GetIOBuffer(IO_BUFFER_ID IOBufferID, INT32 TimeoutInTicks)

	Description: This method will allow the current TASK to acquire the specified
 	IO_BUFFER.  The IO_BUFFER must be acquired to modify, read from, or write to it.

	Blocking: Yes

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to get.

		INT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the data to be present.  The timeout is in OS timer ticks.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned on success, an error code otherwise.

	Notes:
		- This method must be called before the majority of other IOBuffer methods.
 
		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- IOBufferRelease()
*/
OS_RESULT GetIOBuffer(  IO_BUFFER_ID IOBufferID
        
                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                            , INT32 TimeoutInTicks
                        #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                    
                        );

/*
    OS_RESULT IOBufferReadBytes(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 NumberOfBytesToRead, UINT32 *BytesRead, INT32 TimeoutInTicks)

	Description: This method will read the specified number of bytes out of the RX buffer of
	the specified IO_BUFFER.  This method will only block if the RX buffer of the IO_BUFFER
	has less than NumberOfBytesToRead in it.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to read the data from.

		BYTE *UserBuffer - The user buffer to store the data being read.

		UINT32 UserBufferSizeInBytes - The size of UserBuffer in bytes.

		UINT32 NumberOfBytesToRead - The number of bytes to read out of the RX buffer
		of the specified IO_BUFFER.

		UINT32 *BytesRead - This is an optional parameter.  If specified the number of
		bytes read will be recorded here.  If this method returns OS_SUCCESS, then 
		*BytesRead = NumberOfBytesToRead.  If this method does not return OS_SUCCESS then
		*BytesRead < NumberOfBytesToRead.
		
		INT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the data to be present.  The timeout is in OS timer ticks.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned if all specified bytes were successfully read.
		Otherwise an error code is returned.

	Notes:
		- USING_IO_BUFFER_READ_BYTES_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- A TASK must own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

		- If the TASK times out before all data is read, once the TASK resumes
		  one more check will be done in the IO_BUFFER to see if the data is
		  available.  If it is available it will be read and OS_SUCCESS will
		  be returned even though the data was technically all there within the
		  specified timeout period.

	See Also:
		- IOBufferReadUntil(), IOBufferReadLine(), GetIOBuffer()
*/
OS_RESULT IOBufferReadBytes(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 NumberOfBytesToRead, UINT32 *BytesRead
        
                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                            );

/*
    OS_RESULT IOBufferReadUntil(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, BYTE ReadUntil, UINT32 *BytesRead, INT32 TimeoutInTicks)

	Description: This method will read from the RX buffer of the specified IO_BUFFER until
	the ReadUntil byte is found.  The ReadUntil bytes is also read into the UserBuffer if found.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to read the data from.

		BYTE *UserBuffer - The user buffer to store the data being read.

		UINT32 UserBufferSizeInBytes - The size of UserBuffer in bytes.

		BYTE ReadUntil - The data to read up until in the RX buffer of the IO_BUFFER.
		This value is also read from the RX buffer.

		UINT32 *BytesRead - This is an optional parameter.  If specified the number of
		bytes read will be recorded here.
		
		INT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the data to be present.  The timeout is in OS timer ticks.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned if the ReadUntil byte was found and read.
		Otherwise an error code is returned.

	Notes:
		- USING_IO_BUFFER_READ_UNTIL_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- A TASK must own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

		- If the TASK times out before all data is read, once the TASK resumes
		  one more check will be done in the IO_BUFFER to see if the data is
		  available.  If it is available it will be read and OS_SUCCESS will
		  be returned even though the data was technically all there within the
		  specified timeout period.

	See Also:
		- IOBufferReadBytes(), IOBufferReadLine(), GetIOBuffer()
*/
OS_RESULT IOBufferReadUntil(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, BYTE ReadUntil, UINT32 *BytesRead
        
                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                            );

/*
    OS_RESULT IOBufferReadLine( IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesRead, INT32 TimeoutInTicks)

	Description: This method will read from the RX buffer of the specified IO_BUFFER until
	the new line sequence is found.  The new line sequence is also read from the RX buffer.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to read the data from.

		BYTE *UserBuffer - The user buffer to store the data being read.

		UINT32 UserBufferSizeInBytes - The size of UserBuffer in bytes.

		UINT32 *BytesRead - This is an optional parameter.  If specified the number of
		bytes read will be recorded here.
		
		INT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the event to occur.  The timeout is in OS timer ticks.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned if the new line sequence was found and read.
		Otherwise an error code is returned.

	Notes:
		- USING_IO_BUFFER_READ_LINE_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- A TASK must own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

		- If the TASK times out before all data is read, once the TASK resumes
		  one more check will be done in the IO_BUFFER to see if the data is
		  available.  If it is available it will be read and OS_SUCCESS will
		  be returned even though the data was technically all there within the
		  specified timeout period.

	See Also:
		- IOBufferReadBytes(), IOBufferReadUntil(), GetIOBuffer(), IOBufferGetNewLine(), IOBufferSetNewLine()
*/
OS_RESULT IOBufferReadLine( IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesRead
        
                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                            );

/*
    OS_RESULT IOBufferWriteBytes(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 NumberOfBytesToWrite, UINT32 *BytesWritten, BOOL AsyncWrite, INT32 TimeoutInTicks)

	Description: This method will write the specified number of bytes to the TX buffer
	of the specified IO_BUFFER.  At which point the OS will begin to load the data into
	the hardware buffers of the microcontroller until all data is sent.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to write data to its TX buffer.

		BYTE *UserBuffer - The user buffer which has the data to TX.

		UINT32 NumberOfBytesToWrite - The number of bytes from the UserBuffer to place
		into the TX buffer of the specified IO_BUFFER.

		UINT32 *BytesWritten - This is an optional parameter.  If specified then the 
		number of bytes successfully written to the TX buffer of the IO_BUFFER are
		recorded here.

		BOOL AsyncWrite - If TRUE, the calling TASK will return from this method once
		all data is written to the TX buffer of the specified IO_BUFFER.  If FALSE, 
		the calling TASK will wait for all data written to the TX buffer of the 
		specified IO_BUFFER to be transmitted out the hardware of the microcontroller
		before returning from this method.

		INT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the write to finish.  The timeout is in OS timer ticks.
		If AsyncWrite is TRUE, the timeout specifies how long to wait before all
		data is written to the TX buffer of the specified IO_BUFFER.  If AsyncWrite
		is FALSE or not present, the timeout specifies how long to wait before all
		data is transmitted out of the hardware of the microcontroller.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned if all bytes were successfully written to
		the TX buffer of the IO_BUFFER.  Otherwise an error code is returned.

	Notes:
		- USING_IO_BUFFER_WRITE_BYTES_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- A TASK must own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must be defined
		  as a 1 to use the BOOL AsyncWrite argument.  If this feature is not
		  used, the default behavior of this method is equivalent to AsyncWrite
		  being set to FALSE.

		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

		- If the TASK times out, any data pending transmission in the TX buffer
		  will still be transmitted out the hardware of the microcontroller.

	See Also:
		- IOBufferWriteBytesUntil(), IOBufferWriteLine(), GetIOBuffer()
*/
OS_RESULT IOBufferWriteBytes(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 NumberOfBytesToWrite, UINT32 *BytesWritten

                            #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                , BOOL AsyncWrite
                            #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        
                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                            );

/*
    OS_RESULT IOBufferWriteBytesUntil(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesWritten, BYTE WriteUntilByte, BOOL AsyncWrite, INT32 TimeoutInTicks)

	Description: This method will continue to write data from the UserBuffer to the TX
	buffer of the specified IO_BUFFER until the WriteUntilByte is found.  The WriteUntilByte
	is also written to the TX buffer of the specified IO_BUFFER.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to write data to its TX buffer.

		BYTE *UserBuffer - The user buffer which has the data to TX.

		UINT32 UserBufferSizeInBytes - The size in bytes of UserBuffer.

		UINT32 *BytesWritten - This is an optional parameter.  If specified then the 
		number of bytes successfully written to the TX buffer of the IO_BUFFER are
		recorded here.

		BYTE WriteUntilByte - The byte to write until.  All bytes in the UserBuffer
		from location zero to the WriteUntilByte will be written to the TX buffer of
		the specified IO_BUFFER.  This includes the WriteUntilByte too.

		BOOL AsyncWrite - If TRUE, the calling TASK will return from this method once
		all data is written to the TX buffer of the specified IO_BUFFER.  If FALSE, 
		the calling TASK will wait for all data written to the TX buffer of the 
		specified IO_BUFFER to be transmitted out the hardware of the microcontroller
		before returning from this method.

		INT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the write to finish.  The timeout is in OS timer ticks.
		If AsyncWrite is TRUE, the timeout specifies how long to wait before all
		data is written to the TX buffer of the specified IO_BUFFER.  If AsyncWrite
		is FALSE or not present, the timeout specifies how long to wait before all
		data is transmitted out of the hardware of the microcontroller.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned if all bytes were successfully written to
		the TX buffer of the IO_BUFFER.  Otherwise an error code is returned.

	Notes:
		- USING_IO_BUFFER_WRITE_UNTIL_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- A TASK must own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must be defined
		  as a 1 to use the BOOL AsyncWrite argument.  If this feature is not
		  used, the default behavior of this method is equivalent to AsyncWrite
		  being set to FALSE.

		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

		- If the TASK times out, any data pending transmission in the TX buffer
		  will still be transmitted out the hardware of the microcontroller.

	See Also:
		- IOBufferWriteBytes(), IOBufferWriteLine(), GetIOBuffer()
*/
OS_RESULT IOBufferWriteBytesUntil(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesWritten, BYTE WriteUntilByte

                            #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                , BOOL AsyncWrite
                            #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                            );

/*
    OS_RESULT IOBufferWriteLine(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 NumberOfBytesToWrite, UINT32 *BytesWritten, BOOL AsyncWrite, INT32 TimeoutInTicks)

	Description: This method will write the specified number of bytes from the UserBuffer
	to the TX buffer of the specified IO_BUFFER.  Then the new line sequence will be written
	to the TX buffer of the specified IO_BUFFER.  At which point the OS will begin to 
	load the data into the hardware buffers of the microcontroller until all data is sent.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to write data to its TX buffer.

		BYTE *UserBuffer - The user buffer which has the data to TX.

		UINT32 NumberOfBytesToWrite - The number of bytes from the UserBuffer to place
		into the TX buffer of the specified IO_BUFFER.

		UINT32 *BytesWritten - This is an optional parameter.  If specified then the 
		number of bytes successfully written to the TX buffer of the IO_BUFFER are
		recorded here.

		BOOL AsyncWrite - If TRUE, the calling TASK will return from this method once
		all data is written to the TX buffer of the specified IO_BUFFER.  If FALSE, 
		the calling TASK will wait for all data written to the TX buffer of the 
		specified IO_BUFFER to be transmitted out the hardware of the microcontroller
		before returning from this method.

		INT32 TimeoutInTicks - If a timeout is desired a value greater than
		zero should be specified.  Otherwise a value of zero means to wait
		indefinitely for the write to finish.  The timeout is in OS timer ticks.
		If AsyncWrite is TRUE, the timeout specifies how long to wait before all
		data is written to the TX buffer of the specified IO_BUFFER.  If AsyncWrite
		is FALSE or not present, the timeout specifies how long to wait before all
		data is transmitted out of the hardware of the microcontroller.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned if all bytes were successfully written to
		the TX buffer of the IO_BUFFER.  Otherwise an error code is returned.

	Notes:
		- USING_IO_BUFFER_WRITE_LINE_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- A TASK must own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must be defined
		  as a 1 to use the BOOL AsyncWrite argument.  If this feature is not
		  used, the default behavior of this method is equivalent to AsyncWrite
		  being set to FALSE.

		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

		- If the TASK times out, any data pending transmission in the TX buffer
		  will still be transmitted out the hardware of the microcontroller.

	See Also:
		- IOBufferWriteBytes(), IOBufferWriteBytesUntil(), GetIOBuffer(), IOBufferGetNewLine(), 
		  IOBufferSetNewLine()
*/
OS_RESULT IOBufferWriteLine(IO_BUFFER_ID IOBufferID, BYTE *UserBuffer, UINT32 NumberOfBytesToWrite, UINT32 *BytesWritten

                            #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                , BOOL AsyncWrite
                            #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
        
                            #if (USING_TASK_DELAY_TICKS_METHOD == 1)
                                , INT32 TimeoutInTicks
                            #endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                            );

/*
	OS_RESULT IOBufferRelease(IO_BUFFER_ID IOBufferID)

	Description: This method will release the IO_BUFFER should it be owned by
    the calling TASK.  Once released another TASK can acquire the IO_BUFFER with
    a call to GetIOBuffer().

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to release.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned on success, an error code otherwise.

	Notes:
        - USING_IO_BUFFER_RELEASE_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.
 
		- This method can only be called if the TASK currently owns the IOBuffer
          from a previous call to GetIOBuffer().

	See Also:
		- GetIOBuffer()
*/
OS_RESULT IOBufferRelease(IO_BUFFER_ID IOBufferID);

/*
    INT32 IOBufferContains(IO_BUFFER_ID IOBufferID, BYTE Value, BOOL TXBuffer)

	Description: This method will check the RX or TX buffer of the IO_BUFFER for
	the specified value.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to check for the data.

		BYTE Value - The value to find in the RX or TX buffer.

		BOOL TXBuffer - If TRUE the TX buffer will be searched, otherwise the RX
		buffer will be searched.

	Returns: 
		INT32 - The number of times Value was found inside the buffer.

	Notes:
        - USING_IO_BUFFER_CONTAINS_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.
  
		- This method will first read any data in the hardware buffers into the IO_BUFFER.  
		  This method will not search the users RX or TX buffer if a TASK is actively 
		  using the IO_BUFFER.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must
          be defined as a 1 to use the BOOL TXBuffer argument.

		- A TASK does not need to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- IOBufferContainsSequence(), IOBufferPeek()
*/
INT32 IOBufferContains(IO_BUFFER_ID IOBufferID, BYTE Value

                          #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                              , BOOL TXBuffer
                          #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                          );

/*
    OS_RESULT IOBufferContainsSequence(IO_BUFFER_ID IOBufferID, BYTE *Sequence, UINT32 SequenceSizeInBytes, BOOL TXBuffer)

	Description: This method will check the RX or TX buffer of the IO_BUFFER for
	the specified sequence.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to check for the sequence.

		BYTE *Sequence - The BYTE sequence to search for in the RX or TX buffer.

		BOOL TXBuffer - If TRUE the TX buffer will be searched, otherwise the RX
		buffer will be searched.

	Returns: 
		OS_RESULT - OS_SUCCESS if the sequence was found, an error code otherwise.

	Notes:
        - USING_IO_BUFFER_CONTAINS_SEQUENCE_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.
 
		- This method will first read any data in the hardware buffers into the IO_BUFFER.  
		  This method will not search the users RX or TX buffer if a TASK is actively 
		  using the IO_BUFFER.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must
          be defined as a 1 to use the BOOL TXBuffer argument.

		- A TASK does not need to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- IOBufferContains(), IOBufferPeek()
*/
OS_RESULT IOBufferContainsSequence(IO_BUFFER_ID IOBufferID, BYTE *Sequence, UINT32 SequenceSizeInBytes

                                  #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                      , BOOL TXBuffer
                                  #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                                  );

/*
    OS_RESULT IOBufferPeek(IO_BUFFER_ID IOBufferID, UINT32 NumberOfBytesToPeekAt, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesPeekedAt, BOOL TXBuffer)

	Description: This method will read the specified number of bytes from the IO_BUFFER
	and place them in the user buffer.  The data read from the IO_BUFFER remains in 
	the IO_BUFFER after this method completes.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to peek into.

		UINT32 NumberOfBytesToPeekAt - The number of bytes that will be peeked at in the
		IO_BUFFER.

		BYTE *UserBuffer - The user buffer to place the data peeked at from the IO_BUFFER.

		UINT32 UserBufferSizeInBytes - The size in bytes of the UserBuffer.

		UINT32 *BytesPeekedAt - This is an optional parameter.  If not (UINT32*)NULL then the
		number of bytes that were successfully peeked at and placed into UserBuffer are
		recorded in this variable.

		BOOL TXBuffer - If TRUE the TX buffer will be peeked at, otherwise the RX
		buffer will be peeked at.
		

	Returns: 
		OS_RESULT - OS_SUCCESS is returned if the IO_BUFFER was successfully peeked at.
		Otherwise an error code is returned.  If there is less than NumberOfBytesToPeekAt
		inside the IO_BUFFER, OS_SUCCESS is still returned if the IO_BUFFER was peeked at.

	Notes:
		- USING_IO_BUFFER_PEEK_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- This method will first read any data in the hardware buffers into the IO_BUFFER.  
		  This method will not search the users RX or TX buffer if a TASK is actively 
		  using the IO_BUFFER.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must
          be defined as a 1 to use the BOOL TXBuffer argument.
 
        - A TASK does not need to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- IOBufferContains(), IOBufferContainsSequence()
*/
OS_RESULT IOBufferPeek(IO_BUFFER_ID IOBufferID, UINT32 NumberOfBytesToPeekAt, BYTE *UserBuffer, UINT32 UserBufferSizeInBytes, UINT32 *BytesPeekedAt

                      #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                          , BOOL TXBuffer
                      #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                      );

/*
    OS_RESULT IOBufferFlush(IO_BUFFER_ID IOBufferID, BOOL TXBuffer)

	Description: This method will flush the RX or TX buffer inside of an IO_BUFFER.
	No hardware buffers are updated or flushed when this method is called.  Only
	the owning TASK of the IO_BUFFER can call this method.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to flush the RX or TX buffer of.

		BOOL TXBuffer - If TRUE the TX buffer will be flushed, otherwise the RX
		buffer will be flushed.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned on success, an error code otherwise.

	Notes:
		- USING_IO_BUFFER_FLUSH_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must
          be defined as a 1 to use the BOOL TXBuffer argument.

		- A TASK must own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- GetIOBuffer()
*/
OS_RESULT IOBufferFlush(IO_BUFFER_ID IOBufferID

                       #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                           , BOOL TXBuffer
                       #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                       );

/*
    UINT32 IOBufferGetSize(IO_BUFFER_ID IOBufferID, BOOL TXBuffer)

	Description: This method will return the number of bytes currently present inside the 
	RX or TX buffer of the IO_BUFFER.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to get the size of the RX or TX buffer.

		BOOL TXBuffer - If TRUE the TX buffer current size will be returned, otherwise 
		the RX buffer current size will be returned.

	Returns: 
		UINT32 - The number of bytes present in the requested RX or TX buffer
        of the specified IO_BUFFER.

	Notes:
		- USING_IO_BUFFER_GET_SIZE_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must
          be defined as a 1 to use the BOOL TXBuffer argument.

		- A TASK does not need to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- IOBufferGetRemainingBytes(), IOBufferGetCapacity()
*/
UINT32 IOBufferGetSize(IO_BUFFER_ID IOBufferID

                      #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                          , BOOL TXBuffer
                      #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                      );

/*
    UINT32 IOBufferGetRemainingBytes(IO_BUFFER_ID IOBufferID, BOOL TXBuffer)

	Description: This method will return the number of bytes currently not in use inside the 
	RX or TX buffer of the IO_BUFFER.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to get the remaining empty space in bytes of the RX or TX buffer.

		BOOL TXBuffer - If TRUE the TX buffer empty space in bytes will be returned, 
		otherwise the RX buffer empty space in bytes will be returned.

	Returns: 
		UINT32 - The number of empty bytes available in the requested RX or TX buffer
		of the specified IO_BUFFER.

	Notes:
		- USING_IO_BUFFER_GET_REMAINING_BYTES_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must
          be defined as a 1 to use the BOOL TXBuffer argument.

		- A TASK does not need to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- IOBufferGetSize(), IOBufferGetCapacity()
*/
UINT32 IOBufferGetRemainingBytes(IO_BUFFER_ID IOBufferID

                                #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                                    , BOOL TXBuffer
                                #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                                );

/*
    UINT32 IOBufferGetCapacity(IO_BUFFER_ID IOBufferID, BOOL TXBuffer)

	Description: This method will return the capacity of the specified RX or TX buffer
	inside of the IO_BUFFER.  This is the overall size of the buffer which represents 
	how many bytes are stored plus the number of empty bytes available in the buffer.
	The capacity of each buffer was passed to the method InitIOBuffer() at creation.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to get the capacity of in bytes of the RX or TX buffer.

		BOOL TXBuffer - If TRUE the TX buffer capacity in bytes will be returned, 
		otherwise the RX buffer capacity in bytes will be returned.

	Returns: 
		UINT32 - The capacity in bytes of the requested RX or TX buffer
		of the specified IO_BUFFER.

	Notes:
		- USING_IO_BUFFER_GET_CAPACITY_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- USING_IO_BUFFER_ASYNC_WRITE inside of RTOSConfig.h must
          be defined as a 1 to use the BOOL TXBuffer argument.

		- A TASK does not need to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- IOBufferGetSize(), IOBufferGetRemainingBytes(), InitIOBuffer()
*/
UINT32 IOBufferGetCapacity(IO_BUFFER_ID IOBufferID

                          #if (USING_IO_BUFFER_ASYNC_WRITE == 1)
                              , BOOL TXBuffer
                          #endif // end of #if (USING_IO_BUFFER_ASYNC_WRITE == 1)

                          );

/*
    BYTE *IOBufferGetNewLine(IO_BUFFER_ID IOBufferID)

	Description: This method will return the new line sequence which is used in the methods
	IOBufferReadLine() and IOBufferWriteLine().  By default when an IO_BUFFER is created the
	new line sequence is "\r\n" (no quotes), or expressed in hexadecimal 0x0D 0x0A.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to get the new line sequence of.

	Returns: 
		BYTE* - A pointer to the new line sequence on success, (BYTE*)NULL otherwise.  
        Do not attempt to modify the new line sequence by calling this method to get 
        a pointer to it.  To modify the new line sequence call IOBufferSetNewLine().

	Notes:
		- USING_IO_BUFFER_GET_NEW_LINE_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- USING_IO_BUFFER_READ_LINE_METHOD or USING_IO_BUFFER_WRITE_LINE_METHOD 
		  inside of RTOSConfig.h must be defined as a 1 to use this method.

		- A TASK does not need to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

		- The new line sequence is the same for RX and TX operations.

	See Also:
		- IOBufferSetNewLine(), IOBufferReadLine(), IOBufferWriteLine()
*/
BYTE *IOBufferGetNewLine(IO_BUFFER_ID IOBufferID);

/*
    OS_RESULT IOBufferSetNewLine(IO_BUFFER_ID IOBufferID, BYTE *NewLine, UINT32 NewLineSequenceSizeInBytes)

	Description: This method will update the new line sequence pointer inside the IO_BUFFER.  The IO_BUFFER
	will only point to the new line sequence and not make a copy of it.  Therefore the data must exist
	outside the scope of this method.

	Blocking: No

	User Callable: Yes

	Arguments:
		IO_BUFFER_ID IOBufferID - A valid IO_BUFFER_ID which represents the IO_BUFFER
        to update the new line sequence of.

		BYTE *NewLine - A pointer to the new line sequence.  The data in the new line
		does not have to be printable ASCII characters, it can contain any value.

		UINT32 NewLineSequenceSizeInBytes - The number of bytes large the data being
		pointed to by BYTE *NewLine is.

	Returns: 
		OS_RESULT - OS_SUCCESS is returned on success, an error code otherwise.

	Notes:
		- USING_IO_BUFFER_SET_NEW_LINE_METHOD inside of RTOSConfig.h must be 
          defined as a 1 to use this method.

		- USING_IO_BUFFER_READ_LINE_METHOD or USING_IO_BUFFER_WRITE_LINE_METHOD 
		  inside of RTOSConfig.h must be defined as a 1 to use this method.

		- A TASK needs to own the IO_BUFFER with a call to GetIOBuffer()
		  in order to call this method.

	See Also:
		- IOBufferGetNewLine(), IOBufferReadLine(), IOBufferWriteLine()
*/
OS_RESULT IOBufferSetNewLine(IO_BUFFER_ID IOBufferID, BYTE *NewLine, UINT32 NewLineSequenceSizeInBytes);

#endif // end of #ifndef IO_BUFFER_H