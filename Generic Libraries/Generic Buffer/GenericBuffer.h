/*
	Developer: brodie
	Date: July 9, 2015
	File Name: GenericBuffer.h
	Version: 1.06
	IDE: Visual Studio 2010 Professional
	Compiler: C89

	Description:
	This file contains all the the method headers for interfacing to the
	GenericBuffer library.  This should be included in any file that references
	a GENERIC_BUFFER.  All methods and macro's have their documentation above
	their declaration.
*/

/*! \mainpage GenericBuffer Library
 *  \brief This is a Library written in C for manipulating a GenericBuffer Data Structure.
 *  \author brodie
 *  \version 1.06
 *  \date   July 9, 2015
 */

#ifndef GENERIC_BUFFER_H
	#define GENERIC_BUFFER_H

#include "GenericBufferObject.h"
#include "GenericBufferConfig.h"

/*
	Function: GENERIC_BUFFER *CreateGenericBuffer(GENERIC_BUFFER *GenericBuffer, UINT32 CapacityInBytes, BYTE *Buffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - The address at which the GENERIC_BUFFER
		will be initialized.  If NULL is passed in then this method will 
		create a GENERIC_BUFFER out of a the heap with a call to
		BufferMemAlloc()(User Supplied Define).

		UINT32 CapacityInBytes - The number of bytes that the GENERIC_BUFFER can
		store.  If Buffer is NULL then this parameter defines how many bytes this
		method will ask BufferMemAlloc() for.

		BYTE *Buffer - The address of the pre-determined area in RAM at which
		the GENERIC_BUFFER will store it's data.  If this is NULL then this method
		will use CapacityInBytes to determine how big of a buffer in RAM to 
		create through a call to BufferMemAlloc().

	Returns:
		GENERIC_BUFFER* - The address at which the newly initialized GENERIC_BUFFER resides
		in memory.  If a new GENERIC_BUFFER could not be created then (GENERIC_BUFFER*)NULL is returned.

	Description: Creates a new GENERIC_BUFFER in memory, and can allocate room for the buffer
	where the storage of the data will be held.

	Notes: None
*/
/**
		* @brief Initializes a GENERIC_BUFFER, and can create a GENERIC_BUFFER.
		* @param *GenericBuffer- A pointer to an already allocate GENERIC_BUFFER or a NULL GENERIC_BUFFER 
		pointer to create a GENERIC_BUFFER from BufferMemAlloc().
		* @param CapacityInBytes - This is how big the Buffer will be in bytes.  This number must be greater
		than 3.  If BYTE *Buffer is (BYTE*)NULL, then the value of CapacityInBytes will be used when 
		CreateGenericBuffer calls BufferMemAlloc().  Otherwise CapacityInBytes should be the size of the Buffer
		in RAM that BYTE *Buffer is pointing to.
		* @param *Buffer - This is a pointer to a spot in RAM that the GENERIC_BUFFER will use for Buffering
		the data.  If this parameter is equal to (BYTE*)NULL, then CreateGenericBuffer will call BufferMemAlloc()
		and pass in the size from CapacityInBytes.
		* @return *GENERIC_BUFFER - The address of the GENERIC_BUFFER in memory.  If a GENERIC_BUFFER could
		not be allocated, returns a NULL GENERIC_BUFFER pointer.
		* @note None.
		* @sa BufferMemAlloc()
		* @since v1.0
*/
GENERIC_BUFFER *CreateGenericBuffer(GENERIC_BUFFER *GenericBuffer, UINT32 CapacityInBytes, BYTE *Buffer);

/*
	Function: UINT32 GenericBufferWrite(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, const BYTE *DataToWrite)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER that 
		will be written to.

		UINT32 NumberOfBytes - The number of bytes from const BYTE *DataToWrite that will
		be written to the GENERIC_BUFFER.

		const BYTE *DataToWrite - A pointer to the data that will be written to the
		GENERIC_BUFFER.

	Returns:
		UINT32 - The number of bytes successfully written to inside the GENERIC_BUFFER.

	Description: Writes the specified number of bytes from the user buffer to the GENERIC_BUFFER.

	Notes: None
*/
/**
		* @brief Writes to a GENERIC_BUFFER.
		* @param *GenericBuffer- A pointer to a GENERIC_BUFFER.
		* @param NumberOfBytes - The number of bytes from const BYTE *DataToWrite that will
		be written to the GENERIC_BUFFER.
		* @param *DataToWrite - A pointer to the data that will be written to the
		GENERIC_BUFFER.
		* @return UINT32 - The number of bytes successfully written to inside the GENERIC_BUFFER.
		* @note None.
		* @sa None
		* @since v1.00
*/
UINT32 GenericBufferWrite(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, const BYTE *DataToWrite);

