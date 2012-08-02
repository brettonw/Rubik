//-----------------------------------------------------------------------------
//  state info node 
//  copyright © 1997 qualia incorporated. all rights reserved
// 
//  02/12/97	DS	used to maintain information about states, also to
//								save information useful for building the state table
//	02/17/97	DS 	cleaned up for crixa dropin
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//-----------------------------------------------------------------------------
#include "state node.h"

//-----------------------------------------------------------------------------
StateNode::~StateNode (void)
{
	condL.Reset();																																// delete the contents of the list
}

//-----------------------------------------------------------------------------
