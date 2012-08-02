//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	04/24/96
//
//	Implementation Note:
//		This file implements a naive priority queue using a simple doubly
//		linked list. If the performance of this class is not adequate, a
//		binomial heap or fibonacci heap based queue will be substituted.
//		These heaps don't generally support raise-key operations well, and
//		the added code complexity makes it worth-while to try this approach
//		first.  -- BW --
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "naive.h"

//------------------------------------------------------------------------------
Naive::~Naive (void)																														//	destructor
{																																								//	begin
	list.Reset ();																																//	reset the list
}																																								//	end

//------------------------------------------------------------------------------
void	Naive::Insert (PriorityNode *node)																				//	insert a node into the queue
{																																								//	begin
	if (node->Value () == INFINITY)																								//	if the node value is intended to indicate it is lowest priority
		list.InsertBefore (node);																										//	insert it at the end of the list
	else																																					//	otherwise, it should be treated well
	{																																							//	begin
		PriorityNode	*current = (PriorityNode *) list.Head ();											//	start with a pointer to the min node
		while (current && (current->Value () < node->Value ()))											//	loop until the node value is less than the tested one, or we hit the end of the list
			current = (PriorityNode *) list.Next (current);														//	advance to the next node for comparison
		list.InsertBefore (node, current);																					//	insert the new node before the current one
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Naive::Remove (PriorityNode *node)																				//	remove a node from the queue
{																																								//	begin
	list.Remove (node);																														//	remove the node from the list
}																																								//	end

//------------------------------------------------------------------------------
void	Naive::Changed (PriorityNode *node)																				//	inform the queue that a node has changed value
{																																								//	begin
	list.Remove (node);																														//	remove the node from the queue
	Insert (node);																																//	then reinstall it
}																																								//	end

//------------------------------------------------------------------------------
void	Naive::Reset (void)																												//	reset the queue to initial state
{																																								//	begin
	list.Reset ();																																//	reset the list
}																																								//	end

//------------------------------------------------------------------------------
void	Naive::Modify (Real delta)																								//	modify all the nodes by a certain amount
{																																								//	begin
	PriorityNode	*current = (PriorityNode *) list.Head ();												//	start with a pointer to the min node
	while (current)																																//	loop until we hit the end of the list
	{																																							//	begin
		current->Modify (delta);																										//	propogate the modify message
		current = (PriorityNode *) list.Next (current);															//	advance to the next node for comparison
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
PriorityNode	*Naive::Peek (void)																								//	return a pointer to the minimum valued node
{																																								//	begin
	return (PriorityNode *) list.Head ();																					//	return a pointer to the list head
}																																								//	end

//------------------------------------------------------------------------------
	