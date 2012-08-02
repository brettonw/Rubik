//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/08/96
//
//------------------------------------------------------------------------------

#ifndef	NAIVE
#define	NAIVE

//------------------------------------------------------------------------------
#include "priority.h"
#include "list.h"

//------------------------------------------------------------------------------
class Naive : public Priority																										//	naive priority queue
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				List			list;																													//	list for maintaining the priority
	public:																																				//	members available externally
				Naive (void) {}																													//	constructor
virtual	~Naive (void);																													//	destructor
virtual	void			Insert (PriorityNode *node);																	//	insert a node into the queue
virtual	void			Remove (PriorityNode *node);																	//	remove a node from the queue
virtual	void			Changed (PriorityNode *node);																	//	inform the queue that a node has changed value
virtual	void			Reset (void);																									//	reset the heap to initial state
virtual	void			Modify (Real delta);																					//	modify all the nodes by a certain amount
virtual	PriorityNode			*Peek (void);																					//	return a pointer to the minimum valued node
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	NAIVE