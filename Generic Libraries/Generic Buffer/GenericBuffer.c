/*
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
	Developer: brodie
	Date: July 9, 2015
	File Name: GenericBuffer.c
	Version: 1.06
	IDE: Visual Studio 2010 Professional
	Compiler: C89

	Description:
	This file contains all the code which comprises 
	the GenericBuffer library.  For method descriptions see
	GenericBuffer.h.

	v1.06 Release Notes
			- Added in the method GenericBufferContainsSequence().  This looks for a sequence
			  of bytes located within the GENERIC_BUFFER.

	v1.05 Release Notes
			- Added in the method GenericBufferSeek() for searching how far in a specific
			  byte is inside a GENERIC_BUFFER.
			- Fixed the USE_MALLOC definition.  It now is more specific to this library and
			  is called GENERIC_BUFFER_USE_MALLOC.  This avoids conflicts with other libraries.

	v1.04 Release Notes
			- Added in a method for searching a GENERIC_BUFFER for a specific
			character.
			- Made the GenericBuffer Peek/Read methods have the option of NULL 
			terminating the destination buffer.
			- Changed all void* pointers to BYTE* pointers (made for easier indexing)
	
	v1.02 Release Notes
			- Small bug fixes

	v1.00 Release Notes
			- Original Release

*/

#include "GenericBufferConfig.h"
#include "GenericBuffer.h"

#if (GENERIC_BUFFER_USE_MALLOC == 1)
	#include "stdlib.h"
#endif // end of GENERIC_BUFFER_USE_MALLOC

static UINT32 BufferReadToUserBuffer(GENERIC_BUFFER *GenericBuffer, UINT32 BytesToRead, BYTE *UserBuffer)
{
	UINT32 BytesRead = (UINT32)0;
	BYTE *ReadPtr = (BYTE*)UserBuffer;

	/*
		While the Buffer still has some space in it to read from, and we haven't
		reached the number of bytes the user wanted us to read, go ahead and keep
		on reading.
	*/
	while(GenericBuffer->BufferSize && BytesRead != (UINT32)BytesToRead)
	{
		/*
			There is at least 1 byte to read, and we haven't read the number
			of bytes that the user wanted us to.
		*/
		BytesRead++;

		GenericBuffer->BufferSize--;

		/*
			Copy the next byte into the user buffer, and advance the read pointer and
			the pointer to the users buffer.
		*/
		*ReadPtr++ = *(GenericBuffer->CurrentReadPosition);
		
		GenericBuffer->CurrentReadPosition++;

		/*
			If we're 1 byte past the end of the Buffer, then go back to the beginning.
		*/
		if((UINT32)(GenericBuffer->CurrentReadPosition) == ((UINT32)(GenericBuffer->Buffer) + (UINT32)(GenericBuffer->BufferCapacity)))
			GenericBuffer->CurrentReadPosition = (BYTE*)(GenericBuffer->Buffer);
	}

	/*
		Return the number of bytes read.  This may not be the requested amount, but return
		it anways.
	*/

	return (UINT32)BytesRead;
}

GENERIC_BUFFER *CreateGenericBuffer(GENERIC_BUFFER *GenericBuffer, UINT32 CapacityInBytes, BYTE *Buffer)
{
	void *TempBuffer;

	#if (GENERIC_BUFFER_SAFE_MODE == 1)
		if(CapacityInBytes < (UINT32)3)
			return (GENERIC_BUFFER*)NULL;
	#endif // end of GENERIC_BUFFER_SAFE_MODE

	/*
		If the buffer pointer that the user passed in is equal to (void*)NULL, then allocate
		some space out of the heap that is CapacityInBytes bytes big.
	*/
	if(Buffer == (BYTE*)NULL)
	{
		if((TempBuffer = (void*)BufferMemAlloc(CapacityInBytes)) == (BYTE*)NULL)
		{
			return (GENERIC_BUFFER*)NULL;
		}
	}

	/*
		If the user passed in an empty GENERIC_BUFFER poitner then we need to allocate
		one in the heap.
	*/
	if(GenericBufferIsNull(GenericBuffer))
	{
		if((GenericBuffer = (GENERIC_BUFFER*)BufferMemAlloc(sizeof(GENERIC_BUFFER))) == (GENERIC_BUFFER*)NULL)
		{
			if(Buffer == (void*)NULL)
			{
				BufferMemDealloc((void*)TempBuffer);
			}

			return (GENERIC_BUFFER*)NULL;
		}
	}

	/*
		We needed to hold off assigning anything to (void*)Buffer incase the GENERIC_BUFFER 
		BufferBufferMemAlloc() attempt failed.  Check to see if the user passed a buffer in, if not then
		we must have allocated one and it is located at (void*)TempBuffer.
	*/
	if(Buffer)
		GenericBuffer->Buffer = (BYTE*)Buffer;
	else
		GenericBuffer->Buffer = (BYTE*)TempBuffer;

	/*
		Initialize the new GENERIC_BUFFER with it's defualt values.
	*/
	GenericBuffer->BufferCapacity = (UINT32)CapacityInBytes;
	GenericBuffer->BufferSize = (UINT32)0;
	GenericBuffer->CurrentReadPosition = GenericBuffer->CurrentWritePosition = (BYTE*)(GenericBuffer->Buffer);

	return (GENERIC_BUFFER*)GenericBuffer;
}

