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

#include "DoubleLinkedList.h"
#include "DoubleLinkedListConfig.h"

void InitializeDoubleLinkedListHead(DOUBLE_LINKED_LIST_HEAD *Head)
{
	Head->Beginning = (DOUBLE_LINKED_LIST_NODE*)NULL;
	Head->End = (DOUBLE_LINKED_LIST_NODE*)NULL;
}

void InitializeDoubleLinkedListNode(DOUBLE_LINKED_LIST_NODE *Node)
{
	Node->Data = (void*)NULL;

	Node->NextNode = Node->PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;
}

#if (USING_INSERT_NODE_AT_BEGINNING_OF_DOUBLE_LINKED_LIST_METHOD == 1)
	void InsertNodeAtBeginningOfDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node)
	{
		if (Head->Beginning == (DOUBLE_LINKED_LIST_NODE*)NULL)
		{
			Head->Beginning = Head->End = Node;
			Node->NextNode = Node->PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;
		}
		else
		{
			Node->PreviousNode = NULL;
			Node->NextNode = Head->Beginning;
			Head->Beginning->PreviousNode = Node;
			Head->Beginning = Node;
		}
	}
#endif // end of #if (USING_INSERT_NODE_AT_BEGINNING_OF_DOUBLE_LINKED_LIST_METHOD == 1)

#if (USING_INSERT_NODE_AT_END_OF_DOUBLE_LINKED_LIST_METHOD == 1)
    void InsertNodeAtEndOfDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node)
    {
        if(Head->End == (DOUBLE_LINKED_LIST_NODE*)NULL)
        {
            Head->End = Head->Beginning = Node;
            Node->NextNode = Node->PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;
        }
        else
        {
            Node->NextNode = NULL;
            Node->PreviousNode = Head->End;
            Head->End = Head->End->NextNode = Node;
        }
    }
#endif // end of #if (USING_INSERT_NODE_AT_END_OF_DOUBLE_LINKED_LIST_METHOD == 1)

#if (USING_REMOVE_NODE_FROM_DOUBLE_LINKED_LIST_METHOD == 1)
    DOUBLE_LINKED_LIST_NODE *RemoveNodeFromDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node)
    {
        if(Node->NextNode != (DOUBLE_LINKED_LIST_NODE*)NULL)
        {
            Node->NextNode->PreviousNode = Node->PreviousNode;
        }

        if(Node->PreviousNode != (DOUBLE_LINKED_LIST_NODE*)NULL)
        {
            Node->PreviousNode->NextNode = Node->NextNode;
        }

        if(Head->Beginning == Node)
        {
            if(Head->End == Node)
                Head->Beginning = Head->End = (DOUBLE_LINKED_LIST_NODE*)NULL;
            else
                Head->Beginning = Node->NextNode;
        }
        else
        {
            if(Head->End == Node)
                Head->End = Node->PreviousNode;
        }

        Node->NextNode = Node->PreviousNode = (DOUBLE_LINKED_LIST_NODE*)NULL;

        return Node;
    }
#endif // end of #if (USING_REMOVE_NODE_FROM_DOUBLE_LINKED_LIST_METHOD == 1)

#if (USING_DOUBLE_LINKED_LIST_HAS_DATA_METHOD == 1)
    BOOL DoubleLinkedListHasData(DOUBLE_LINKED_LIST_HEAD *Head)
    {
        return (BOOL)(Head->Beginning != (DOUBLE_LINKED_LIST_NODE*)NULL);
    }
#endif // end of #if (USING_DOUBLE_LINKED_LIST_HAS_DATA_METHOD == 1)

#if (USING_DOUBLE_LINKED_LIST_GET_SIZE_METHOD == 1)
	UINT32 DoubleLinkedListGetSize(DOUBLE_LINKED_LIST_HEAD *Head)
	{
		UINT32 Size;
		DOUBLE_LINKED_LIST_NODE *Iterator;

		if (Head->Beginning == (DOUBLE_LINKED_LIST_NODE*)NULL)
			return 0;

		Iterator = Head->Beginning;

		Size = 0;

		do
		{
			Iterator = Iterator->NextNode;
			Size++;
		} while (Iterator != (DOUBLE_LINKED_LIST_NODE*)NULL);

		return Size;
	}
#endif // end of #if (USING_DOUBLE_LINKED_LIST_GET_SIZE_METHOD == 1)

#if (USING_DOUBLE_LINKED_LIST_ITERATE_TO_NEXT_NODE_METHOD == 1)
	DOUBLE_LINKED_LIST_NODE *DoubleLinkedListIterateToNextNode(DOUBLE_LINKED_LIST_NODE *Node)
	{
		if(Node == (DOUBLE_LINKED_LIST_NODE*)NULL)
			return (DOUBLE_LINKED_LIST_NODE*)NULL;

		return Node->NextNode;
	}
#endif // end of #if (USING_DOUBLE_LINKED_LIST_ITERATE_TO_NEXT_NODE_METHOD == 1)
