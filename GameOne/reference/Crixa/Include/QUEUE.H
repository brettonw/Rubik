//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	06/14/96
//
//------------------------------------------------------------------------------

#ifndef	QUEUE
#define	QUEUE

//------------------------------------------------------------------------------
#include "list.h"

//------------------------------------------------------------------------------
class	Queue : public List																												//	queue class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
virtual	~Queue (void);																													//	destructor
				ListNode	*Dequeue (void);																							//	dequeue the head element
				void			Enqueue (ListNode *node);																			//	add a new element to the tail of the list
};																																							//	end

//------------------------------------------------------------------------------
inline	ListNode	*Queue::Dequeue (void)																				//	dequeue the head element
{																																								//	begin
	ListNode	*head = Head ();																										//	get the head of the list
	if (head)																																			//	if the head is valid
		Remove (head);																															//	remove it
	return head;																																	//	return it
}																																								//	end

//------------------------------------------------------------------------------
inline	void	Queue::Enqueue (ListNode *node)																		//	add a new element to the tail of the list
{																																								//	begin
	InsertBefore (node);																													//	insert the element before the sentinel
}																																								//	end

//------------------------------------------------------------------------------

#endif	//QUEUE