UINT32 GenericBufferWrite(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, const BYTE *DataToWrite)
{
	UINT32 BytesWritten;
	BYTE *DataPtr;

	#if (GENERIC_BUFFER_SAFE_MODE == 1)
		if(GenericBufferIsNull(GenericBuffer))
			return (UINT32)0;

		if(NumberOfBytes == 0)
			return (UINT32)0;

		if(DataToWrite == (BYTE*)NULL)
			return (UINT32)0;
	#endif // end of GENERIC_BUFFER_SAFE_MODE

	BytesWritten = (UINT32)0;
	DataPtr = (BYTE*)DataToWrite;

	/*
		While the Buffer still has some space in it to write to, and we haven't
		reached the number of bytes the user wanted us to write, go ahead and keep
		on writing.
	*/
	while(GenericBuffer->BufferSize < (UINT32)(GenericBuffer->BufferCapacity) && BytesWritten != (UINT32)NumberOfBytes)
	{
		BytesWritten++;
		GenericBuffer->BufferSize++;

		/*
			Copy the next byte into the buffer, and advance the read pointer and
			the pointer to the users buffer.
		*/
		*(GenericBuffer->CurrentWritePosition) =  *DataPtr++;
		GenericBuffer->CurrentWritePosition++;

		/*
			If we're 1 byte past the end of the Buffer, then go back to the beginning.
		*/
		if((UINT32)(GenericBuffer->CurrentWritePosition) == ((UINT32)(GenericBuffer->Buffer) + (UINT32)(GenericBuffer->BufferCapacity)))
			GenericBuffer->CurrentWritePosition = (BYTE*)(GenericBuffer->Buffer);
	}

	return (UINT32)BytesWritten;
}

UINT32 GenericBufferRead(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, BOOL NullTerminate)
{
	UINT32 BytesRead;

	#if (GENERIC_BUFFER_SAFE_MODE == 1)
		if(GenericBufferIsNull(GenericBuffer))
			return (UINT32)0;

		if(GenericBufferIsEmpty(GenericBuffer))
			return (UINT32)0;

		if(DestinationBuffer == (BYTE*)NULL)
			return (UINT32)0;

		if (DestinationBufferSize == 0 || NumberOfBytes > DestinationBufferSize)
			return (UINT32)0;

		if (NullTerminate == TRUE)
		{
			if (NumberOfBytes + 1 > DestinationBufferSize)
				return (UINT32)0;
		}
	#endif // end of GENERIC_BUFFER_SAFE_MODE


	BytesRead = (UINT32)BufferReadToUserBuffer(GenericBuffer, NumberOfBytes, DestinationBuffer);

	if (NullTerminate == TRUE)
		DestinationBuffer[BytesRead] = '\0';

	return (UINT32)BytesRead;
}

#if (USING_GENERIC_BUFFER_PEEK_METHOD == 1)
	UINT32 GenericBufferPeek(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, BOOL NullTerminate)
	{
		UINT32 BytesRead;
		BYTE *ReadPointerMarker;
		UINT32 BuffersSize;

		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if(GenericBufferIsNull(GenericBuffer))
				return (UINT32)0;

			if(GenericBufferIsEmpty(GenericBuffer))
				return (UINT32)0;

			if(DestinationBuffer == (BYTE*)NULL)
				return (UINT32)0;

			if(DestinationBufferSize == 0 || NumberOfBytes > DestinationBufferSize)
				return (UINT32)0;

			if (NullTerminate == TRUE)
			{
				if (NumberOfBytes + 1 > DestinationBufferSize)
					return (UINT32)0;
			}
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		/*
			Copy the Buffer size and read pointers so we can set them
			back to where they were before the read.
		*/
		ReadPointerMarker = (BYTE*)(GenericBuffer->CurrentReadPosition);
		BuffersSize = (UINT32)(GenericBuffer->BufferSize);

		/*
			Read in the data to the user buffer.
		*/
		BytesRead = (UINT32)BufferReadToUserBuffer(GenericBuffer, NumberOfBytes, DestinationBuffer);

		/*
			Since the above method changed the buffers size and read pointer accordingly,
			we must put them back to where they were before the read.
		*/
		GenericBuffer->BufferSize = (UINT32)BuffersSize;
		GenericBuffer->CurrentReadPosition = (BYTE*)ReadPointerMarker;

		if (NullTerminate == TRUE)
			DestinationBuffer[BytesRead] = '\0';

		return (UINT32)BytesRead;
	}
