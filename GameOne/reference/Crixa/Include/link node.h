//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/30/96
//
//------------------------------------------------------------------------------

#ifndef	LINK_NODE
#define	LINK_NODE

//------------------------------------------------------------------------------
#include "list node.h"
#include "intersection node.h"

//------------------------------------------------------------------------------
class			LinkNode : public ListNode																						//	private structure for maintaining links between active pairs of objects
{																																								//	begin
	private:																																			//	members internal to this class only
static	Stack			pool;																													//	the link node pool
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				INode			*intersection;																								//	pointer to the intersection record for this link
				LinkNode	*pair;																												//	pointer to the pair for this link node
	public:																																				//	members available externally
  			LinkNode (INode *intersection);																					//	constructor
virtual	~LinkNode (void);																												//	destructor
				void			*operator new (size_t size);																	//	free store allocation
				void			operator delete (void *ptr, size_t size);											//	free store recovery
static	void			PreAllocate (int number = 5000);															//	pre-allocate link nodes
};																																							//	end

//------------------------------------------------------------------------------
inline	LinkNode::LinkNode (INode *isect)																				//	constructor
{																																								//	begin
	intersection = isect;																													//	copy the intersection ptr
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	LINK_NODE
