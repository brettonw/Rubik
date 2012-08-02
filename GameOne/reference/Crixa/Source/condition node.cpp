//-----------------------------------------------------------------------------
//  condition node.cpp 
//	copyright © 1997 qualia incorporated. all rights reserved
// 
//  02/12/97	DS	Stores a trigger signature for comparison	by a StateNode
//	02/17/97	DS	cleaned up for crixa dropin
//	02/21/97	DS	added counter logic
// 
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "condition node.h"

//-----------------------------------------------------------------------------
bool CondNode::TestConditions (int trig, int code, int msg)											// test if all conditions are met
{
	TrSigNode *node = (TrSigNode *) exitL.Head();																	// get the starting trigger
	bool result = TRUE;	
	while (node)																																	// if there are conditions
	{
		result &= node->TestCondition(trig,code,msg);																// test a condition for this node
		node = (TrSigNode*) exitL.Next(node);																				// get the next node to test
	}

	if (result) 
	{
		if (--counter_>0)
		{
			DEBUG_STR("           %d to go...\n",counter_);
			result = FALSE;																														// we're not done! decrement counter
			node = (TrSigNode*) exitL.Head();																					// reset the trigger nodes for this condition
			while (node)
			{
				node->ResetCondition();
				node = (TrSigNode*) exitL.Next(node);
			}
		}
	}
	return result;																																// if any conditions were met, result is TRUE
}																																							

//-----------------------------------------------------------------------------
void CondNode::SendTriggers (PiecePtr &pc)																			// triggers sent by this node
{
	TrSigNode *node = (TrSigNode *) sendL.Head();
	while (node)
	{
		node->SendTrigger(pc);
		node = (TrSigNode *) sendL.Next (node);
	}
}

//------------------------------------------------------------------------------
CondNode::~CondNode (void)
{
	exitL.Reset();																																// remove all nodes from list
	sendL.Reset();																																// remove all nodes from list
}

//------------------------------------------------------------------------------
