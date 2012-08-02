//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	05/08/96
//
//------------------------------------------------------------------------------

#ifndef	PRIORITY_NODE
#define	PRIORITY_NODE

//------------------------------------------------------------------------------
#include "list node.h"

//------------------------------------------------------------------------------
class PriorityNode : public ListNode																						//	priority node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			value;																												//	the value of the node
	public:																																				//	members available externally
				PriorityNode (void) {}																									//	constructor
virtual	~PriorityNode (void);																										//	destructor
				Real			Value (void);																									//	return the value of the node
				void			Modify (Real delta);																					//	modify the value of the node
};																																							//	end

//------------------------------------------------------------------------------
inline	Real	PriorityNode::Value (void)																				//	return the value of the node
{																																								//	begin
	return value;																																	//	return the value
}																																								//	end

//------------------------------------------------------------------------------
inline	void	PriorityNode::Modify (Real delta)																	//	modify the value of the node
{																																								//	begin
	value += delta;																																//	modify the value
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	PRIORITY_NODE