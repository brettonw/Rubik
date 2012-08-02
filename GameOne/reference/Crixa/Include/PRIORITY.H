//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/08/96
//
//------------------------------------------------------------------------------

#ifndef	PRIORITY
#define	PRIORITY

//------------------------------------------------------------------------------
#include "priority node.h"

//------------------------------------------------------------------------------
class Priority																																	//	priority queue base class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Priority (void) {}																											//	constructor
virtual	~Priority (void);																												//	destructor
virtual	void			Insert (PriorityNode *node) = 0;															//	insert a node into the queue
virtual	void			Remove (PriorityNode *node) = 0;															//	remove a node from the queue
virtual	void			Changed (PriorityNode *node) =0;															//	inform the queue that a node has changed value
virtual	void			Reset (void) = 0;																							//	reset the heap to initial state
virtual	void			Modify (Real delta) = 0;																			//	modify all the nodes by a certain amount
virtual	PriorityNode			*Peek (void) = 0;																			//	return a pointer to the minimum valued node
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PRIORITY