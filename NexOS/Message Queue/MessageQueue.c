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

#include "MessageQueue.h"
#include "../Kernel/Memory.h"
#include "CriticalSection.h"

#if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)
	extern TASK * volatile gCurrentTask;
#endif // end of #if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)

/*
	This method removes the MESSAGE_QUEUE_NODE at the head of the queue.
	Then updates the queue node pointers to point to the next node.
	Decrements the Queue size by 1, and returns the node.

	This method assumes there is at least 1 MESSAGE_QUEUE_NODE in the MESSAGE_QUEUE.
*/
#if(USING_MESSAGE_QUEUE_REMOVE_FROM_ISR_METHOD == 1 || USING_MESSAGE_QUEUE_CLEAR_FROM_ISR_METHOD == 1)
    static MESSAGE_QUEUE_NODE *MessageQueueRemoveNode(MESSAGE_QUEUE *MessageQueue)
    {
        MESSAGE_QUEUE_NODE *Node;

        // point to the first piece of data
        Node = MessageQueue->MessageQueueData;

        // now make the start of data in the message queue point to the second node (if it exists)
        MessageQueue->MessageQueueData = (MESSAGE_QUEUE_NODE*)MessageQueue->MessageQueueData->NextMessageQueueNode;

        #if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)
            // now decrement the size by one
            MessageQueue->MessageQueueSize--;
        #endif // end of #if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)

        return Node;
    }
#endif // end of #if(USING_MESSAGE_QUEUE_REMOVE_FROM_ISR_METHOD == 1 || USING_MESSAGE_QUEUE_CLEAR_FROM_ISR_METHOD == 1)

static void MessageQueueAddNode(MESSAGE_QUEUE *MessageQueue, MESSAGE_QUEUE_NODE *Node)
{
	// make the new node next node point to null
	Node->NextMessageQueueNode = (void*)NULL;

	// on a size of 1 it is a special case
	if (MessageQueue->MessageQueueData == (MESSAGE_QUEUE_NODE*)NULL)
	{
		// this is the first node of data now
		MessageQueue->MessageQueueData = Node;
	}
	else
	{
		MESSAGE_QUEUE_NODE *NodeIterator;

		// start at the beginning since we know there is at least one node
		NodeIterator = MessageQueue->MessageQueueData;

		// now iterate through and put the new node at the end of the queue
		while (NodeIterator->NextMessageQueueNode != (void*)NULL)
			NodeIterator = (MESSAGE_QUEUE_NODE*)NodeIterator->NextMessageQueueNode;

		// now point to the new data
		NodeIterator->NextMessageQueueNode = (void*)Node;
	}

	#if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)
		// now that the data was added lets increase the size
		MessageQueue->MessageQueueSize++;
	#endif // end of #if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)
}

MESSAGE_QUEUE *CreateMessageQueue(	MESSAGE_QUEUE *MessageQueue

                                    #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)
                                        , BOOL(*MessageQueueFreeMethod)(void *)
                                    #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)

                                    )
{
	MESSAGE_QUEUE *NewMessageQueue;

    #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
        if (MessageQueue != (MESSAGE_QUEUE*)NULL)
            if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                return (MESSAGE_QUEUE*)NULL;

        #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)
            if (MessageQueueFreeMethod != (BOOL(*)(void*))NULL)
                if (AddressValid((OS_WORD)MessageQueueFreeMethod) == FALSE)
                    return (MESSAGE_QUEUE*)NULL;
        #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)
    #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

	NewMessageQueue = MessageQueue;

	if (NewMessageQueue == (MESSAGE_QUEUE*)NULL)
	{
		if ((NewMessageQueue = AllocateMemory(sizeof(MESSAGE_QUEUE))) == (MESSAGE_QUEUE*)NULL)
		{
			return (MESSAGE_QUEUE*)NULL;
		}
	}

	#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)
		NewMessageQueue->MessageQueueFreeMethod = MessageQueueFreeMethod;
	#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)

	#if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)
		NewMessageQueue->MessageQueueSize = 0;
	#endif // end of #if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)

	#if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)
		InitializeDoubleLinkedListHead(&NewMessageQueue->BlockedListHead);
	#endif // end of #if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)

    #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
        NewMessageQueue->BlockDeleteCount = 0;
    #endif // end of #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)

	NewMessageQueue->MessageQueueData = (MESSAGE_QUEUE_NODE*)NULL;

	return NewMessageQueue;
}

