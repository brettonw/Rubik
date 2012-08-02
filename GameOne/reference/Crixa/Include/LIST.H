//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/14/96
//
//------------------------------------------------------------------------------

#ifndef	LIST
#define	LIST

//------------------------------------------------------------------------------
#include "list node.h"

//------------------------------------------------------------------------------
class	List																																			//	list class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				ListNode	sentinel;																											//	list sentinel
	public:																																				//	members available externally
				List (void);																														//	constructor
virtual	~List (void);																														//	destructor
				ListNode	*Head (void);																									//	return the head of the list
				ListNode	*Next (ListNode *node);																				//	return the next node in the list
				ListNode	*Previous (ListNode *node);																		//	return the previous node in the list
				void			InsertAfter (ListNode *node, ListNode *after = 0);						//	insert a new node after a specified node
				void			InsertBefore (ListNode *node, ListNode *before = 0);					//	insert a new node before a specified node
				void			Remove (ListNode *node);																			//	remove a node
				void			Reset (void);																									//	revert the list to an empty state
};																																							//	end

//------------------------------------------------------------------------------
inline	ListNode	*List::Head (void)																						//	return the head of the list
{																																								//	begin
	return (sentinel.next == &sentinel) ? 0 : sentinel.next;											//	return the head, 0 if next is the sentinel
}																																								//	end

//------------------------------------------------------------------------------
inline	ListNode	*List::Next (ListNode *node)																	//	return the next node in the list
{																																								//	begin
	if (node)																																			//	if the node is valid
		return (node->next == &sentinel) ? 0 : node->next;													//	return next, 0 if next is the sentinel
	return 0;																																			//	return 0 since the node was not valid
}																																								//	end

//------------------------------------------------------------------------------
inline	ListNode	*List::Previous (ListNode *node)															//	return the previous node in the list
{																																								//	begin
	if (node)																																			//	if the node is valid
		return (node->previous == &sentinel) ? 0 : node->previous;									//	return previous, 0 if previous is the sentinel
	return 0;																																			//	return 0 since the node was not valid
}																																								//	end

//------------------------------------------------------------------------------
inline	void	List::Remove (ListNode *node)																			//	remove a node
{																																								//	begin
	node->next->previous = node->previous;																				//	successor points back to the predecessor
	node->previous->next = node->next;																						//	predecessor points forward to the successor
}																																								//	end

//------------------------------------------------------------------------------

#endif	//LIST