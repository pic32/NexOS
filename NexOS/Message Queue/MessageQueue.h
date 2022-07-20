/*
    NexOS Kernel Version v1.01.03
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
	MESSAGE_QUEUE Description:
		A MESSAGE_QUEUE can be used to write data from a TASK to an ISR, or vice versa.  There are
		separate methods for if the data is read/written from an ISR or a TASK.

		A MESSAGE_QUEUE uses the OS heap to allocate data on the fly.  Each time data is added to the
		MESSAGE_QUEUE space from the OS heap is used to store the data.  This allows a MESSAGE_QUEUE
		to grow dynamically at runtime as needed.

		The MESSAGE_QUEUE is designed to be used by one consumer and one producer.  However, this 
		behavior is not enforced by the OS.  A TASK can block waiting for data from the MESSAGE_QUEUE, 
		but another TASK could come along and read the data before the blocking TASK unblocks and gets it.
 
        A MESSAGE_QUEUE is designed to copy by reference and not by value and data is stored in a FIFO fashion.
*/

#ifndef MESSAGE_QUEUE_H
	#define MESSAGE_QUEUE_H

#include "RTOSConfig.h"
#include "../Kernel/Kernel.h"

#define MESSAGE_QUEUE_INVALID_SIZE                                          0xFFFFFFFF

typedef struct
{
    void *NextMessageQueueNode; // this points to the next MESSAGE_QUEUE_NODE in the queue
    
    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
        UINT32 MessageCode; // this is a piece of data the user sets
    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

	#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
		void *AdditionalData; // this points to optional user data of type unknown
	#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
}MESSAGE_QUEUE_NODE;

typedef struct
{
	MESSAGE_QUEUE_NODE *MessageQueueData;

	#if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)
		UINT32 MessageQueueSize;
	#endif // end of #if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)

	#if (USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD == 1)
		BOOL(*MessageQueueFreeMethod)(void *); // this optionally points to a method to free data in a node when clear is called.
	#endif // end of #if (USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD == 1)

	#if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)
		/*
			Event though this data structure is capable of holding
			multiple nodes, we will not use this feature.  It will
			hold no more than 1 node at any given time.  The 
			MESSAGE_QUEUE uses this data structure because all other
			resources in the OS use it.  When a TASK is blocked the
			OS assumes this data structure is the one used to hold
			the blocked TASK.
		*/
		DOUBLE_LINKED_LIST_HEAD BlockedListHead;
	#endif // end of #if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)

    #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
        // This is used to know if a TASK is trying to get data from the MESSAGE_QUEUE.
        // If there is a lock count, the MESSAGE_QUEUE cannot be deleted.
        UINT32 BlockDeleteCount;
    #endif // end of #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
}MESSAGE_QUEUE;