#if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)
	OS_RESULT MessageQueueWaitForData(	MESSAGE_QUEUE *MessageQueue

										#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
											, void **AdditionalData
										#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                        #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                            , UINT32 *MessageCode
                                        #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                        #if (USING_TASK_DELAY_TICKS_METHOD == 1)
											, INT32 TimeoutInTicks
										#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

                                        )
	{
		BOOL QueueIsEmpty;
		OS_RESULT Result;

        #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

		EnterCritical();

		// first check if there is any data to begin with
		QueueIsEmpty = MessageQueueIsEmptyFromISR(MessageQueue);
        
		// if the queue is empty we need to check if we can block on the resource
		if (QueueIsEmpty == TRUE)
		{
			// the queue has no data
			// we can only block if no TASK is already blocking on the queue
			if (DoubleLinkedListHasData(&MessageQueue->BlockedListHead) == TRUE)
			{
				// a TASK is already blocking on this queue, we cannot block
				ExitCritical();

				return OS_RESOURCE_IN_USE;
			}

			// now block since there was no data
			OS_PlaceTaskOnBlockedList(gCurrentTask, &MessageQueue->BlockedListHead, &gCurrentTask->TaskNodeArray[PRIMARY_TASK_NODE], BLOCKED, TRUE);

			// now we wait for the specified time and then read once awoken
			#if (USING_TASK_DELAY_TICKS_METHOD == 1)
				if (TimeoutInTicks > 0)
				{
					// place on timer list
					OS_AddTaskToDelayQueue(gCurrentTask, &gCurrentTask->TaskNodeArray[SECONDARY_TASK_NODE], TimeoutInTicks, FALSE);
				}
			#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)

            #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
                MessageQueue->BlockDeleteCount++;
                
                #if(USING_DELETE_TASK == 1)
                    gCurrentTask->DeleteBlockCounter = &MessageQueue->BlockDeleteCount;
                #endif // end of #if(USING_DELETE_TASK == 1)
            #endif // end of #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
    
			SurrenderCPU();
            
            #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
                MessageQueue->BlockDeleteCount--;
                
                #if(USING_DELETE_TASK == 1)
                    gCurrentTask->DeleteBlockCounter = (UINT32*)NULL;
                #endif // end of #if(USING_DELETE_TASK == 1)
            #endif // end of #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)

			#if (USING_TASK_DELAY_TICKS_METHOD == 1)
				if (TimeoutInTicks > 0)
				{
					gCurrentTask->DelayInTicks = 0;

					// if we timed out just exit with an error
					if (gCurrentTask->DelayInTicks == TASK_TIMEOUT_DONE_VALUE)
					{
						// we timed out
						ExitCritical();

						return OS_TASK_TIMEOUT;
					}
				}
			#endif // end of #if (USING_TASK_DELAY_TICKS_METHOD == 1)
		}
		
		// try and read the data out
		Result = MessageQueueRemoveFromISR(	MessageQueue

											#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
												, AdditionalData
											#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                                , MessageCode
                                            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                            );

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)

