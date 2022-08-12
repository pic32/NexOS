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
	File Name: GenericBufferObject.h
	Version: 1.06
	IDE: Visual Studio 2010 Professional
	Compiler: C89

	Description:
	This file contains the definition of the GENERIC_BUFFER
	data structure.  This file was created just to 
	further separate out the structure of the library.
*/

#ifndef GENERIC_BUFFER_OBJECT_H
	#define GENERIC_BUFFER_OBJECT_H

#include "GenericTypes.h"

/*
	The following struct is the definition
	of the GENERIC_BUFFER.  This holds all 
	the necessary data to manipulate the 
	GENERIC_BUFFER.
*/
struct _GenericBuffer
{
	/*
		The current write position of the GENERIC_BUFFER.  The location that
		this pointer points to is where the next byte that will be written to
		the GENERIC_BUFFER will be placed.
	*/
	BYTE *CurrentWritePosition;

	/*
		The current read position of the GENERIC_BUFFER.  The location that
		this pointer points to is where the next byte that will be read from
		the GENERIC_BUFFER.
	*/
	BYTE *CurrentReadPosition;

	/*
		This variable denotes how full the GENERIC_BUFFER is in bytes.  This
		value cannot be larger than BufferCapacity.
	*/
	UINT32 BufferSize;

	/*
		This variable denotes how many bytes at most a GENERIC_BUFFER can hold.
	*/
	UINT32 BufferCapacity;

	/*
		This is a pointer to the area in RAM that will actually hold the data
		that the GENERIC_BUFFER can write and read.
	*/
	BYTE *Buffer;
};

typedef struct _GenericBuffer GENERIC_BUFFER;


#endif // end of GENERIC_BUFFER_OBJECT_H
