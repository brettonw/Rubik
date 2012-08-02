//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	10/08/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "dictionary node.h"

//------------------------------------------------------------------------------
DNode		*DNode::pool = 0;																												//	the dictionary node pool

//------------------------------------------------------------------------------
DNode::DNode (char k)																														//	constructor
{																																								//	begin
	key = k;																																			//	store the key
	value = 0;																																		//	zero the value
	high = low = next = 0;																												//	zero the tree pointers
}																																								//	end

//------------------------------------------------------------------------------
DNode::~DNode (void)																														//	destructor
{																																								//	begin
	if (next)																																			//	if there is a sub-tree in the chain
		delete next;																																//	delete it
	if (low)																																			//	if there is a sub-tree going down
		delete low;																																	//	delete it
	if (high)																																			//	if there is a sub-tree going up
		delete high;																																//	delete it
}																																								//	end

//------------------------------------------------------------------------------
void	DNode::PreAllocate (int number)																						//	pre-allocate intersection nodes
{																																								//	begin
	DNode	**table = new DNode*[number];																						//	make a table up for node pointers
	for (int i = 0; i < number; i++)																							//	loop over the entire size of the table
		table[i] = new DNode (0);																										//	allocating empty nodes
	for (i = 0; i < number; i++)																									//	loop over the table again
		delete table[i];																														//	deleting all the nodes
	delete[] table;																																//	free the table
}																																								//	end

//------------------------------------------------------------------------------