#endif // end of USING_GENERIC_BUFFER_PEEK_METHOD

#if (USING_GENERIC_BUFFER_CONTAINS_METHOD == 1)
	UINT32 GenericBufferContains(GENERIC_BUFFER *GenericBuffer, BYTE Character)
	{
		BYTE *TempReadPtr;
		UINT32 i, ReturnValue = 0;

		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if (GenericBufferIsNull(GenericBuffer))
				return (BOOL)FALSE;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		TempReadPtr = GenericBuffer->CurrentReadPosition;

		for(i = 0; i < GenericBuffer->BufferSize; i++)
		{
			if (*TempReadPtr++ == Character)
				ReturnValue++;

			/*
			If we're 1 byte past the end of the Buffer, then go back to the beginning.
			*/
			if ((UINT32)TempReadPtr == ((UINT32)(GenericBuffer->Buffer) + (UINT32)(GenericBuffer->BufferCapacity)))
				TempReadPtr = (BYTE*)(GenericBuffer->Buffer);
		}

		return (UINT32)ReturnValue;
	}
#endif // end of USING_GENERIC_BUFFER_CONTAINS_METHOD

#if (USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD == 1)
	static BOOL CheckForSequence(GENERIC_BUFFER *GenericBuffer, BYTE *BufferPosition, BYTE *Sequence, UINT32 SequenceSize)
	{
		if (*BufferPosition++ == *Sequence++)
		{
			UINT32 i;

			// we have at least the starting character
			for (i = 0; i < SequenceSize - 1; i++)
			{
				/*
				If we're 1 byte past the end of the Buffer, then go back to the beginning.
				*/
				if ((UINT32)BufferPosition == ((UINT32)(GenericBuffer->Buffer) + (UINT32)(GenericBuffer->BufferCapacity)))
					BufferPosition = (BYTE*)(GenericBuffer->Buffer);

				// check to see if they're the same
				if (*BufferPosition++ != *Sequence++)
				{
					// they're enot equal, sequence not found
					return FALSE;
				}
			}

			// if we got out here then we found a winner
			return TRUE;
		}

		// the starting bytes didn't even match
		return FALSE;
	}

	BOOL GenericBufferContainsSequence(GENERIC_BUFFER *GenericBuffer, BYTE *Sequence, UINT32 SequenceSize)
	{
		UINT32 i;
		BYTE *BufferIndexer;

		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if (GenericBufferIsNull(GenericBuffer))
				return (BOOL)FALSE;

			if (Sequence == (BYTE*)NULL)
				return (BOOL)FALSE;

			if (SequenceSize == 0)
				return (BOOL)FALSE;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		// if there are more bytes in the sequence then there are bytes
		// available in the buffer, it is impossible to find the sequence
		if (SequenceSize > GenericBuffer->BufferSize)
			return (BOOL)FALSE;

		// we will start trying to find the sequence from the read position
		BufferIndexer = GenericBuffer->CurrentReadPosition;

		for (i = 0; i <= GenericBuffer->BufferSize - SequenceSize; i++)
		{
			if (CheckForSequence(GenericBuffer, BufferIndexer, Sequence, SequenceSize) == TRUE)
				return TRUE;

			// increment our starting search position inside the buffer
			BufferIndexer++;

			/*
			If we're 1 byte past the end of the Buffer, then go back to the beginning.
			*/
			if ((UINT32)BufferIndexer == ((UINT32)(GenericBuffer->Buffer) + (UINT32)(GenericBuffer->BufferCapacity)))
				BufferIndexer = (BYTE*)(GenericBuffer->Buffer);
		}

		return FALSE;
	}
#endif // end of USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD

#if (USING_GENERIC_BUFFER_SEEK_METHOD == 1)
	UINT32 GenericBufferSeek(GENERIC_BUFFER *GenericBuffer, BYTE Character)
	{
		BYTE *TempReadPtr;
		UINT32 i, ReturnValue = 1;

		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if (GenericBufferIsNull(GenericBuffer))
				return (BOOL)FALSE;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		TempReadPtr = GenericBuffer->CurrentReadPosition;

		for (i = 0; i < GenericBuffer->BufferSize; i++)
		{
			if (*TempReadPtr++ == Character)
				return ReturnValue;

			ReturnValue++;

			/*
			If we're 1 byte past the end of the Buffer, then go back to the beginning.
			*/
			if ((UINT32)TempReadPtr == ((UINT32)(GenericBuffer->Buffer) + (UINT32)(GenericBuffer->BufferCapacity)))
				TempReadPtr = (BYTE*)(GenericBuffer->Buffer);
		}

		return (UINT32)0;
	}
#endif // end of USING_GENERIC_BUFFER_SEEK_METHOD

#if (USING_GENERIC_BUFFER_FLUSH_METHOD == 1)
	BOOL GenericBufferFlush(GENERIC_BUFFER *GenericBuffer)
	{
		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if(GenericBufferIsNull(GenericBuffer))
				return (BOOL)FALSE;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		/*
			Just reset the read and write pointers to the beginning of the buffer
			and set the size to zero.  Technically any data in the buffer is still
			there, it's just that the size is now 0 so the GENIER_BUFFER library
			will think it shouldn't access it.
		*/
		GenericBuffer->CurrentReadPosition = GenericBuffer->CurrentWritePosition = (BYTE*)(GenericBuffer->Buffer);
		GenericBuffer->BufferSize = (UINT32)0;

		return (BOOL)TRUE;
	}
#endif // end of USING_GENERIC_BUFFER_FLUSH_METHOD

#if (USING_GENERIC_BUFFER_GET_SIZE_METHOD == 1)
	UINT32 GenericBufferGetSize(GENERIC_BUFFER *GenericBuffer)
	{
		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if(GenericBufferIsNull(GenericBuffer))
				return (UINT32)0;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		/*
			Just return the current size of the GENERIC_BUFFER, this is 
			how much space is left to write to inside the buffer.
		*/
		return (UINT32)(GenericBuffer->BufferSize);
	}
#endif // end of USING_GENERIC_BUFFER_GET_SIZE_METHOD

#if (USING_GENERIC_BUFFER_GET_CAPACITY_METHOD == 1)
	UINT32 GenericBufferGetCapacity(GENERIC_BUFFER *GenericBuffer)
	{
		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if(GenericBufferIsNull(GenericBuffer))
				return (UINT32)0;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		/*
			Return just the capacity of the buffer.  This value is
			how many bytes can be held inside the GENERIC_BUFFER.
		*/
		return (UINT32)(GenericBuffer->BufferCapacity);
	}
#endif // end of USING_GENERIC_BUFFER_GET_CAPACITY_METHOD

#if (USING_GENERIC_BUFFER_DELETE_METHOD == 1)
	BOOL GenericBufferDelete(GENERIC_BUFFER *GenericBuffer, BOOL FreeBufferSpace)
	{
		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if(GenericBufferIsNull(GenericBuffer))
				return (BOOL)FALSE;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		/*
			If the user wants us to delete the buffer, then this means
			it was allocated by CreateGenericBuffer() when the GENERIC_BUFFER
			was created.
		*/
		if(FreeBufferSpace)
			BufferMemDealloc((void*)(GenericBuffer->Buffer));

		/*
			Finally delete the GENERIC_BUFFER in RAM.
		*/
		BufferMemDealloc((void*)(GenericBuffer));

		return (BOOL)TRUE;
	}
#endif // end of USING_GENERIC_BUFFER_DELETE_METHOD

#if (USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD == 1)
	UINT32 GenericBufferGetRemainingBytes(GENERIC_BUFFER *GenericBuffer)
	{
		#if (GENERIC_BUFFER_SAFE_MODE == 1)
			if(GenericBufferIsNull(GenericBuffer))
				return (BOOL)FALSE;
		#endif // end of GENERIC_BUFFER_SAFE_MODE

		/*
			Return the size left inside the GENERIC_BUFFER.  This is the capacity 
			minus the current size.
		*/
		return (UINT32)((UINT32)(GenericBuffer->BufferCapacity) - (UINT32)(GenericBuffer->BufferSize));
	}
#endif // end of USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD

#if (USING_GENERIC_BUFFER_GET_LIBRARY_VERSION == 1)

	const BYTE GenericBufferLibraryVersion[] = {"Generic Buffer Lib v1.06\0"};

	const BYTE *GenericBufferGetLibraryVersion(void)
	{
		return (const BYTE*)GenericBufferLibraryVersion;
	}
#endif // end of USING_GENERIC_BUFFER_GET_LIBRARY_VERSION
