/*
    NexOS Kernel Version v1.02.01
    Copyright (c) 2023 brodie

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

#include <stdlib.h>
#include <string.h>

#include "CriticalSection.h"
#include "Memory.h"
#include "Kernel.h"
#include "OS_Callback.h"

#if (USING_MEMORY_WARNING_CLEAR_EVENT == 1 || USING_MEMORY_WARNING_EVENT == 1)
	#include "../Event/Event.h"
#endif // end of 

#if (OS_HEAP_SIZE_IN_BYTES < (OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES + OS_WORD_SIZE_IN_BYTES))
	#error "OS_HEAP_SIZE_IN_BYTES must be defined and be larger than 8 or 12 bytes!"
#endif // end of OS_HEAP_SIZE_IN_BYTES < sizeof(OS_MEMORY_BLOCK_HEADER) + sizeof(WORD)

#if (OS_HEAP_SIZE_IN_BYTES % OS_WORD_SIZE_IN_BYTES)
	#error "OS_HEAP_SIZE_IN_BYTES must be word aligned!"
#endif // end of OS_HEAP_SIZE_IN_BYTES % sizeof(WORD)

#if (OS_HEAP_SIZE_IN_BYTES > 2147483648)
	#error "OS_HEAP_SIZE_IN_BYTES too big.  It cannot exceed 2,147,483,648!"
#endif // end of OS_HEAP_SIZE_IN_BYTES % sizeof(WORD)

#if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1)
    #if (MEMORY_WARNING_LEVEL_IN_BYTES >= OS_HEAP_SIZE_IN_BYTES)
        #error "MEMORY_WARNING_LEVEL_IN_BYTES too big.  It cannot exceed OS_HEAP_SIZE_IN_BYTES - 1!"
    #endif // end of #if (MEMORY_WARNING_LEVEL_IN_BYTES > = OS_HEAP_SIZE_IN_BYTES)
#endif // end of #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1)

#if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1)
	BOOL gMemoryWarning;
#endif // end of USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1

#if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
    static UINT32 gHeapUsedInBytes;
#endif // end of #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
    
OS_WORD gKernelManagedMemory[((OS_HEAP_SIZE_IN_BYTES + OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES) / OS_WORD_SIZE_IN_BYTES)];

#if (MEMORY_TEST == 1)
    #include <stdio.h>

	BOOL NextMemoryBlockValid(OS_MEMORY_BLOCK_HEADER *MemoryBlock)
	{
		OS_MEMORY_BLOCK_HEADER *MemoryBlockIterator = MemoryBlock;

		MemoryBlockIterator += ((MemoryBlockIterator->SizeInWords * 4) / OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES + 1);

		if(MemoryBlockIterator >= (OS_MEMORY_BLOCK_HEADER*)&(gKernelManagedMemory[(OS_HEAP_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES)]))
			return FALSE;

		return TRUE;
	}

	OS_MEMORY_BLOCK_HEADER *GetNextMemoryBlock(OS_MEMORY_BLOCK_HEADER *CurrentMemoryBlock)
	{
		CurrentMemoryBlock += ((CurrentMemoryBlock->SizeInWords * 4) / OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES + 1);

		return CurrentMemoryBlock;
	}

	void PrintMemoryBlock(OS_MEMORY_BLOCK_HEADER *MemoryBlock)
	{
		if(MemoryBlock->Allocated == 1)
			printf("ALLOCATED: YES\n\0" );
		else
			printf("ALLOCATED: NO\n\0" );

		printf("SPACE: %i bytes\n\n\0", MemoryBlock->SizeInWords * 4);
	}

	void PrintWalkThroughOSMemory(void)
	{
		OS_MEMORY_BLOCK_HEADER *MemoryBlock = (OS_MEMORY_BLOCK_HEADER*)gKernelManagedMemory;

		printf("\nOS HEAP WALKTHROUGH:\n\n\0" );

		PrintMemoryBlock(MemoryBlock);

		while(NextMemoryBlockValid(MemoryBlock) == TRUE)
		{
			MemoryBlock = GetNextMemoryBlock(MemoryBlock);

			PrintMemoryBlock(MemoryBlock);
		}

		printf("OS HEAP USED: %i bytes\n\0", gHeapUsedInBytes);
	}
#endif // end of #if (MEMORY_TEST == 1)
    
static OS_MEMORY_BLOCK_HEADER *OS_IterateMemoryBlockHeader(OS_MEMORY_BLOCK_HEADER *MemoryBlock)
{
	// Iterate to the next memory block
	MemoryBlock += ((MemoryBlock->SizeInWords * 4) / OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES + 1);

	// check to see if we have reached the end of the heap, if so return (void*)NULL
	if (MemoryBlock >= (OS_MEMORY_BLOCK_HEADER*)&(gKernelManagedMemory[(OS_HEAP_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES)]))
	{
		return (OS_MEMORY_BLOCK_HEADER*)NULL;
	}

	return MemoryBlock;
}

BOOL OS_InitializeHeap(void)
{
	OS_MEMORY_BLOCK_HEADER *MemoryBlockIterator = (OS_MEMORY_BLOCK_HEADER*)gKernelManagedMemory;

	// make the first block not allocated
	MemoryBlockIterator->Allocated = 0;

	MemoryBlockIterator->SizeInWords = (OS_HEAP_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES);

	#if(CLEAR_HEAP_AT_START_UP == 1)
		memset(&gKernelManagedMemory[1], HEAP_INITIALIZATION_VALUE, (OS_HEAP_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES));
	#endif // end of #if(CLEAR_HEAP_AT_START_UP == 1)

    #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1)
        gMemoryWarning = FALSE;
    #endif // end of USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1

    #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
        gHeapUsedInBytes = 0;
    #endif // end of #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)

	return TRUE;
}

void *OS_AllocateMemory(UINT32 SizeInBytes)
{
	UINT32 RequestedSizeInWords;
	void *StartingMemoryAddress;
	OS_MEMORY_BLOCK_HEADER *MemoryBlockIterator;

	if (SizeInBytes == 0)
		return (void*)NULL;

	StartingMemoryAddress = (void*)NULL;
	MemoryBlockIterator = (OS_MEMORY_BLOCK_HEADER*)gKernelManagedMemory;

	// the request has to be at least OS_WORD_SIZE_IN_BYTES large
	if (SizeInBytes < OS_WORD_SIZE_IN_BYTES)
		SizeInBytes = OS_WORD_SIZE_IN_BYTES;

	// make it evenly divisible by OS_WORD_SIZE_IN_BYTES
	if ((SizeInBytes % OS_WORD_SIZE_IN_BYTES) != 0)
		SizeInBytes += (OS_WORD_SIZE_IN_BYTES - (SizeInBytes % OS_WORD_SIZE_IN_BYTES));

	// first normalize the request to words, and make it a value thats word aligned
	RequestedSizeInWords = SizeInBytes / OS_WORD_SIZE_IN_BYTES;

	while (1)
	{
		// While the current memory block is allocated keep searching
		while (MemoryBlockIterator->Allocated)
		{
			// check to see if we have reached the end of the heap, if so return (void*)NULL
			if ((MemoryBlockIterator = OS_IterateMemoryBlockHeader(MemoryBlockIterator)) == (OS_MEMORY_BLOCK_HEADER*)NULL)
			{
				return (void*)NULL;
			}
		}

		// lets see if we found a winner, if so then lets break out of this loop
		if (MemoryBlockIterator->SizeInWords >= RequestedSizeInWords)
			break;

		// check to see if we have reached the end of the heap, if so return (void*)NULL
		if ((MemoryBlockIterator = OS_IterateMemoryBlockHeader(MemoryBlockIterator)) == (OS_MEMORY_BLOCK_HEADER*)NULL)
		{
			return (void*)NULL;
		}
	}

	MemoryBlockIterator->Allocated = TRUE;

	// Now check to see if there is enough room in this memory block to make another one, otherwise just add on the
	// space to the current memory block

	if (MemoryBlockIterator->SizeInWords >= (RequestedSizeInWords + ((OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES) + 1)))
	{
		// There's enough room in the current memory block to allocate at least the space requested another OS_MEMORY_BLOCK_HEADER
		// and 1 word
		// Just need to save this for the time being
		UINT32 Space = MemoryBlockIterator->SizeInWords;
		MemoryBlockIterator->SizeInWords = RequestedSizeInWords;

		// Iterate to the open space that will be allocated
		MemoryBlockIterator++;
		StartingMemoryAddress = (void*)MemoryBlockIterator;

		// Iterate the MemoryBlockIterator to the end of the newly allocated space
		MemoryBlockIterator += (SizeInBytes / OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES);
		MemoryBlockIterator->Allocated = FALSE;
		MemoryBlockIterator->SizeInWords = (Space - RequestedSizeInWords - (OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES));
	}
	else
	{
		MemoryBlockIterator++;
		StartingMemoryAddress = (void*)(MemoryBlockIterator);
	}

    #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
        gHeapUsedInBytes += SizeInBytes + sizeof(OS_MEMORY_BLOCK_HEADER);
    #endif // end of #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
    
	#if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1)
		if (gMemoryWarning == FALSE)
		{
			if (gHeapUsedInBytes > MEMORY_WARNING_LEVEL_IN_BYTES)
			{
				gMemoryWarning = TRUE;

				#if (USING_MEMORY_WARNING_USER_CALLBACK == 1)
					MemoryWarningUserCallback(gHeapUsedInBytes);
				#endif // end of USING_MEMORY_WARNING_USER_CALLBACK

				#if (USING_MEMORY_WARNING_EVENT == 1)
					OS_RaiseEvent(MEMORY_WARNING_EVENT);
				#endif // end of USING_MEMORY_WARNING_EVENT == 1
			}
		}
	#endif // end of USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1

	return (void*)StartingMemoryAddress;
}

BOOL OS_ReleaseMemory(void *Ptr)
{
	OS_MEMORY_BLOCK_HEADER *MemoryBlockIterator;
	OS_MEMORY_BLOCK_HEADER *MemoryBlockIterator2;

    // is the address even in the OS Heap?  If not we can't delete it.
	if (AddressInHeap((OS_WORD)Ptr) == FALSE)
		return FALSE;

	MemoryBlockIterator = (OS_MEMORY_BLOCK_HEADER*)Ptr;

	// Back up to where the memory block resides in memory
	MemoryBlockIterator--;

	MemoryBlockIterator2 = MemoryBlockIterator;

    #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
        // Record the amount of bytes that were in block and subtract them, we may be able to get rid of the header later on
        gHeapUsedInBytes -= (MemoryBlockIterator->SizeInWords * 4);
    #endif // end of #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)

	// Unallocate the memory block
	MemoryBlockIterator->Allocated = FALSE;

	// jump to the potential next memory block
	MemoryBlockIterator += ((MemoryBlockIterator->SizeInWords * 4) / OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES) + 1;

	// We must check to see if the block after us is allocated, lets see if we jump out of bounds
	if (MemoryBlockIterator < (OS_MEMORY_BLOCK_HEADER*)&(gKernelManagedMemory[(OS_HEAP_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES)]))
	{
		// If we're in here we haven't jumped out of bounds, and the memory block is valid.  Lets see if its allocated
		// if so then we're going to merge it.
		if (!MemoryBlockIterator->Allocated)
		{
            #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
                // subtract the size of one header from the heap used since we're merging 2 blocks
                gHeapUsedInBytes -= sizeof(OS_MEMORY_BLOCK_HEADER);
            #endif // end of #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)

			// We can merge the 2, this ones unallocated
			MemoryBlockIterator2->SizeInWords += MemoryBlockIterator->SizeInWords + (OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES);
		}
	}

	// Lets make sure the memory block wasn't the first one, cause if it was theres nothing before it!
	if (MemoryBlockIterator2 != (OS_MEMORY_BLOCK_HEADER*)gKernelManagedMemory)
	{
		OS_WORD PreviousJumpInWords;

		// Now lets see if the memory block before the first one is allocated or not.
		MemoryBlockIterator = (OS_MEMORY_BLOCK_HEADER*)gKernelManagedMemory;

		do
		{
			PreviousJumpInWords = MemoryBlockIterator->SizeInWords;

			MemoryBlockIterator += ((MemoryBlockIterator->SizeInWords * 4) / OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES + 1);
		} 
		while (MemoryBlockIterator != MemoryBlockIterator2);

		// Now that we're at where we we're allocated, we have to go back by one memory block
		MemoryBlockIterator -= ((PreviousJumpInWords * 4) / OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES + 1);

		if (!MemoryBlockIterator->Allocated)
		{
            #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
                // subtract the size of one header from the heap used since we're merging 2 blocks
                gHeapUsedInBytes -= sizeof(OS_MEMORY_BLOCK_HEADER);
            #endif // end of #if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1 || USING_GET_HEAP_USED_IN_BYTES_METHOD == 1 || USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)

			// This memory block isn't allocated, lets merge them
			MemoryBlockIterator->SizeInWords += (MemoryBlockIterator2->SizeInWords + OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES);
		}
	}

	#if (USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1)
		if (gMemoryWarning == TRUE)
		{
			if (gHeapUsedInBytes < MEMORY_WARNING_LEVEL_IN_BYTES)
			{
				gMemoryWarning = FALSE;

				#if (USING_MEMORY_WARNING_CLEAR_USER_CALLBACK == 1)
					MemoryWarningClearUserCallback();
				#endif // end of USING_MEMORY_WARNING_CLEAR_USER_CALLBACK

				#if (USING_MEMORY_WARNING_CLEAR_EVENT == 1)
					OS_RaiseEvent(MEMORY_WARNING_CLEAR_EVENT);
				#endif // end of USING_MEMORY_WARNING_CLEAR_EVENT == 1
			}
		}
	#endif // end of USING_MEMORY_WARNING_EVENT == 1 || USING_MEMORY_WARNING_USER_CALLBACK == 1

	return TRUE;
}

void *AllocateMemory(UINT32 SizeInBytes)
{
	void *AllocatedMemory;

	EnterCritical();

	AllocatedMemory = OS_AllocateMemory(SizeInBytes);

	ExitCritical();

	return AllocatedMemory;
}

BOOL ReleaseMemory(void *Ptr)
{
	BOOL ReleaseSuccessful;

	EnterCritical();

	ReleaseSuccessful = OS_ReleaseMemory(Ptr);

	ExitCritical();

	return ReleaseSuccessful;
}

#if (USING_GET_HEAP_USED_IN_BYTES_METHOD == 1)
    UINT32 GetHeapUsedInBytes(void)
    {
        UINT32 TempHeapUsed;

        EnterCritical();

        TempHeapUsed = gHeapUsedInBytes;

        ExitCritical();

        return TempHeapUsed;
    }
#endif // end of #if (USING_GET_HEAP_USED_IN_BYTES_METHOD == 1)

#if (USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
    UINT32 GetHeapRemainingInBytes(void)
    {
        UINT32 TempHeapRemaining;

        EnterCritical();

        TempHeapRemaining = OS_HEAP_SIZE_IN_BYTES - gHeapUsedInBytes;

        ExitCritical();

        return TempHeapRemaining;
    }
#endif // end of #if (USING_GET_HEAP_REMAINING_IN_BYTES_METHOD == 1)
    

#if (USING_CALLOC_MEMORY_METHOD == 1)
	/*
		This will clear all the memory that the user requests
	*/
	void *CallocMemory(UINT32 NumberOfItems, UINT32 SizeInBytes)
	{
		UINT32 RequestedSizeInWords;
		void *Data;

		if(SizeInBytes == 0 || NumberOfItems == 0)
			return (void*)NULL;
        
        SizeInBytes *= NumberOfItems;

		if (SizeInBytes < OS_WORD_SIZE_IN_BYTES)
			SizeInBytes = OS_WORD_SIZE_IN_BYTES;
	
		RequestedSizeInWords = ((SizeInBytes + (SizeInBytes % OS_WORD_SIZE_IN_BYTES)) / OS_WORD_SIZE_IN_BYTES);
	
		Data = AllocateMemory(SizeInBytes);
	
		if(Data != (void*)NULL)
		{
			UINT32 i = 0;
	
			OS_WORD *Ptr = (OS_WORD*)Data;
	
			for(i = 0; i < RequestedSizeInWords; i++)
				*Ptr++ = 0;
		}
	
		return (void*)Data;
	}
