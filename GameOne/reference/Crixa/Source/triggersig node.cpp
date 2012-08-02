//-----------------------------------------------------------------------------
//  triggersig node.cpp 
//	copyright © 1997 qualia incorporated. all rights reserved
// 
//  02/12/97	DS	stores a trigger, provides testing
//	02/17/97	DS	cleaned up for crixa dropin
//	02/24/97	DS	add option of triggering a piece
//	03/07/97	DS	modification of TRIGID = 0 for fall through behavior
//								logic can be cleaned up now that this is the case
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//-----------------------------------------------------------------------------
#include "triggersig node.h"
#include "local human.h"

//-----------------------------------------------------------------------------
bool TrSigNode::TestCondition (int trig, int code, int msg)											// test a condition
{
	if (!wasMatched)																															// if not previously matched...
	{
		bool result = (msg == evtMsg) && (code == evtCode);													// see if both evt match
		wasMatched = (trigID == 0) ? TRUE : (result && (trig == trigID));						// if trigID == 0, then everything matches
		if (wasMatched)
		{
			DEBUG_SOFT("           match [%3d %3d %3d]\n",trig,code,msg);
			if (trigID == 0)
				DEBUG_SOFT("           (was wildcard)\n");
		}
	}
	return wasMatched;																														// return the condition of the match
}																																							

//-----------------------------------------------------------------------------
void TrSigNode::SendTrigger (void)																							// broadcast trigger
{
	DEBUG_SOFT("           send_c  [%3d %3d %3d]\n",trigID,evtCode,evtMsg);
	gTrigger->Trigger(trigID,gLocal->GetCrixa(),evtCode,evtMsg);
}

//-----------------------------------------------------------------------------
void TrSigNode::SendTrigger (PiecePtr &pc)																				// broadcast trigger
{
	DEBUG_SOFT("           send_p  [%3d %3d %3d]\n",trigID,evtCode,evtMsg);
	gTrigger->Trigger(trigID,pc,evtCode,evtMsg);
}

//-----------------------------------------------------------------------------
TrSigNode::~TrSigNode (void)
{
}

//-----------------------------------------------------------------------------
 