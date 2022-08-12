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

#ifndef DOUBLE_LINKED_LIST_H
	#define DOUBLE_LINKED_LIST_H

#include "GenericTypes.h"

/*
	This structure is used link up tasks that are in a list.  It has a pointer
	to the next node in the list, and the previous pointer, this allows removal
	of a node to be implemented in just 2 lines of code.  The void pointer Data
	is used to just hold the address of the data that the node is pointing to.  In this
	case it is the address of the Tasks TCB that data will be pointing to.
*/
struct _DoubleLinkedListNode
{
	struct _DoubleLinkedListNode *NextNode;
	struct _DoubleLinkedListNode *PreviousNode;
	void *Data;
};

typedef struct _DoubleLinkedListNode DOUBLE_LINKED_LIST_NODE;


/*
	This structure is used to point to the beginning and the end of a double linked
	list.  The pointer to the end allows simple insertion of a node into the linked list.
	The pointer to the beginning is just, well, needed for the linked list, instead of having
	a single DOUBLE_LINKED_LIST_NODE pointer to the head.
*/
typedef struct
{
	DOUBLE_LINKED_LIST_NODE *Beginning;
	DOUBLE_LINKED_LIST_NODE *End;
}DOUBLE_LINKED_LIST_HEAD;

/*
	void InitializeDoubleLinkedListHead(DOUBLE_LINKED_LIST_HEAD *Head)

	Description: This method initializes a DOUBLE_LINKED_LIST_HEAD.  This method
    should be called on a DOUBLE_LINKED_LIST_HEAD before any other method.

	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_HEAD *Head - The DOUBLE_LINKED_LIST_HEAD to initialize.
 
	Returns:
		- None

	Notes:
		- None

	See Also:
		- None
*/
void InitializeDoubleLinkedListHead(DOUBLE_LINKED_LIST_HEAD *Head);

/*
	void InitializeDoubleLinkedListNode(DOUBLE_LINKED_LIST_NODE *Node)

	Description: This method initializes a DOUBLE_LINKED_LIST_NODE.  This should
    be called on any DOUBLE_LINKED_LIST_NODE before being added to a DOUBLE_LINKED_LIST_HEAD.

	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_NODE *Node - The DOUBLE_LINKED_LIST_NODE to initialize.
 
	Returns:
		- None

	Notes:
		- None

	See Also:
		- None
*/
void InitializeDoubleLinkedListNode(DOUBLE_LINKED_LIST_NODE *Node);

/*
	void InsertNodeAtEndOfDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node)

	Description: This method adds a DOUBLE_LINKED_LIST_NODE to the specified DOUBLE_LINKED_LIST_HEAD.
    The node is added to the end of the double linked list.

	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_HEAD *Head - The head of double linked list to added the node to.
 
        DOUBLE_LINKED_LIST_NODE *Node - The node of the double linked list to add to.
 
	Returns:
		- None

	Notes:
		- USING_INSERT_NODE_AT_END_OF_DOUBLE_LINKED_LIST_METHOD in DoubleLinkedListConfig.h
          must be defined as a 1 to use this method.

	See Also:
		- InitializeDoubleLinkedListNode(), InsertNodeAtBeginningOfDoubleLinkedList(), RemoveNodeFromDoubleLinkedList()
*/
void InsertNodeAtEndOfDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node);

/*
	void InsertNodeAtBeginningOfDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node)

	Description: This method adds a DOUBLE_LINKED_LIST_NODE to the specified DOUBLE_LINKED_LIST_HEAD.
    The node is inserted at the beginning of the double linked list.

	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_HEAD *Head - The head of double linked list to added the node to.
 
        DOUBLE_LINKED_LIST_NODE *Node - The node of the double linked list to add to.
 
	Returns:
		- None

	Notes:
		- USING_INSERT_NODE_AT_BEGINNING_OF_DOUBLE_LINKED_LIST_METHOD in DoubleLinkedListConfig.h
          must be defined as a 1 to use this method.

	See Also:
		- InitializeDoubleLinkedListNode(), InsertNodeAtEndOfDoubleLinkedList(), RemoveNodeFromDoubleLinkedList()
*/
void InsertNodeAtBeginningOfDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node);

