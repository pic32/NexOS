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
	File Name: GenericBufferConfig.h
	Version: 1.06
	IDE: Visual Studio 2010 Professional
	Compiler: C89

	Description:
	This file configures the options built into the GenericBuffer Library.
	The below set of defines allows the user to enable certain methods
	located inside the GenericBuffer library.

	It also allows the user to define how the GenericBuffer library will
	allocate and deallocate memory for new nodes.

	All methods are thread safe(re-entrant), with the potential exception 
	of GenericBufferCreate, GenericBufferDelete.  The reason
	for this is that it all depends on how the user defines the way the 
	GenericBuffer library will allocate, deallocate memory.
*/

#ifndef GENERIC_BUFFER_CONFIG_H
	#define GENERIC_BUFFER_CONFIG_H

/**
	*Set USING_GENERIC_BUFFER_PEEK_METHOD to 1 to enable the
	GenericBufferPeek() method.
*/
#define USING_GENERIC_BUFFER_PEEK_METHOD					1

/**
*Set USING_GENERIC_BUFFER_CONTAINS_METHOD to 1 to enable the
GenericBufferContains() method.
*/
#define USING_GENERIC_BUFFER_CONTAINS_METHOD				1

/**
 Set USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD to 1 to 
 enable the GenericBufferContainsSequence() method.
*/
#define USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD		1

/**
 Set USING_GENERIC_BUFFER_SEEK_METHOD to 1 to enable the
 GenericBufferSeek() method.
*/
#define USING_GENERIC_BUFFER_SEEK_METHOD					1

/**
	*Set USING_GENERIC_BUFFER_FLUSH_METHOD to 1 to enable the
	GenericBufferFlush() method.
*/
#define USING_GENERIC_BUFFER_FLUSH_METHOD					1

/**
	*Set USING_GENERIC_BUFFER_GET_SIZE_METHOD to 1 to enable the
	GenericBufferGetSize() method.
*/
#define USING_GENERIC_BUFFER_GET_SIZE_METHOD				1

/**
	*Set USING_GENERIC_BUFFER_GET_CAPACITY_METHOD to 1 to enable the
	GenericBufferGetCapacity() method.
*/
#define USING_GENERIC_BUFFER_GET_CAPACITY_METHOD			1

/**
	*Set USING_GENERIC_BUFFER_DELETE_METHOD to 1 to enable the
	GenericBufferDelete() method.
*/
#define USING_GENERIC_BUFFER_DELETE_METHOD					1

/**
	*Set USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD to 1 to enable the
	GenericBufferGetRemainingBytes() method.
*/
#define USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD		1

/**
	*Set USING_GENERIC_BUFFER_GET_LIBRARY_VERSION to 1 to endable the 
	GenericBufferGetLibraryVersion() method.
*/

#define USING_GENERIC_BUFFER_GET_LIBRARY_VERSION			1

/**
	*Set GENERIC_BUFFER_SAFE_MODE to 1 to enable the portions of code
	inside the GENERIC_BUFFER Library that check to make sure all passed
	in parameters are of a valid nature.
*/
#define GENERIC_BUFFER_SAFE_MODE							1

/**
	*This defines what method the GenericBuffer library will use to 
	allocate memory and deallocate memory.  This allows the GenericBuffer 
	library to be integrated into an OS.
*/
#define BufferMemAlloc(Mem)									AllocateMemory(Mem)

/**
	*The method used for freeing up memory.
*/
#define BufferMemDealloc(Mem)								ReleaseMemory(Mem)

/**
	*Define USE_MALLOC as 1 to enable the stdlib.h file included
	with GenericBuffer.c.
*/
//#define GENERIC_BUFFER_USE_MALLOC							1

/**
	*If the user isn't using malloc then include the file that will
	be needed for accessing whatever method MemAlloc() and MemDealloc()
	implement, below.
*/
#include "../NexOS/Kernel/Memory.h"

#endif // end of GENERIC_BUFFER_CONFIG_H
