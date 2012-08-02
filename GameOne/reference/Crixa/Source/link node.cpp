//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/30/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "link node.h"

//------------------------------------------------------------------------------
Stack		LinkNode::pool;																													//	the link node pool

//------------------------------------------------------------------------------
LinkNode::~LinkNode (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	*LinkNode::operator new (size_t size)																			//	free store allocation
{																																								//	begin
	ListNode	*node = pool.Pop ();
	if (!node)
		return ::operator new (size);
	return node;
}																																								//	end

//------------------------------------------------------------------------------
void	LinkNode::operator delete (void *ptr, size_t size)												//	free store recovery
{																																								//	begin
	pool.Push ((ListNode *) ptr);
}																																								//	end

//------------------------------------------------------------------------------
void	LinkNode::PreAllocate (int number)																				//	pre-allocate link nodes
{																																								//	begin
	LinkNode	**table = new LinkNode*[number];
	for (int i = 0; i < number; i++)
		table[i] = new LinkNode (0);
	for (i = 0; i < number; i++)
		delete table[i];
	delete[] table;
}																																								//	end

//------------------------------------------------------------------------------