/*
	Function: UINT32 GenericBufferRead(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, BOOL NullTerminate)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER that 
		will be read from.

		UINT32 NumberOfBytes - The number of bytes from the GENERIC_BUFFER that
		will be read into DestinationBuffer

		BYTE *DestinationBuffer - A pointer to the area in RAM that will have 
		NumberOfBytes copied to it from the GENERIC_BUFFER.

		UINT32 DestinationBufferSize - The size of the user destination buffer in bytes.

		BOOL NullTerminate - If set to TRUE, the user buffer will be NULL terminated.

	Returns:
		UINT32 - The number of bytes successfully read from the GENERIC_BUFFER to the user buffer.

	Description: Read the specified number of bytes from the GENERIC_BUFFER to the user buffer.
	The user buffer is NULL terminated.

	Notes: None
*/
/**
		* @brief Reads from a GENERIC_BUFFER.  The user buffer is NULL terminated.
		* @param *GenericBuffer- A pointer to a GENERIC_BUFFER.
		* @param NumberOfBytes - The number of bytes from the GENERIC_BUFFER that will
		be written to BYTE *DestinationBuffer.
		* @param *DestinationBuffer - A pointer to the area in RAM that will have 
		NumberOfBytes copied to it from the GENERIC_BUFFER.
		* @param DestinationBufferSize - The size of the user destination buffer in bytes.
		* @param NullTerminate - If set to TRUE, the user buffer will be NULL terminated.
		* @return UINT32 - The number of bytes successfully read from the GENERIC_BUFFER.
		* @note None.
		* @sa None
		* @since v1.00
*/
UINT32 GenericBufferRead(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, BOOL NullTerminate);

/*
	Function: UINT32 GenericBufferPeek(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, BOOL NullTerminate)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER that 
		will be peeked at.  The contents of the GENERIC_BUFFER remain the same
		as if the peek never took place.

		UINT32 NumberOfBytes - The number of bytes from the GENERIC_BUFFER that
		will be read into DestinationBuffer

		BYTE *DestinationBuffer - A pointer to the area in RAM that will have 
		NumberOfBytes copied to it from the GENERIC_BUFFER.

		UINT32 DestinationBufferSize - The size of the user destination buffer in bytes.

		BOOL NullTerminate - If set to TRUE, the user buffer will be NULL terminated.

	Returns:
		UINT32 - The number of bytes successfully read from the GENERIC_BUFFER to the user buffer.

	Description: Peek at the specified number of bytes from the GENERIC_BUFFER to the user buffer.
	The user buffer is NULL terminated.

	Notes: USING_GENERIC_BUFFER_PEEK_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
*/
/**
		* @brief Peeks at a GENERIC_BUFFER.  The GENERIC_BUFFER remains unchanged.  The user buffer is NULL terminated.
		* @param *GenericBuffer - A pointer to a GENERIC_BUFFER.
		* @param NumberOfBytes - The number of bytes from the GENERIC_BUFFER that will
		be written to BYTE *DestinationBuffer.
		* @param DestinationBufferSize - The size of the user destination buffer in bytes.
		* @param *DestinationBuffer - A pointer to the area in RAM that will have 
		NumberOfBytes copied to it from the GENERIC_BUFFER.
		* @param NullTerminate - If set to TRUE, the user buffer will be NULL terminated.
		* @return UINT32 - The number of bytes successfully read from the GENERIC_BUFFER.
		* @note USING_GENERIC_BUFFER_PEEK_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
		* @sa None
		* @since v1.00
*/
#if (USING_GENERIC_BUFFER_PEEK_METHOD == 1)
	UINT32 GenericBufferPeek(GENERIC_BUFFER *GenericBuffer, UINT32 NumberOfBytes, BYTE *DestinationBuffer, UINT32 DestinationBufferSize, BOOL NullTerminate);
