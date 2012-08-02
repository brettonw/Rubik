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
#include "list.h"

//------------------------------------------------------------------------------
List::List (void)																																//	constructor
{																																								//	begin
	sentinel.next = &sentinel;																										//	sentinel points forward to itself
	sentinel.previous = &sentinel;																								//	sentinel points backward to itself
}																																								//	end

//------------------------------------------------------------------------------
List::~List (void)																															//	destructor
{																																								//	begin
	Reset ();																																			//	make the list empty
}																																								//	end

//------------------------------------------------------------------------------
void	List::InsertAfter (ListNode *node, ListNode *after)												//	insert a new node after a specified node
{																																								//	begin
	if (!after)																																		//	if the requested node is null
		after = &sentinel;																													//	do it with the sentinel
	node->previous = after;																												//	point backward to the requested node
	node->next = after->next;																											//	point forward to the requested node's successor
	after->next = node;																														//	the requested node points forward to the new node
	node->next->previous = node;																									//	the requested node's successor points backward to the new node
}																																								//	end

//------------------------------------------------------------------------------
void	List::InsertBefore (ListNode *node, ListNode *before)											//	insert a new node before a specified node
{																																								//	begin
	if (!before)																																	//	if the requested node is null
		before = &sentinel;																													//	do it with the sentinel
	node->next = before;																													//	point forward to the requested node
	node->previous = before->previous;																						//	point backward to the requested node's predecessor
	before->previous = node;																											//	the requested node points backward to the new node
	node->previous->next = node;																									//	the requested node's predecessor points forward to the new node
}																																								//	end

//------------------------------------------------------------------------------
void	List::Reset (void)																												//	revert the list to an empty state
{																																								//	begin
	ListNode	*current = Head ();																									//	get the head of the list
	while (current)																																//	while there are elements in the list
	{																																							//	begin
		Remove (current);																														//	remove the current node
		delete current;																															//	delete it
		current = Head ();																													//	set the current node to be the head again
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
