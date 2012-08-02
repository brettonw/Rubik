//-----------------------------------------------------------------------------
//  triggersig node.h 
//	copyright © 1997 qualia incorporated. all rights reserved
// 
//  02/12/97	DS	store trigger information (used by Condition Nodes)
//	02/17/97	DS	cleaned up for crixa dropin
//	02/24/97	DS	added SendTrigger with pc
//	03/09/97	DS	using queues to enqueue
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef TRIGGERSIG
#define TRIGGERSIG

//-----------------------------------------------------------------------------
#include "trigger array.h"
#include "debugger.h"

//-----------------------------------------------------------------------------
class TrSigNode : public ListNode
{
	protected:
		bool			wasMatched;																												// flag to check for matches
		int				trigID;																														// triggerID to match
		int				evtCode;																													// eventCode to match
		int				evtMsg;																														// eventMessage to match
	public:
							TrSigNode (int trig, int code, int msg);													// constructor
		virtual		~TrSigNode (void);																								// destructor
		bool			TestCondition (int trigID, int evtCode, int evtMsg);							// compare condition
		void			ResetCondition (void);																						// reset the comparison flag
		void			SendTrigger (void);																								// send all triggers away, using CrixaShip
		void			SendTrigger (PiecePtr &pc);																				// send all triggers away
};

//------------------------------------------------------------------------------
inline void TrSigNode::ResetCondition (void)
{ 
	wasMatched = FALSE; 
}

//------------------------------------------------------------------------------
inline TrSigNode::TrSigNode (int trig, int code, int msg)
{
	wasMatched = FALSE;																														// in-body assignment
	trigID = trig;																																// should move to initlist
	evtCode = code;
	evtMsg = msg;
}


//------------------------------------------------------------------------------
#endif