#endif // end of USING_GENERIC_BUFFER_PEEK_METHOD

	/*
	Function: UINT32 GenericBufferContains(GENERIC_BUFFER *GenericBuffer, BYTE Character)

	Parameters:
	GENERIC_BUFFER *GenericBuffer - A pointer to a GENERIC_BUFFER.

	BYTE Character - The character to look for inside of the GENERIC_BUFFER.

	Returns:
	UINT32 - The number of times Character was found inside the GENERIC_BUFFER.

	Description: Searches a GENERIC_BUFFER for a particular byte.

	Notes: USING_GENERIC_BUFFER_CONTAINS_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
	*/
	/**
	* @brief Searches a GENERIC_BUFFER for a particular byte.
	* @param *GenericBuffer - A pointer to a GENERIC_BUFFER.
	* @param Character - The character to look for inside of the GENERIC_BUFFER.
	* @return UINT32 - The number of times Character was found inside the GENERIC_BUFFER.
	* @note USING_GENERIC_BUFFER_CONTAINS_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
	* @sa GenericBufferContainsSequence()
	* @since v1.04
	*/
#if (USING_GENERIC_BUFFER_CONTAINS_METHOD == 1)
	UINT32 GenericBufferContains(GENERIC_BUFFER *GenericBuffer, BYTE Character);
#endif // end of USING_GENERIC_BUFFER_CONTAINS_METHOD

	/*
	Function: BOOL GenericBufferContainsSequence(GENERIC_BUFFER *GenericBuffer, BYTE *Sequence, UINT32 SequenceSize)

	Parameters:
	GENERIC_BUFFER *GenericBuffer - A pointer to a GENERIC_BUFFER.

	BYTE *Sequence - The sequence of bytes to search for.

	UINT32 SequenceSize - The size of the sequence of bytes to search for.

	Returns:
	BOOL - TRUE if the GENERIC_BUFFER conatains the sequence of bytes, FALSE otherwise.

	Description: Searches a GENERIC_BUFFER for a particular sequence of bytes.

	Notes: USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
	*/
	/**
	* @brief Searches a GENERIC_BUFFER for a particular sequence of bytes.
	* @param *GenericBuffer - A pointer to a GENERIC_BUFFER.
	* @param *Sequence - The sequence of bytes to search for.
	* @param SequenceSize - The size of the sequence of bytes to search for.
	* @return BOOL - TRUE if the GENERIC_BUFFER conatains the sequence of bytes, FALSE otherwise.
	* @note USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
	* @sa GenericBufferContains()
	* @since v1.06
	*/
#if (USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD == 1)
	BOOL GenericBufferContainsSequence(GENERIC_BUFFER *GenericBuffer, BYTE *Sequence, UINT32 SequenceSize);
#endif // end of USING_GENERIC_BUFFER_CONTAINS_SEQUENCE_METHOD

/*
	Function: UINT32 GenericBufferSeek(GENERIC_BUFFER *GenericBuffer, BYTE Character)

	Parameters:
	GENERIC_BUFFER *GenericBuffer - A pointer to a GENERIC_BUFFER.

	BYTE Character - The character to look for inside of the GENERIC_BUFFER.

	Returns:
	UINT32 - How many bytes deep the specified character is.  If Character is not
			 found then this method returns 0.  This method is 1 based, meaning if
			 a 1 is returned, the first byte that was compared has the same value
			 as Character.

	Description: Searches in a GENERIC_BUFFER for how many bytes in a certain byte is.

	Notes: USING_GENERIC_BUFFER_SEEK_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
	*/
	/**
	* @brief Searches a GENERIC_BUFFER for a particular byte.  This method is 1 based, meaning if a 1 is returned, the first byte that was compared has the same value
			 as Character.
	* @param *GenericBuffer - A pointer to a GENERIC_BUFFER.
	* @param Character - The character to look for inside of the GENERIC_BUFFER.
	* @return UINT32 - How many bytes deep the specified character is.  If Character is not found then this method returns 0.
	* @note USING_GENERIC_BUFFER_SEEK_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
	* @sa None
	* @since v1.05
*/
#if (USING_GENERIC_BUFFER_SEEK_METHOD == 1)
	UINT32 GenericBufferSeek(GENERIC_BUFFER *GenericBuffer, BYTE Character);