/*
	DOUBLE_LINKED_LIST_NODE *RemoveNodeFromDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node)

	Description: This method removes the specified node from a double linked list.

	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_HEAD *Head - The head of double linked list to remove the node from.
 
        DOUBLE_LINKED_LIST_NODE *Node - The node of the double linked list to remove.
 
	Returns:
		DOUBLE_LINKED_LIST_NODE * - The node which was removed.

	Notes:
		- None

	See Also:
		- InsertNodeAtBeginningOfDoubleLinkedList(), InsertNodeAtEndOfDoubleLinkedList()
*/
DOUBLE_LINKED_LIST_NODE *RemoveNodeFromDoubleLinkedList(DOUBLE_LINKED_LIST_HEAD *Head, DOUBLE_LINKED_LIST_NODE *Node);

/*
	BOOL DoubleLinkedListHasData(DOUBLE_LINKED_LIST_HEAD *Head)

	Description: This method checks to see if the specified DOUBLE_LINKED_LIST_HEAD
    has any data stored in it from a previous call to InsertNodeAtBeginningOfDoubleLinkedList()
    or InsertNodeAtEndOfDoubleLinkedList().

	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_HEAD *Head - The DOUBLE_LINKED_LIST_HEAD to check
        if it has any data associated with it.
 
	Returns:
        BOOL - TRUE is any data is associated with the DOUBLE_LINKED_LIST_HEAD, FALSE otherwise.

	Notes:
		- None

	See Also:
		- InsertNodeAtBeginningOfDoubleLinkedList(), InsertNodeAtEndOfDoubleLinkedList()
*/
BOOL DoubleLinkedListHasData(DOUBLE_LINKED_LIST_HEAD *Head);

/*
	UINT32 DoubleLinkedListGetSize(DOUBLE_LINKED_LIST_HEAD *Head)

	Description: This method gets how many nodes are associated with a DOUBLE_LINKED_LIST_HEAD.
    Nodes of data are added with the methods InsertNodeAtBeginningOfDoubleLinkedList() and 
    InsertNodeAtEndOfDoubleLinkedList().
 
	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_HEAD *Head - The DOUBLE_LINKED_LIST_HEAD to check how many
        nodes are associated in it.
 
	Returns:
		UINT32 - How many nodes are associated in the DOUBLE_LINKED_LIST_HEAD.

	Notes:
		- None

	See Also:
		- InsertNodeAtBeginningOfDoubleLinkedList(), InsertNodeAtEndOfDoubleLinkedList()
*/
UINT32 DoubleLinkedListGetSize(DOUBLE_LINKED_LIST_HEAD *Head);

/*
	DOUBLE_LINKED_LIST_NODE *DoubleLinkedListIterateToNextNode(DOUBLE_LINKED_LIST_NODE *Node)

	Description: This method iterates from the specified DOUBLE_LINKED_LIST_NODE to
    the next DOUBLE_LINKED_LIST_NODE in the double linked list.

	Blocking: No

	User Callable: Yes

	Arguments:
		DOUBLE_LINKED_LIST_NODE *Node - The DOUBLE_LINKED_LIST_NODE to iterate from.
 
	Returns:
		DOUBLE_LINKED_LIST_NODE * - The next node in the list.  If there is no next
        node the value (DOUBLE_LINKED_LIST_NODE*)NULL is returned.

	Notes:
		- None

	See Also:
		- None
*/
DOUBLE_LINKED_LIST_NODE *DoubleLinkedListIterateToNextNode(DOUBLE_LINKED_LIST_NODE *Node);

#endif // end of DOUBLE_LINKED_LIST_H