#if (USING_MESSAGE_QUEUE_ADD_METHOD == 1)
	OS_RESULT MessageQueueAdd(	MESSAGE_QUEUE *MessageQueue

								#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
									, void *AdditionalData
								#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                    , UINT32 MessageCode
                                #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                )
	{
		OS_RESULT Result;

		EnterCritical();

		Result = MessageQueueAddFromISR(	MessageQueue

											#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
												, AdditionalData
											#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                                , MessageCode
                                            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                            );  

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_ADD_METHOD == 1)

#if (USING_MESSAGE_QUEUE_REMOVE_METHOD == 1)
	OS_RESULT MessageQueueRemove(	MESSAGE_QUEUE *MessageQueue

									#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
										, void **AdditionalData
									#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                        , UINT32 *MessageCode
                                    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                    )
	{
		OS_RESULT Result;

		EnterCritical();

		Result = MessageQueueRemoveFromISR(	MessageQueue

											#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                                                , AdditionalData
											#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
								
                                            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                                , MessageCode
                                            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                            );

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_REMOVE_METHOD == 1)

#if (USING_MESSAGE_QUEUE_PEEK_METHOD == 1)
    OS_RESULT MessageQueuePeek(MESSAGE_QUEUE *MessageQueue

							#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
								, void **AdditionalData
							#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                , UINT32 *MessageCode
                            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                            )
    {
		OS_RESULT Result;

		EnterCritical();

		Result = MessageQueuePeekFromISR(	MessageQueue

											#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                                                , AdditionalData
											#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
								
                                            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                                , MessageCode
                                            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                            );

		ExitCritical();

		return Result;
    }
#endif // end of #if (USING_MESSAGE_QUEUE_PEEK_METHOD == 1)

#if (USING_MESSAGE_QUEUE_GET_SIZE_METHOD == 1)
	UINT32 MessageQueueGetSize(MESSAGE_QUEUE *MessageQueue)
	{
		UINT32 Size;

		EnterCritical();

		Size = MessageQueueGetSizeFromISR(MessageQueue);

		ExitCritical();

		return Size;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_GET_SIZE_METHOD == 1)

#if (USING_MESSAGE_QUEUE_IS_EMPTY_METHOD == 1)
	BOOL MessageQueueIsEmpty(MESSAGE_QUEUE *MessageQueue)
	{
		BOOL IsEmpty;

		EnterCritical();

		IsEmpty = MessageQueueIsEmptyFromISR(MessageQueue);

		ExitCritical();

		return IsEmpty;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_IS_EMPTY_METHOD == 1)

#if (USING_MESSAGE_QUEUE_CLEAR_METHOD == 1)
	OS_RESULT MessageQueueClear(MESSAGE_QUEUE *MessageQueue)
	{
		OS_RESULT Result;

		EnterCritical();

		Result = MessageQueueClearFromISR(MessageQueue);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_CLEAR_METHOD == 1)

#if (USING_MESSAGE_QUEUE_DELETE_METHOD == 1)
	OS_RESULT MessageQueueDelete(MESSAGE_QUEUE *MessageQueue)
	{
		OS_RESULT Result;

		EnterCritical();

		Result = MessageQueueDeleteFromISR(MessageQueue);

		ExitCritical();

		return Result;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_DELETE_METHOD == 1)


OS_RESULT MessageQueueAddFromISR(	MESSAGE_QUEUE *MessageQueue

                                    #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                                        , void *AdditionalData
                                    #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                        , UINT32 MessageCode
                                    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                    )
{
    MESSAGE_QUEUE_NODE *Node;

    #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
        if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
            return OS_INVALID_ARGUMENT_ADDRESS;

        #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
            if (RAMAddressValid((OS_WORD)AdditionalData) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
    #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

    Node = OS_AllocateMemory(sizeof(MESSAGE_QUEUE_NODE));

    if (Node == (MESSAGE_QUEUE_NODE*)NULL)
        return OS_ALLOCATE_MEMORY_FAILED;

    #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
        // copy the message code over
        Node->MessageCode = MessageCode;
    #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

    // copy the data over
    #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
        Node->AdditionalData = AdditionalData;
    #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

    // now add the node to the queue
    MessageQueueAddNode(MessageQueue, Node);

    #if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)
        // if there is a TASK blocking on this for data, wake it.
        if (DoubleLinkedListHasData(&MessageQueue->BlockedListHead) == TRUE)
        {
            UINT32 i;

            // there was a TASK blocking for data
            TASK *TaskHandle = (TASK*)MessageQueue->BlockedListHead.Beginning->Data;

            // remove all the nodes from any lists they were on
            for (i = 0; i < NUMBER_OF_INTERNAL_TASK_NODES; i++)
                OS_RemoveTaskFromList(&TaskHandle->TaskNodeArray[i]);

            // now add it to the ready queue
            if (OS_AddTaskToReadyQueue(TaskHandle) == TRUE)
                SurrenderCPU();
        }
    #endif // end of #if (USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD == 1)

    return OS_SUCCESS;
}

#if (USING_MESSAGE_QUEUE_REMOVE_FROM_ISR_METHOD == 1)
	OS_RESULT MessageQueueRemoveFromISR(MESSAGE_QUEUE *MessageQueue
	
										#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
											, void **AdditionalData
										#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                        #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                            , UINT32 *MessageCode
                                        #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                        )
	{
		MESSAGE_QUEUE_NODE *Node;

        #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                if (RAMAddressValid((OS_WORD)AdditionalData) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                if (RAMAddressValid((OS_WORD)MessageCode) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
        #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

		if (MessageQueue->MessageQueueData == (MESSAGE_QUEUE_NODE*)NULL)
			return OS_RESOURCE_HAS_NO_DATA;

		// remove the first node
		Node = MessageQueueRemoveNode(MessageQueue);

		// copy the data over
		#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
			*AdditionalData = Node->AdditionalData;
		#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
	
        #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
            // copy the message code over
            *MessageCode = Node->MessageCode;
        #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

		// now clear our the MESSAGE_QUEUE_NODE itself!
		if (OS_ReleaseMemory((void*)Node) == FALSE)
			return OS_FREE_MEMORY_FAILED;

		return OS_SUCCESS;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_REMOVE_FROM_ISR_METHOD == 1)
    
#if (USING_MESSAGE_QUEUE_PEEK_FROM_ISR_METHOD == 1)
	OS_RESULT MessageQueuePeekFromISR(MESSAGE_QUEUE *MessageQueue
	
										#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
											, void **AdditionalData
										#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

                                        #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                                            , UINT32 *MessageCode
                                        #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

                                        )
	{
        #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;

            #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                if (RAMAddressValid((OS_WORD)AdditionalData) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            #endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

            #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
                if (RAMAddressValid((OS_WORD)MessageCode) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;
            #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
        #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

        // if it is empty, we cannot read anything
		if (MessageQueue->MessageQueueData == (MESSAGE_QUEUE_NODE*)NULL)
			return OS_RESOURCE_HAS_NO_DATA;

		// copy the data over for the peek
		#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
			*AdditionalData = MessageQueue->MessageQueueData->AdditionalData;
		#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
	
        #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)
            // copy the message code over for the peek
            *MessageCode = MessageQueue->MessageQueueData->MessageCode;
        #endif // end of #if (USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA == 1)

		return OS_SUCCESS;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_PEEK_FROM_ISR_METHOD == 1)

#if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)
	UINT32 MessageQueueGetSizeFromISR(MESSAGE_QUEUE *MessageQueue)
	{
        #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                return MESSAGE_QUEUE_INVALID_SIZE;
        #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

		return MessageQueue->MessageQueueSize;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD == 1)

#if (USING_MESSAGE_QUEUE_IS_EMPTY_FROM_ISR_METHOD == 1)
	BOOL MessageQueueIsEmptyFromISR(MESSAGE_QUEUE *MessageQueue)
	{
        #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                return TRUE;
        #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

		return (BOOL)(MessageQueue->MessageQueueData == (MESSAGE_QUEUE_NODE*)NULL);
	}
#endif // end of #if (USING_MESSAGE_QUEUE_IS_EMPTY_FROM_ISR_METHOD == 1)

#if (USING_MESSAGE_QUEUE_CLEAR_FROM_ISR_METHOD == 1)
	OS_RESULT MessageQueueClearFromISR(MESSAGE_QUEUE *MessageQueue)
	{
		MESSAGE_QUEUE_NODE *Node;

        #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
            if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                return OS_INVALID_ARGUMENT_ADDRESS;
        #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

		// now we just go through and iterate to delete the data
		while (MessageQueue->MessageQueueData != (MESSAGE_QUEUE_NODE*)NULL)
		{
			Node = MessageQueueRemoveNode(MessageQueue);

			#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
                #if (USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD == 1)
					if (MessageQueue->MessageQueueFreeMethod != (BOOL(*)(void*))NULL)
					{
						// call the method to free the data since a method was supplied
						if (MessageQueue->MessageQueueFreeMethod(Node->AdditionalData) == FALSE)
							return OS_FREE_MEMORY_FAILED;
					}
				#endif // end of #if (USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD == 1)
			#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

			// now clear our the MESSAGE_QUEUE_NODE itself!
			if (OS_ReleaseMemory((void*)Node) == FALSE)
				return OS_FREE_MEMORY_FAILED;
		}

		return OS_SUCCESS;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_CLEAR_FROM_ISR_METHOD == 1)

#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)
	#if (USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD == 1)
		OS_RESULT MessageQueueFreeDataFromISR(MESSAGE_QUEUE *MessageQueue, void *Data)
		{
			OS_RESULT Result = OS_SUCCESS;

            #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
                if (RAMAddressValid((OS_WORD)MessageQueue) == FALSE)
                    return OS_INVALID_ARGUMENT_ADDRESS;

                if (AddressInHeap((OS_WORD)Data) == FALSE)
                    return OS_RESOURCE_NOT_IN_OS_HEAP;
            #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)

			// if the free method is valid, call it
			if (MessageQueue->MessageQueueFreeMethod != (BOOL(*)(void*))NULL)
			{
				// call the method to free the data since a method was supplied
				if (MessageQueue->MessageQueueFreeMethod(Data) == FALSE)
					Result = OS_FREE_MEMORY_FAILED;
			}

			return Result;
		}

		#if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)
			OS_RESULT MessageQueueFreeData(MESSAGE_QUEUE *MessageQueue, void *Data)
			{
				OS_RESULT Result = OS_SUCCESS;

				EnterCritical();

				Result = MessageQueueFreeDataFromISR(MessageQueue, Data);

				ExitCritical();

				return Result;
			}
		#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD == 1)
	#endif // end of #if (USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD == 1)
#endif // end of #if (USING_MESSAGE_QUEUE_ADDITIONAL_DATA == 1)

#if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
	OS_RESULT MessageQueueDeleteFromISR(MESSAGE_QUEUE *MessageQueue)
	{
        OS_RESULT Result;
        
        #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
            if (AddressInHeap((OS_WORD)MessageQueue) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
        #endif // end of #if (USING_CHECK_MESSAGE_QUEUE_PARAMETERS == 1)
        
        #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
            // is anyone waiting on data from the MESSAGE_QUEUE?
            if(MessageQueue->BlockDeleteCount != 0)
                return OS_RESOURCE_IN_USE;
        #endif // end of #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)
        
        // first we must clear out the MESSAGE_QUEUE
        Result = MessageQueueClearFromISR(MessageQueue);
        
        // if the MESSAGE_QUEUE was successfully cleared, now we can delete it
        if(Result == OS_SUCCESS)
        {
            if(OS_ReleaseMemory((void*)MessageQueue) == FALSE)
                return OS_RESOURCE_NOT_IN_OS_HEAP;
        }

		return Result;
	}
#endif // end of #if (USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD == 1)