#endif // end of USING_GENERIC_BUFFER_SEEK_METHOD

/*
	Function: BOOL GenericBufferFlush(GENERIC_BUFFER *GenericBuffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER that 
		will be flushed.

	Returns:
		BOOL - TRUE if the GENERIC_BUFFER was successfully flushed, FALSE otherwise.

	Description: Flushes the specified GENERIC_BUFFER.  The internal read and write pointers are 
	reset to the beginning of the GENERIC_BUFFER and the size is set to 0.

	Notes: USING_GENERIC_BUFFER_FLUSH_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
*/
/**
		* @brief Empties and resets a GENERIC_BUFFER.
		* @param *GenericBuffer- A pointer to a GENERIC_BUFFER.
		* @return BOOL - TRUE if the GENERIC_BUFFER was successfully flushed, FALSE otherwise.
		* @note USING_GENERIC_BUFFER_FLUSH_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
		* @sa None
		* @since v1.00
*/
#if (USING_GENERIC_BUFFER_FLUSH_METHOD == 1)
	BOOL GenericBufferFlush(GENERIC_BUFFER *GenericBuffer);
#endif // end of USING_GENERIC_BUFFER_FLUSH_METHOD

/*
	Function: UINT32 GenericBufferGetSize(GENERIC_BUFFER *GenericBuffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER.

	Returns:
		UINT32 - The number of bytes that are currently in use inside of the GENERIC_BUFFER.

	Description: Returns the size in bytes of how full the GENERIC_BUFFER currently is.

	Notes: USING_GENERIC_BUFFER_GET_SIZE_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
*/
/**
		* @brief Returns the size in bytes of how full the GENERIC_BUFFER currently is.
		* @param *GenericBuffer- A pointer to a GENERIC_BUFFER.
		* @return UINT32 - The number of bytes that are currently in use inside of the GENERIC_BUFFER.
		* @note USING_GENERIC_BUFFER_GET_SIZE_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
		* @sa None
		* @since v1.00
*/
#if (USING_GENERIC_BUFFER_GET_SIZE_METHOD == 1)
	UINT32 GenericBufferGetSize(GENERIC_BUFFER *GenericBuffer);
#endif // end of USING_GENERIC_BUFFER_GET_SIZE_METHOD

/*
	Function: UINT32 GenericBufferGetCapacity(GENERIC_BUFFER *GenericBuffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER.

	Returns:
		UINT32 - The number of bytes that the GENERIC_BUFFER can hold.

	Description: Returns the number of bytes that the GENERIC_BUFFER can hold.

	Notes: USING_GENERIC_BUFFER_GET_CAPACITY_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
*/
/**
		* @brief Returns the available amount of space inside the GENERIC_BUFFER
		in bytes that can be written to.
		* @param *GenericBuffer- A pointer to a GENERIC_BUFFER.
		* @return UINT32 - Returns the number of bytes that the GENERIC_BUFFER can hold.
		* @note USING_GENERIC_BUFFER_GET_CAPACITY_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
		* @sa None
		* @since v1.00
*/
#if (USING_GENERIC_BUFFER_GET_CAPACITY_METHOD == 1)
	UINT32 GenericBufferGetCapacity(GENERIC_BUFFER *GenericBuffer);
#endif // end of USING_GENERIC_BUFFER_GET_CAPACITY_METHOD

/*
	Function: BOOL GenericBufferDelete(GENERIC_BUFFER *GenericBuffer, BOOL FreeBufferSpace)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER that will
		be deleted.

		BOOL FreeBufferSpace - Set this to TRUE if the GENERIC_BUFFER's buffer space
		was allocated when CreateGenericBuffer was called.  FALSE otherwise.

	Returns:
		BOOL - TRUE if the deletion was successfull, otherwise FALSE.

	Description: Frees the specified GENERIC_BUFFER and the buffer space inside the
	GENERIC_BUFFER if the user desires.

	Notes: USING_GENERIC_BUFFER_DELETE_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
*/
/**
		* @brief Deletes the specified GENERIC_BUFFER and the buffer inside the GENERIC_BUFFER
		that holds the data if the user wishes.
		* @param *GenericBuffer- A pointer to a GENERIC_BUFFER.
		* @param FreeBufferSpace - Set to TRUE if the buffer that holds the data inside of 
		the GENERIC_BUFFER should be freed, FALSE otherwise.
		* @return BOOL - TRUE if the GENERIC_BUFFER was successfully deleted from RAM, FALSE
		otherwise.
		* @note USING_GENERIC_BUFFER_DELETE_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
		* @sa CreateGenericBuffer()
		* @since v1.00
*/
#if (USING_GENERIC_BUFFER_DELETE_METHOD == 1)
	BOOL GenericBufferDelete(GENERIC_BUFFER *GenericBuffer, BOOL FreeBufferSpace);