#endif // end of USING_CALLOC_MEMORY_METHOD

#if (USING_REALLOC_MEMORY_METHOD == 1)
    void *ReallocMemory(void *Ptr, UINT32 SizeInBytes)
	{
		void *Data;

		if (Ptr != (void*)NULL)
			if (AddressInHeap((OS_WORD)Ptr) == FALSE)
				return (void*)NULL;

		if (SizeInBytes == 0)
			return (void*)NULL;

		Data = AllocateMemory(SizeInBytes);

		if (Data == (void*)NULL)
			return (void*)NULL;

		// if they passed in a valid pointer to the heap,
		// just go ahead and copy the data over and then
		// free the previous data.
		if (Ptr != (void*)NULL)
		{
			memcpy(Data, Ptr, SizeInBytes);

			ReleaseMemory(Ptr);
		}
	
		return Data;
	}
#endif // end of USING_REALLOC_MEMORY_METHOD

BOOL AddressInHeap(OS_WORD Address)
{
	return (BOOL)(Address >= (OS_WORD)gKernelManagedMemory && Address <= ((OS_WORD)gKernelManagedMemory + sizeof(gKernelManagedMemory)));
}

BOOL ProgramAddressValid(OS_WORD ProgramAddress)
{
	if(ProgramAddress < USER_PROGRAM_STARTING_ADDRESS)
		return FALSE;

	if(ProgramAddress > (USER_PROGRAM_STARTING_ADDRESS + USER_PROGRAM_SIZE_IN_BYTES))
		return FALSE;

	return TRUE;
}

BOOL RAMAddressValid(OS_WORD RAMAddress)
{
	if(RAMAddress < USER_RAM_STARTING_ADDRESS)
		return FALSE;

	if(RAMAddress > (USER_RAM_STARTING_ADDRESS + USER_RAM_SIZE_IN_BYTES))
		return FALSE;

	return TRUE;
}

BOOL AddressValid(OS_WORD Address)
{
	return (BOOL)(RAMAddressValid(Address) == TRUE || ProgramAddressValid(Address) == TRUE);
}
