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
	Memory Description:
		The OS has a dynamic heap which it controls access to.  This heap is used to initialize data which is
		allocated for at run time.  The C runtime equivalent would be the methods malloc() and free().  The 
		heap size is determined by the OS_HEAP_SIZE_IN_BYTES definition inside of RTOSConfig.h.  There are 
		also events associated with the heap getting below a certain number of bytes.  All these can be 
		configured inside of RTOSConfig.h.  
	
		I've seen many people on various forms asking the question "How do I determine how much of the heap 
		I have used?".  The method GetHeapUsedInBytes() can be used to answer this question.  While this method
		probably isn't useful at runtime to make any decisions (or at least shouldn't be used to make any decisions),
		this method may be useful to analyze your application and better understand how it is behaving (or misbehaving).
*/

#ifndef MEMORY_H
	#define MEMORY_H

#include "GenericTypeDefs.h"
#include "RTOSConfig.h"
#include "Port.h"

typedef struct
{
	unsigned long SizeInWords: 31;
	unsigned int Allocated: 1;
}OS_MEMORY_BLOCK_HEADER;

/*
	BOOL OS_InitializeHeap(void)

	Description: This method initializes the heap in the OS.  This method 
	should not be called by the user.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
		BOOL - TRUE if the heap was initialized successfully, FALSE otherwise.

	Notes:
		- Only the Kernel calls this method.

	See Also:
		- None
*/
BOOL OS_InitializeHeap(void);

/*
	void *OS_AllocateMemory(UINT32 SizeInBytes)

	Description: This method allocates space in the heap and returns a pointer to the
	newly allocated space.  The allocated space is left uninitialized.  This method
	uses a first fit algorithm.  Meaning the first area of memory which is big enough
	to fulfill the request will be used.

	Blocking: No

	User Callable: No

	Arguments:
		UINT32 SizeInBytes - The number of bytes to allocate in the heap.  This number
		will be rounded up to be evenly divisible by sizeof(OS_WORD).

	Returns:
		void * - A void pointer to the newly allocated space upon success.  If the space
		was not available a (void*)NULL pointer is returned.

	Notes:
		- None

	See Also:
		- OS_ReleaseMemory()
*/
void *OS_AllocateMemory(UINT32 SizeInBytes);

/*
	BOOL OS_ReleaseMemory(void *Ptr)

	Description: This method unallocates space in the heap.  If there are unallocated
	spaces in the heap on either side of the newly unallocated area, these spaces
	are merged into one bigger space.

	Blocking: No

	User Callable: No

	Arguments:
		void *Ptr - A pointer to an area in the heap previous allocated by AllocateMemory().

	Returns:
		BOOL - TRUE if the memory was released back into the heap, FALSE otherwise.

	Notes:
		- None

	See Also:
		- OS_AllocateMemory()
*/
BOOL OS_ReleaseMemory(void *Ptr);

/*
	UINT32 GetHeapUsedInBytes(void)

	Description: This method returns the number of bytes allocated in the heap of the OS.

	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns: 
		UINT32 - The number of bytes used in the heap of the OS.  This number is inclusive
		of the space needed to manage the heap.

	Notes:
		- USING_GET_HEAP_USED_IN_BYTES_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- ReleaseMemory()
*/
UINT32 GetHeapUsedInBytes(void);

/*
	UINT32 GetHeapRemainingInBytes(void)

	Description: This method returns the number of bytes currently free in the heap.

	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns: 
		UINT32 - The number of bytes currently free in the heap.

	Notes:
		- USING_GET_HEAP_REMAINING_IN_BYTES_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- ReleaseMemory()
*/
UINT32 GetHeapRemainingInBytes(void);

/*
	void *AllocateMemory(UINT32 SizeInBytes)

	Description: This method allocates space in the heap and returns a pointer to the
	newly allocated space.  The allocated space is left uninitialized.  This method 
	uses a first fit algorithm.  Meaning the first area of memory which is big enough
	to fulfill the request will be used.

	Blocking: No

	User Callable: Yes

	Arguments:
		UINT32 SizeInBytes - The number of bytes to allocate in the heap.  This number 
		will be rounded up to be evenly divisible by sizeof(OS_WORD).

	Returns: 
		void * - A void pointer to the newly allocated space upon success.  If the space
		was not available a (void*)NULL pointer is returned.

	Notes:
		- None

	See Also:
		- ReleaseMemory()
*/
void *AllocateMemory(UINT32 SizeInBytes);

/*
	BOOL ReleaseMemory(void *Ptr)

	Description: This method unallocates space in the heap.  If there are unallocated
	spaces in the heap on either side of the newly unallocated area, these spaces
	are merged into one bigger space.

	Blocking: No

	User Callable: Yes

	Arguments:
		void *Ptr - A pointer to an area in the heap previous allocated by AllocateMemory().

	Returns: 
		BOOL - TRUE if the memory was released back into the heap, FALSE otherwise.

	Notes:
		- None

	See Also:
		- AllocateMemory()
*/
BOOL ReleaseMemory(void *Ptr);

