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
	PIPE Description:
		A PIPE can be used to write/read data to/from a fixed sized buffer
        in a controlled manner.  The producer consumer model for TASKs using
        a PIPE can be any of the following:
            - Single Producer, Single Consumer
            - Single Producer, Multiple Consumer
            - Multiple Producer, Single Consumer
            - Multiple Producer, Multiple Consumer
 
        The OS does not enforce any producer consumer relationship and it
        is entirely up to the user to do this.  PIPEs can also have starvation
        protection built into them.  This allows a lower priority TASK to write/read
        to/from a PIPE if higher priority TASKs keep preempting it.
*/

#ifndef PIPE_H
	#define PIPE_H

#include "../Kernel/Kernel.h"
#include "../../Generic Libraries/Generic Buffer/GenericBuffer.h"

#define INVALID_PIPE_SIZE                                   0xFFFFFFFF

typedef struct
{
	GENERIC_BUFFER GenericBuffer;
	DOUBLE_LINKED_LIST_HEAD PipeBlockedListHead;
    
    #if (USING_PIPE_DELETE_FROM_ISR_METHOD == 1)
        /*
         * When this is non zero that means there are TASK's using this
         * resource but are not blocked.  As such, the resource cannot
         * be deleted until this counter becomes zero.
         */
        UINT32 DeleteBlockCount;
    #endif // end of #if (USING_PIPE_DELETE_FROM_ISR_METHOD == 1)
}PIPE;

/*
	PIPE *CreatePipe(PIPE *Pipe, BYTE *Buffer, UINT32 CapacityInBytes)

	Description: This method creates a PIPE.  A PIPE can be used by 2 or more
    TASK's to communicate data.  The relationship between TASK's and a PIPE is not
    defined by the PIPE library.  That is to say the producer/consumer model used 
    can be one to one, many to one, one to many, or many to many.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE.  This is optional and can be (PIPE*)NULL.
        If (PIPE*)NULL is passed in, space for a PIPE will be allocated in the OS heap.
 
        BYTE *Buffer - A pointer to a valid BYTE buffer.  This is optional and can
        be (BYTE*)NULL.  If (BYTE*)NULL is passed in, space for the buffer will be allocated
        in the OS heap.  The size of the space allocated in the OS heap will be CapacityInBytes.
 
        UINT32 CapacityInBytes - The size of the buffer pointed to by BYTE *Buffer or to be created.
 
	Returns:
		PIPE * - A pointer to a valid PIPE if successful, (PIPE*)NULL upon failure.

	Notes:
		- None

	See Also:
		- PipeDelete()
*/
PIPE *CreatePipe(PIPE *Pipe, BYTE *Buffer, UINT32 CapacityInBytes);

