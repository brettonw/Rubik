//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/14/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "stack.h"

//------------------------------------------------------------------------------
Stack::Stack (void)																															//	constructor
{																																								//	begin
	head = 0;																																			//	start with an empty list
}																																								//	end

//------------------------------------------------------------------------------
Stack::~Stack (void)																														//	destructor
{																																								//	begin
	Reset ();																																			//	clear the stack
}																																								//	end

//------------------------------------------------------------------------------
ListNode	*Stack::Pop (void)																										//	return the head of the list
{																																								//	begin
	ListNode	*tmp = head;																												//	get the head of the list
	if (tmp)																																			//	if the head was valid
		head = head->next;																													//	advance the head to the next node in the list
	return tmp;																																		//	return the previous head value
}																																								//	end

//------------------------------------------------------------------------------
void	Stack::Push (ListNode *node)																							//	insert a new node at the head of the list
{																																								//	begin
	node->next = head;																														//	point the next at the current head node
	head = node;																																	//	make the new node the head of the list
}																																								//	end

//------------------------------------------------------------------------------
void	Stack::Reset (void)																												//	revert the list to an empty state
{																																								//	begin
	while (head)																																	//	loop while there are items in the list
	{																																							//	begin
		ListNode	*tmp = head;																											//	get the head
		head = head->next;																													//	advance the head to the next node
		delete tmp;																																	//	free the previous head
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