/*
    MESSAGE_QUEUE *CreateMessageQueue(MESSAGE_QUEUE *MessageQueue, BOOL(*MessageQueueFreeMethod)(void *))

	Description: This method creates a MESSAGE_QUEUE which can be used to communicate
    data between two TASKs or a TASK and an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue - A pointer to an already allocated MESSAGE_QUEUE.
        If (MESSAGE_QUEUE*)NULL is passed for this parameter the OS will attempt to
        allocate space in the OS heap for the MESSAGE_QUEUE.

        BOOL(*MessageQueueFreeMethod)(void *) - A method pointer to the free method.
        This method is what will be called when void *AdditionalData needs to be deleted.
        This is used by the methods MessageQueueClear(), MessageQueueFreeData(), 
        MessageQueueDelete() and their ISR equivalents.  This argument can also be NULL in
        which case the MESSAGE_QUEUE will not use the MessageQueueFreeMethod when the 
        methods MessageQueueClear(), MessageQueueFreeData(), MessageQueueDelete() and 
        their ISR equivalents are called.

	Returns: 
		MESSAGE_QUEUE * - A pointer to a valid MESSAGE_QUEUE upon success, otherwise
        a (MESSAGE_QUEUE*)NULL pointer.

	Notes:
		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the MessageQueueFreeMethod functionality.

	See Also:
		- MessageQueueClear(), MessageQueueFreeData(), and MessageQueueDelete()
*/
MESSAGE_QUEUE *CreateMessageQueue(  MESSAGE_QUEUE *MessageQueue

									#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)
										,BOOL(*MessageQueueFreeMethod)(void *)
									#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                    );

/*
    OS_RESULT MessageQueueAdd(MESSAGE_QUEUE *MessageQueue, void *AdditionalData, UINT32 MessageCode)

	Description: This method attempts to add the specified data to the MESSAGE_QUEUE.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void *AdditionalData - A pointer to data which will be copied into the MESSAGE_QUEUE.
        Only the pointer will be copied into the MESSAGE_QUEUE, and not the data it is point to.
  
        UINT32 MessageCode - A 32-bit wide value.  This could be used to denote what the
        void *AdditionalData is pointing to, or just used as the data in the MESSAGE_QUEUE.

	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
        - USING_MESSAGE_QUEUE_ADD_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.
 
		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the UINT32 MessageCode argument.

	See Also:
		- MessageQueueWaitForData(), MessageQueueRemove(), MessageQueuePeek()
*/
OS_RESULT MessageQueueAdd(	MESSAGE_QUEUE *MessageQueue

							#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
								, void *AdditionalData
							#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                , UINT32 MessageCode
                            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                            );

/*
    OS_RESULT MessageQueueWaitForData(MESSAGE_QUEUE *MessageQueue, void **AdditionalData, UINT32 *MessageCode, INT32 TimeoutInTicks)

	Description: This method will first check to see if a message is available in
    MESSAGE_QUEUE.  If it is the method will return with the data.  If data is
    not available, this method will block and wait until data is available.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void **AdditionalData - A pointer to a pointer that will be populated with the
        AdditionalData from when MessageQueueAdd() was called.
 
        UINT32 *MessageCode - A pointer to UINT32 which will be populated with the 
        MessageCode from when MessageQueueAdd() was called.

		INT32 TimeoutInTicks - This is a timeout value in ticks to wait
		for data to be available in the MESSAGE_QUEUE.  Below are valid values for TimeoutInTicks.

            TimeoutInTicks = 1 to (2^31 - 1): The calling TASK will be placed onto the
            Delayed Queue up to the specified number of ticks if
            the MESSAGE_QUEUE has no data in it.

            TimeoutInTicks = 0: The calling TASK will not be placed on the
            Delayed Queue and this method will return OS_RESOURCE_GET_FAILED.

            TimeoutInTicks <= -1: The calling TASK will not be placed on the
            Delay Queue and will wait forever.

	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
        - USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the UINT32 MessageCode argument.
 
		- USING_TASK_DELAY_TICKS_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use the INT32 TimeoutInTicks argument.

	See Also:
		- MessageQueueAdd(), MessageQueueFreeData()
*/
OS_RESULT MessageQueueWaitForData(  MESSAGE_QUEUE *MessageQueue

									#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
										, void **AdditionalData
									#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                        , UINT32 *MessageCode
                                    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

									#if (USING_TASK_DELAY_TICKS_METHOD == 1)
										, INT32 TimeoutInTicks
									#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                    );

/*
    OS_RESULT MessageQueueRemove(MESSAGE_QUEUE *MessageQueue, void **AdditionalData, UINT32 *MessageCode)

	Description: This method will remove the next available message from the MESSAGE_QUEUE.  The 
    size of the MESSAGE_QUEUE will be decreased by 1.  To delete the data pointed to by AdditionalData,
    the method MessageQueueFreeData() should be called if required.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void **AdditionalData - This is where the data written during MessageQueueAdd() will be copied to.
   
        UINT32 *MessageCode - An optional message code associated with AdditionalData will be copied here.
 
	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors and the data was copied, an error value otherwise.

	Notes:
        - USING_MESSAGE_QUEUE_REMOVE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.
 
		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the UINT32 MessageCode argument.

	See Also:
		- MessageQueueWaitForData(), MessageQueueAdd(), MessageQueuePeek(), MessageQueueFreeData()
*/
OS_RESULT MessageQueueRemove(   MESSAGE_QUEUE *MessageQueue

                                #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                                    , void **AdditionalData
                                #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                    , UINT32 *MessageCode
                                #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                );

/*
    OS_RESULT MessageQueuePeek(MESSAGE_QUEUE *MessageQueue, void **AdditionalData, UINT32 *MessageCode)

	Description: This method will read the next set of data out from the MESSAGE_QUEUE, but keep the
    references to the data in the MESSAGE_QUEUE.  The size of the MESSAGE_QUEUE remains the same after
    calling this method.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void **AdditionalData - This is where the data written during MessageQueueAdd() will be copied to.
   
        UINT32 *MessageCode - An optional message code associated with AdditionalData will be copied here.
 
	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors and the data was copied, an error value otherwise.

	Notes:
        - USING_MESSAGE_QUEUE_PEEK_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.
  
		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the UINT32 MessageCode argument.

	See Also:
		- MessageQueueWaitForData(), MessageQueueRemove()
*/
OS_RESULT MessageQueuePeek( MESSAGE_QUEUE *MessageQueue

							#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
								, void **AdditionalData
							#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                , UINT32 *MessageCode
                            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                            );

/*
    UINT32 MessageQueueGetSize(MESSAGE_QUEUE *MessageQueue)

	Description: This method will return the current size of the MESSAGE_QUEUE.  This is how
    many items are stored in the MESSAGE_QUEUE which can be read.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
        UINT32 - The number of items in the MESSAGE_QUEUE which can be read.  If there is an error the
        value MESSAGE_QUEUE_INVALID_SIZE is returned.
        
	Notes:
		- USING_MESSAGE_QUEUE_GET_SIZE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueIsEmpty()
*/
UINT32 MessageQueueGetSize(MESSAGE_QUEUE *MessageQueue);

/*
    BOOL MessageQueueIsEmpty(MESSAGE_QUEUE *MessageQueue)

	Description: This method will check to see if the MESSAGE_QUEUE is empty or not.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
        BOOL - TRUE if the MESSAGE_QUEUE is empty, FALSE otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_IS_EMPTY_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueGetSize()
*/
BOOL MessageQueueIsEmpty(MESSAGE_QUEUE *MessageQueue);

/*
    OS_RESULT MessageQueueClear(MESSAGE_QUEUE *MessageQueue)

	Description: This method clears out the MESSAGE_QUEUE so that MessageQueueGetSize() returns 0 or
    MessageQueueIsEmpty() returns TRUE.  If any data exists in the MESSAGE_QUEUE and a MessageQueueFreeMethod
    was specified when calling CreateMessageQueue(), the data in the MESSAGE_QUEUE will be passed to
    the MessageQueueFreeMethod method.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_CLEAR_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueFreeData(), MessageQueueDelete()
*/
OS_RESULT MessageQueueClear(MESSAGE_QUEUE *MessageQueue);

/*
    OS_RESULT MessageQueueFreeData(MESSAGE_QUEUE *MessageQueue, void *Data)

	Description: This method will free the data associated with the MESSAGE_QUEUE if
    a MessageQueueFreeMethod method was specified when calling CreateMessageQueue().
    This method can be called on the data read from MessageQueueRemove().

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void *Data - The data to free.
	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueRemove()
*/
OS_RESULT MessageQueueFreeData(MESSAGE_QUEUE *MessageQueue, void *Data);

/*
    OS_RESULT MessageQueueDelete(MESSAGE_QUEUE *MessageQueue)

	Description: This method will delete a MESSAGE_QUEUE.  In order to do this (MESSAGE_WUEUE*)NULL
    needs to have been passed in for the MessageQueue argument during the call to CreateMessageQueue().
    Prior to deleting the MESSAGE_QUEUE an equivalent call to MessageQueueClear() is made to
    safely delete all data contained in the MESSAGE_QUEUE.
 
	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_DELETE_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- CreateMessageQueue(), MessageQueueClear()
*/
OS_RESULT MessageQueueDelete(MESSAGE_QUEUE *MessageQueue);

/*
    OS_RESULT MessageQueueAddFromISR(MESSAGE_QUEUE *MessageQueue, void *AdditionalData, UINT32 MessageCode)

	Description: This method attempts to add the specified data to the MESSAGE_QUEUE.  This method
    can be called from an ISR.

	Blocking: Potentially

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void *AdditionalData - A pointer to data which will be copied into the MESSAGE_QUEUE.
        Only the pointer will be copied into the MESSAGE_QUEUE, and not the data it is point to.
  
        UINT32 MessageCode - A 32-bit wide value.  This could be used to denote what the
        void *AdditionalData is pointing to, or just used as the data in the MESSAGE_QUEUE.

	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
    	- USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the UINT32 MessageCode argument.

	See Also:
		- MessageQueueRemoveFromISR(), MessageQueuePeekFromISR()
*/
OS_RESULT MessageQueueAddFromISR(	MESSAGE_QUEUE *MessageQueue

									#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
										, void *AdditionalData
									#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                        , UINT32 MessageCode
                                    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                    );

/*
    OS_RESULT MessageQueueRemoveFromISR(MESSAGE_QUEUE *MessageQueue, void **AdditionalData, UINT32 *MessageCode)

	Description: This method will remove the next available message from the MESSAGE_QUEUE.  The 
    size of the MESSAGE_QUEUE will be decreased by 1.  To delete the data pointed to by AdditionalData,
    the method MessageQueueFreeData() should be called if required.  This method can be called from
    an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void **AdditionalData - This is where the data written during MessageQueueAdd() will be copied to.
   
        UINT32 *MessageCode - An optional message code associated with AdditionalData will be copied here.
 
	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors and the data was copied, an error value otherwise.

	Notes:
        - USING_MESSAGE_QUEUE_REMOVE_FROM_ISR_METHOD  inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the UINT32 MessageCode argument.

	See Also:
		- MessageQueueWaitForData(), MessageQueueAdd(), MessageQueuePeek(), MessageQueueFreeData()
*/
OS_RESULT MessageQueueRemoveFromISR(MESSAGE_QUEUE *MessageQueue

									#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
										, void **AdditionalData
									#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                        , UINT32 *MessageCode
                                    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                    );

/*
    OS_RESULT MessageQueuePeekFromISR(MESSAGE_QUEUE *MessageQueue, void **AdditionalData, UINT32 *MessageCode)

	Description: This method will read the next set of data out from the MESSAGE_QUEUE, but keep the
    references to the data in the MESSAGE_QUEUE.  The size of the MESSAGE_QUEUE remains the same after
    calling this method.  This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void **AdditionalData - This is where the data written during MessageQueueAdd() will be copied to.
   
        UINT32 *MessageCode - An optional message code associated with AdditionalData will be copied here.
 
	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors and the data was copied, an error value otherwise.

	Notes:
        - USING_MESSAGE_QUEUE_PEEK_FROM_ISR_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.
  
		- USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the void *AdditionalData argument.
 
		- USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA inside of RTOSConfig.h must
          be defined as a 1 to use the UINT32 MessageCode argument.

	See Also:
		- MessageQueueRemoveFromISR()
*/
OS_RESULT MessageQueuePeekFromISR(  MESSAGE_QUEUE *MessageQueue
	
                                    #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                                        , void **AdditionalData
                                    #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                        , UINT32 *MessageCode
                                    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                    );

/*
    UINT32 MessageQueueGetSizeFromISR(MESSAGE_QUEUE *MessageQueue)

	Description: This method will return the current size of the MESSAGE_QUEUE.  This is how
    many items are stored in the MESSAGE_QUEUE which can be read.  This method can be called
    from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
        UINT32 - The number of items in the MESSAGE_QUEUE which can be read.  If there is an error the
        value MESSAGE_QUEUE_INVALID_SIZE is returned.
        
	Notes:
		- USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueIsEmptyFromISR()
*/
UINT32 MessageQueueGetSizeFromISR(MESSAGE_QUEUE *MessageQueue);

/*
    BOOL MessageQueueIsEmptyFromISR(MESSAGE_QUEUE *MessageQueue)

	Description: This method will check to see if the MESSAGE_QUEUE is empty or not.
    This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
        BOOL - TRUE if the MESSAGE_QUEUE is empty, FALSE otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_IS_EMPTY_FROM_ISR_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueGetSizeFromISR()
*/
BOOL MessageQueueIsEmptyFromISR(MESSAGE_QUEUE *MessageQueue);

/*
    OS_RESULT MessageQueueClearFromISR(MESSAGE_QUEUE *MessageQueue)

	Description: This method clears out the MESSAGE_QUEUE so that MessageQueueGetSize() returns 0 or
    MessageQueueIsEmpty() returns TRUE.  If any data exists in the MESSAGE_QUEUE and a MessageQueueFreeMethod
    was specified when calling CreateMessageQueue(), the data in the MESSAGE_QUEUE will be passed to
    the MessageQueueFreeMethod method.  This method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_CLEAR_FROM_ISR_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueFreeDataFromISR(), MessageQueueDeleteFromISR()
*/
OS_RESULT MessageQueueClearFromISR(MESSAGE_QUEUE *MessageQueue);

/*
    OS_RESULT MessageQueueFreeDataFromISR(MESSAGE_QUEUE *MessageQueue, void *Data)

	Description: This method will free the data associated with the MESSAGE_QUEUE if
    a MessageQueueFreeMethod method was specified when calling CreateMessageQueue().
    This method can be called on the data read from MessageQueueRemove().  This
    method can be called from an ISR.

	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

        void *Data - The data to free.
	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

        - USING_MESSAGE_QUEUE_ADDITIONAL_DATA inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- MessageQueueRemoveFromISR()
*/
OS_RESULT MessageQueueFreeDataFromISR(MESSAGE_QUEUE *MessageQueue, void *Data);

/*
    OS_RESULT MessageQueueDeleteFromISR(MESSAGE_QUEUE *MessageQueue)

	Description: This method will delete a MESSAGE_QUEUE.  In order to do this (MESSAGE_WUEUE*)NULL
    needs to have been passed in for the MessageQueue argument during the call to CreateMessageQueue().
    Prior to deleting the MESSAGE_QUEUE an equivalent call to MessageQueueClear() is made to
    safely delete all data contained in the MESSAGE_QUEUE.  This method can be called from an ISR.
 
	Blocking: No

	User Callable: Yes

	Arguments:
		MESSAGE_QUEUE *MessageQueue -  A Valid MESSAGE_QUEUE created with a call to CreateMessageQueue().

	Returns: 
		OS_RESULT - OS_SUCCESS if the operation had no errors, an error value otherwise.

	Notes:
		- USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD inside of RTOSConfig.h must
          be defined as a 1 to use this method.

	See Also:
		- CreateMessageQueueFromISR(), MessageQueueClearFromISR()
*/
OS_RESULT MessageQueueDeleteFromISR(MESSAGE_QUEUE *MessageQueue);

#endif // end of #ifndef MESSAGE_QUEUE_H