/*
	OS_RESULT PipeWrite(PIPE *Pipe, BYTE *Data, UINT32 BytesToWrite, UINT32 *BytesWritten, INT32 TimeoutInTicks)

	Description: This method will attempt to write the specified data to the PIPE.  If there is not enough room
    in the PIPE to write the data, the TASK will fill up the PIPE with as much data as it can and then block.
    Then once another TASK performs a read from the PIPE the blocked TASK will resume and continue writing data
    until all data is written to the PIPE.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
        BYTE *Data - A pointer to the data which will be written to the PIPE.

        UINT32 BytesToWrite - The number of bytes Data points to.

        UINT32 *BytesWritten - The number of bytes successfully written to the PIPE will be stored in this variable.

		INT32 TimeoutInTicks - This is a timeout value in ticks to wait
		for the PIPE should it be owned by another TASK.  Below
		are valid values for TimeoutInTicks.

			TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
			Delayed Queue up to the specified number of ticks if
			the PIPE does not have enough space to write the data to.

			TimeoutInTicks = 0: The calling TASK will not be placed on the
			Delay Queue and this method will return OS_RESOURCE_GET_FAILED if
            the PIPE does not have enough space to write the data to.

			TimeoutInTicks <= -1: The calling TASK will not be placed on the
			Delay Queue and will wait forever if the PIPE is does not have 
            enough space to write the data to.
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
		- This method could potentially block the currently executing TASK if the PIPE does not have enough
          space to write the data to.
   
   		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must be defined as a 1 to use 
          the INT32 TimeoutInTicks argument.

	See Also:
		- PipeRead(), PipeFlush()
*/
OS_RESULT PipeWrite(PIPE *Pipe, BYTE *Data, UINT32 BytesToWrite, UINT32 *BytesWritten

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, INT32 TimeoutInTicks
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                );
/*
	OS_RESULT PipeRead(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead, INT32 TimeoutInTicks)

	Description: This method will attempt to read  data from the specified PIPE.  If there is not enough data
    in the PIPE to read, the TASK will read all data in the PIPE and then block.  Then once another TASK performs 
    a write to the PIPE the blocked TASK will resume and continue reading data until all requested data is read from 
    the PIPE.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().

        BYTE *Data - A pointer to a buffer where the data from the PIPE will be read to.

        UINT32 BufferSizeInBytes - The size of the buffer in bytes.

        UINT32 BytesToRead - The number of bytes to read from the PIPE.

        UINT32 *BytesRead - The number of actual bytes read from the PIPE is stored here.
 
		INT32 TimeoutInTicks - This is a timeout value in ticks to wait
		for the PIPE should it be owned by another TASK.  Below
		are valid values for TimeoutInTicks.

			TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
			Delayed Queue up to the specified number of ticks if
			the PIPE does not have enough bytes to read.

			TimeoutInTicks = 0: The calling TASK will not be placed on the
			Delay Queue and this method will return OS_RESOURCE_GET_FAILED if
            the PIPE does not have enough bytes to read.

			TimeoutInTicks <= -1: The calling TASK will not be placed on the
			Delay Queue and will wait forever if the PIPE is does not have 
            enough bytes to read.
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
 		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- PipeWrite(), PipePeek(), PipeFlush() 
*/
OS_RESULT PipeRead(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead	

                                #if (USING_TASK_DELAY_TICKS_METHOD == 1)
									, INT32 TimeoutInTicks
								#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                );

/*
	OS_RESULT PipeFlush(PIPE *Pipe)

	Description: This method will flush all data within the specified PIPE.  Any TASK blocking
    on the PIPE will resume execution.  If the PIPE was full any TASK waiting to write data to 
    it will be allowed to do so.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
        - USING_PIPE_FLUSH_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeRead()
*/
OS_RESULT PipeFlush(PIPE *Pipe);

/*
	OS_RESULT PipePeek(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead)

	Description: This method will read the specified data from the PIPE, but will not advance the internal read
    pointers or change the number of bytes written to the PIPE.  That is to say this method will return the data
    from the PIPE without modifying the PIPE.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
   
        BYTE *Data - A pointer to a buffer where the data from the PIPE will be read to.

        UINT32 BufferSizeInBytes - The size of the buffer in bytes.

        UINT32 BytesToRead - The number of bytes to read from the PIPE.

        UINT32 *BytesRead - The number of actual bytes read from the PIPE is stored here.
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
        - USING_PIPE_PEEK_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeRead()
*/
OS_RESULT PipePeek(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead);

/*
	UINT32 PipeContains(PIPE *Pipe, BYTE Value)

	Description: This method will search for a value within the PIPE and return
    how many times it came across the value.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
   
        BYTE Value - The value to search for inside of the PIPE.
 
	Returns:
		UINT32 - The number of times the Value was found inside of the PIPE.

	Notes:
        - USING_PIPE_CONTAINS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeContainsSequence(), PipeSeek()
*/
UINT32 PipeContains(PIPE *Pipe, BYTE Value);

/*
	OS_RESULT PipeContainsSequence(PIPE *Pipe, BYTE *Sequence, UINT32 SequenceSize)

	Description:  This method will search for a BYTE pattern within the PIPE.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
  
        BYTE *Sequence - The byte sequence to search for inside of the PIPE.
  
        UINT32 SequenceSize - The number of bytes large the Sequence is.
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
        - USING_PIPE_CONTAINS_SEQUENCE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeContains(), PipeSeek()
*/
OS_RESULT PipeContainsSequence(PIPE *Pipe, BYTE *Sequence, UINT32 SequenceSize);

/*
	UINT32 PipeSeek(PIPE *Pipe, BYTE Value)

	Description: This method will search for the specified value within a PIPE.
    It will then return how many bytes deep within the PIPE the value is.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
  
        BYTE Value - The BYTE value to look for in the PIPE.
 
	Returns:
		UINT32 - How many bytes deep the value is located in the PIPE, or 0 if not found.

	Notes:
        - USING_PIPE_SEEK_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeContainsSequence(), PipeContains()
*/
UINT32 PipeSeek(PIPE *Pipe, BYTE Value);

/*
	UINT32 PipeGetSize(PIPE *Pipe)

	Description: This method will return the number of bytes presently written to the PIPE.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		UINT32 - The number of bytes currently stored inside of the PIPE.

	Notes:
        - USING_PIPE_GET_SIZE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeGetCapacity(), PipeGetRemainingBytes()
*/
UINT32 PipeGetSize(PIPE *Pipe);

/*
	UINT32 PipeGetCapacity(PIPE *Pipe)

	Description: This method will return how many bytes the PIPE can hold.  Essentially
    this is the size of the buffer when the PIPE was created.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		UINT32 - The size of the buffer inside the PIPE in bytes.

	Notes:
        - USING_PIPE_GET_CAPACITY_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeGetSize(), PipeGetRemainingBytes()
*/
UINT32 PipeGetCapacity(PIPE *Pipe);

/*
	UINT32 PipeGetRemainingBytes(PIPE *Pipe)

	Description: This method will return how many bytes are left to write to the PIPE
    before it becomes full.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		UINT32 - The free space left in the PIPE in bytes.

	Notes:
        - USING_PIPE_GET_REMAINING_BYTES_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeGetCapacity(), PipeGetSize()
*/
UINT32 PipeGetRemainingBytes(PIPE *Pipe);

/*
	OS_RESULT PipeDelete(PIPE *Pipe, BOOL FreeBufferSpace)

	Description: This method will delete the specified PIPE and potentially the
    buffer space for the PIPE.

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
        BOOL FreeBufferSpace - If TRUE this method will attempt to free the buffer associated with
        the PIPE.
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
        - USING_PIPE_DELETE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- PipeCreate()
*/
OS_RESULT PipeDelete(PIPE *Pipe, BOOL FreeBufferSpace);

/*
	OS_RESULT PipeWriteFromISR(PIPE *Pipe, BYTE *Data, UINT32 BytesToWrite, UINT32 *BytesWritten, BOOL *HigherPriorityTask)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
		- None

	See Also:
		- PipeReadFromISR(), PipeFlushFromISR()
*/
OS_RESULT PipeWriteFromISR(PIPE *Pipe, BYTE *Data, UINT32 BytesToWrite, UINT32 *BytesWritten, BOOL *HigherPriorityTask);

/*
	OS_RESULT PipeReadFromISR(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead, BOOL *HigherPriorityTask)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
		- None

	See Also:
		- PipeWriteFromISR(), PipeFlushFromISR()
*/
OS_RESULT PipeReadFromISR(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead, BOOL *HigherPriorityTask);

/*
	OS_RESULT PipeFlushFromISR(PIPE *Pipe)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
		- None

	See Also:
		- PipeReadFromISR()
*/
OS_RESULT PipeFlushFromISR(PIPE *Pipe);

/*
	OS_RESULT PipePeekFromISR(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
		- None

	See Also:
		- PipeReadFromISR()
*/
OS_RESULT PipePeekFromISR(PIPE *Pipe, BYTE *Data, UINT32 BufferSizeInBytes, UINT32 BytesToRead, UINT32 *BytesRead);

/*
	UINT32 PipeContainsFromISR(PIPE *Pipe, BYTE Value)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
        UINT32 - 

	Notes:
		- None

	See Also:
		- PipeContainsSequenceFromISR()
*/
UINT32 PipeContainsFromISR(PIPE *Pipe, BYTE Value);

/*
	OS_RESULT PipeContainsSequenceFromISR(PIPE *Pipe, BYTE *Sequence, UINT32 SequenceSize)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
		- None

	See Also:
		- PipeContainsFromISR()
*/
OS_RESULT PipeContainsSequenceFromISR(PIPE *Pipe, BYTE *Sequence, UINT32 SequenceSize);

/*
	OS_RESULT PipeSeekFromISR(PIPE *Pipe, BYTE Value, UINT32 *BytesDeep)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		UINT32 - 

	Notes:
		- None

	See Also:
		- 
*/
UINT32 PipeSeekFromISR(PIPE *Pipe, BYTE Value);

/*
	UINT32 PipeGetSizeFromISR(PIPE *Pipe)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
        UINT32 - 

	Notes:
		- None

	See Also:
		- PipeGetCapacityFromISR(), PipeGetRemainingBytesFromISR()
*/
UINT32 PipeGetSizeFromISR(PIPE *Pipe);

/*
	UINT32 PipeGetCapacityFromISR(PIPE *Pipe)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		UINT32 - 

	Notes:
		- None

	See Also:
		- PipeGetSizeFromISR(), PipeGetRemainingBytesFromISR()
*/
UINT32 PipeGetCapacityFromISR(PIPE *Pipe);

/*
	UINT32 PipeGetRemainingBytesFromISR(PIPE *Pipe)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		UINT32 - 

	Notes:
		- None

	See Also:
		- PipeGetCapacityFromISR(), PipeGetSizeFromISR()
*/
UINT32 PipeGetRemainingBytesFromISR(PIPE *Pipe);

/*
	OS_RESULT PipeDeleteFromISR(PIPE *Pipe, BOOL FreeBufferSpace)

	Description: 

	Blocking: No

	User Callable: Yes

	Arguments:
		PIPE *Pipe - A pointer to a valid PIPE which was returned from CreatePipe().
 
	Returns:
		OS_RESULT - The value OS_SUCCESS if the operation was successful, another value if the operation failed.

	Notes:
		- None

	See Also:
		- CreatePipe()
*/
OS_RESULT PipeDeleteFromISR(PIPE *Pipe, BOOL FreeBufferSpace);

#endif // end of #ifndef PIPE_H