#endif // end of USING_GENERIC_BUFFER_DELETE_METHOD

/*
	Function: UINT32 GenericBufferGetRemainingBytes(GENERIC_BUFFER *GenericBuffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER.

	Returns:
		UINT32 - The number of bytes available inside the GENERIC_BUFFER that
		can be currently written to.

	Description: Returns the number of bytes available inside the GENERIC_BUFFER that
	can be currently written to.

	Notes: USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
*/
/**
		* @brief Returns the number of bytes available inside the GENERIC_BUFFER that
		can be currently written to.
		* @param *GenericBuffer- A pointer to a GENERIC_BUFFER.
		* @return UINT32 - The number of bytes available inside the GENERIC_BUFFER that
		can be currently written to.
		* @note USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD in GenericBufferConfig.h must be defined as 1 to use method.
		* @sa None
		* @since v1.00
*/
#if (USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD == 1)
	UINT32 GenericBufferGetRemainingBytes(GENERIC_BUFFER *GenericBuffer);
#endif // end of USING_GENERIC_BUFFER_GET_REMAINING_BYTES_METHOD

/*
	Function: const BYTE *GenericBufferGetLibraryVersion(void)

	Parameters: 
		None

	Returns:
		const BYTE* - The version number of the GENERIC_BUFFER Library.  This string is 
		(BYTE)NULL terminated.

	Description: This method will return the version number of the GENERIC_BUFFER Library.

	Notes: USING_GENERIC_BUFFER_GET_LIBRARY_VERSION in GenericBufferConfig.h must be defined as 1 to use method.
*/
/**
		* @brief The version number of the GENERIC_BUFFER Library.
		* @param None
		* @return const BYTE* - The version number of the GENERIC_BUFFER Library.
		This string is (BYTE)NULL terminated.
		* @note USING_GENERIC_BUFFER_GET_LIBRARY_VERSION in GenericBufferConfig.h must be defined as 1 to use method.
		* @sa None
		* @since v1.02
*/
#if (USING_GENERIC_BUFFER_GET_LIBRARY_VERSION == 1)
	const BYTE *GenericBufferGetLibraryVersion(void);
#endif // end of USING_GENERIC_BUFFER_GET_LIBRARY_VERSION

/*
	Macro: BOOL GenericBufferIsNull(GENERIC_BUFFER *GenericBuffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER.

	Returns:
		BOOL - TRUE if the GENERIC_BUFFER is NULL, FALSE otherwise.

	Description: Checks to see if the GENERIC_BUFFER is NULL.

	Notes: None
*/
#define GenericBufferIsNull(GenBuff)					(GenBuff == (GENERIC_BUFFER*)NULL)

/*
	Macro: BOOL GenericBufferIsFull(GENERIC_BUFFER *GenericBuffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER.

	Returns:
		BOOL - TRUE if the GENERIC_BUFFER is full, FALSE otherwise.

	Description: Checks to see if the GENERIC_BUFFER is full.

	Notes: None
*/
#define GenericBufferIsFull(GenBuff)					(GenBuff->BufferSize == (UINT32)(GenBuff->BufferCapacity))

/*
	Macro: BOOL GenericBufferIsEmpty(GENERIC_BUFFER *GenericBuffer)

	Parameters: 
		GENERIC_BUFFER *GenericBuffer - A pointer to the GENENERIC_BUFFER.

	Returns:
		BOOL - TRUE if the GENERIC_BUFFER is empty, FALSE otherwise.

	Description: Checks to see if the GENERIC_BUFFER is empty.

	Notes: None
*/
#define GenericBufferIsEmpty(GenBuff)					(GenBuff->BufferSize == (UINT32)0)

#endif // end of GENERIC_BUFFER_H
