//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/13/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "piece node.h"

//------------------------------------------------------------------------------
Stack		PieceNode::pool;																												//	the piece node pool

//------------------------------------------------------------------------------
PieceNode::PieceNode (const PiecePtr &pc, int flgs)															//	constructor
{																																								//	begin
	piece = pc;																																		//	copy the piece ptr
	flags = flgs;																																	//	copy the flags
}																																								//	end

//------------------------------------------------------------------------------
PieceNode::~PieceNode (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	*PieceNode::operator new (size_t size)																		//	free store allocation
{																																								//	begin
	ListNode	*node = pool.Pop ();
	if (!node)
		return ::operator new (size);
	return node;
}																																								//	end

//------------------------------------------------------------------------------
void	PieceNode::operator delete (void *ptr, size_t size)												//	free store recovery
{																																								//	begin
	pool.Push ((PieceNode *) ptr);
}																																								//	end

//------------------------------------------------------------------------------
void	PieceNode::PreAllocate (int number)																				//	pre-allocate piece nodes
{																																								//	begin
	PieceNode	**table = new PieceNode*[number];
	for (int i = 0; i < number; i++)
		table[i] = new PieceNode;
	for (i = 0; i < number; i++)
		delete table[i];
	delete[] table;
}																																								//	end

//------------------------------------------------------------------------------