/*
	void *CallocMemory(UINT32 SizeInBytes)

	Description: This method allocates space in the heap and returns a pointer to the
	newly allocated space.  The allocated space is initialized with all zeros.  This
	method calls AllocateMemory() to allocate the memory.

	Blocking: No

	User Callable: Yes

	Arguments:
        UINT32 NumberOfItems - The number of items that are of SizeInBytes size to 
        allocate space for in the heap.
 
		UINT32 SizeInBytes - The number of bytes to allocate in the heap.  This number
		will be rounded up to be evenly divisible by sizeof(OS_WORD).

	Returns: 
		void * - A void pointer to the newly allocated space upon success.  If the space
		was not availble a (void*)NULL pointer is returned.

	Notes: 
		- USING_CALLOC_MEMORY_METHOD inside of RTOSConfig.h must be defined as 1 to
		use this method.

	See Also:
		- None
*/
void *CallocMemory(UINT32 NumberOfItems, UINT32 SizeInBytes);

/*
	void *ReallocMemory(void *Ptr, UINT32 SizeInBytes)

	Description: This method will first attempt to allocate SizeInBytes space in the
	heap.  Then if Ptr is not NULL it will copy the data located at Ptr to the newly
	allocated area and then called ReleaseMemory(Ptr).

	Blocking: No

	User Callable: Yes

	Arguments:
		void *Ptr - Either (void*)NULL or data previously allocated with a call to 
		AllocateMemory().

		UINT32 SizeInBytes - The number of bytes to allocate in the heap.  This number
		will be rounded up to be evenly divisible by sizeof(OS_WORD).  The data specified
		by Ptr will be copied to the newly allocated area if Ptr is not (void*)NULL.  If
		Ptr is not (void*)NULL this method will attempt to free it from the heap.

	Returns: 
		void * - A void pointer to the newly allocated space upon success.  If the space
		was not available a (void*)NULL pointer is returned.

	Notes: 
		- USING_REALLOC_MEMORY_METHOD inside of RTOSConfig.h must be defined as 1 to
		use this method.

	See Also:
		- ReleaseMemory()
*/
void *ReallocMemory(void *Ptr, UINT32 SizeInBytes);

/*
	BOOL AddressInHeap(OS_WORD Address)

	Description: This method will check to see if the address is within the heap.

	Blocking: No

	User Callable: Yes

	Arguments:
		OS_WORD Address - An address to check if it is located within the heap.

	Returns: 
		BOOL - TRUE if the address is within the heap, FALSE otherwise.

	Notes:
		- None

	See Also:
		- None
*/
BOOL AddressInHeap(OS_WORD Address);

/*
	BOOL ProgramAddressValid(OS_WORD ProgramAddress)

	Description: This method will check to see if the address is within program
	memory of the system.  USER_PROGRAM_STARTING_ADDRESS in RTOSConfig.h and 
	USER_PROGRAM_SIZE_IN_BYTES in Port.h must be configured correctly for
	this function to work.  This function assumes program memory is contiguous 
	starting from USER_PROGRAM_STARTING_ADDRESS.

	Blocking: No

	User Callable: Yes

	Arguments:
		OS_WORD Address - An address to check if it is located within the program memory
		of the system.

	Returns: 
		BOOL - TRUE if the address is within program memory, FALSE otherwise.

	Notes:
		- None

	See Also:
		- RAMAddressValid()
*/
BOOL ProgramAddressValid(OS_WORD ProgramAddress);

/*
	BOOL RAMAddressValid(OS_WORD RAMAddress)

	Description: This method will check to see if the address is within RAM
	of the system.  USER_RAM_STARTING_ADDRESS in RTOSConfig.h and
	USER_RAM_SIZE_IN_BYTES in Port.h must be configured correctly for
	this function to work.  This function assumes RAM is contiguous starting
	from USER_RAM_STARTING_ADDRESS.

	Blocking: No

	User Callable: Yes

	Arguments:
		OS_WORD Address - An address to check if it is located within the RAM
		of the system.

	Returns: 
		BOOL - TRUE if the address is within RAM, FALSE otherwise.

	Notes:
		- None

	See Also:
		- ProgramAddressValid()
*/
BOOL RAMAddressValid(OS_WORD RAMAddress);

/*
	BOOL AddressValid(OS_WORD Address)

	Description: This method will check to see if the address is within RAM
	or program memory of the system.  USER_RAM_STARTING_ADDRESS in RTOSConfig.h and
	USER_RAM_SIZE_IN_BYTES() and USER_PROGRAM_STARTING_ADDRESS in RTOSConfig.h and 
	USER_PROGRAM_SIZE_IN_BYTES() in Port.h must be configured correctly for
	this function to work.

	Blocking: No

	User Callable: Yes

	Arguments:
		OS_WORD Address - An address to check if it is located within the RAM
		or program space of the system.

	Returns:
		BOOL - TRUE if the address is within RAM or program space, FALSE otherwise.

	Notes:
		- None

	See Also:
		- ProgramAddressValid(), RAMAddressValid().
*/
BOOL AddressValid(OS_WORD Address);

#endif // end of MEMORY_H
