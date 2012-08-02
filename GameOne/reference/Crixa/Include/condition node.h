//-----------------------------------------------------------------------------
//  condition node.h 
//	copyright © 1997 qualia incorporated. all rights reserved
// 
//	02/12/97	DS	Stores trigger signatures for comparison
// 								Stores trigger signature to be sent, too.
//								Used by state mgr
//	02/17/97	DS	cleaned up for dropin to crixa
//	02/21/97	DS	added counter logic
//	02/24/97	DS	changed SendTriggers to pass along pieceptr
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef CONDNODE
#define CONDNODE

//-----------------------------------------------------------------------------

#include "list node.h"
#include "queue.h"
#include "triggersig node.h"

//-----------------------------------------------------------------------------
class CondNode : public ListNode
{
	protected:																																		// available to derived classes only
		Queue			exitL;																														// list of triggers to match against
		Queue			sendL;																														// list of triggers to send if match
		ListNode *nextState;																												// next state to go to if matched
		bool			clearall;																													// when the condition is met, statenode should clear everything
	
		int				startCount;																												// # events before triggering
		int				counter_;																													// counter
public:																																					// available to everyone
							CondNode (void);																									// constructor
		virtual		~CondNode (void);																									// destructor
		void			AddCondition (int trigID, int evtCode, int evtMsg);								// add a trigger condition
		void			AddTrigger (int trigID, int evtCode, int evtMsg);									// add a success trigger
		void			SetNextState (ListNode* snode);																		// set the next state to go if match
		ListNode*	TheNextState (void) { return nextState; }													// return the next state
		bool			TestConditions (int trigID, int evtCode, int evtMsg);							// compare condition
		void			SendTriggers (PiecePtr &p);																				// send triggers out with this piece
		void			ResetConditions (void);																						// reset condition trigger states
		void			SetClearAll (bool flag) { clearall = flag; }											// set the type of condition clearing 
		bool			IsClearAll (void) { return clearall; }														// return the cleartype
		void			SetCounter (int num) { counter_ = startCount = num; }							// set the condition counter
};

//-----------------------------------------------------------------------------
inline CondNode::CondNode ()																										// constructor
{
	nextState = 0;																																// no next state by default
	clearall = FALSE;																															// the default clear state
	counter_ = startCount = 1;																										// default one shot
}

//-----------------------------------------------------------------------------

inline void CondNode::AddCondition (int trig, int code, int msg)								// add a conditional trigger to this node
{
	ListNode *node = new TrSigNode (trig,code,msg);																// create a new condition node
	exitL.Enqueue (node);																													// add it to the list
}
    
//-----------------------------------------------------------------------------

inline void CondNode::AddTrigger (int trig, int code, int msg)									// add a trigger to broadcast to this node
{
	TrSigNode *node = new TrSigNode (trig,code,msg);															// create a new trigger node
	sendL.Enqueue (node);																													// add it to the list
}
  
//-----------------------------------------------------------------------------
inline void CondNode::SetNextState (ListNode *snode)														// set the next state to go for this condition node
{
	nextState = snode;																														// stores the destination state if this condition node is met
}

//-----------------------------------------------------------------------------
inline void CondNode::ResetConditions (void)																		// reset the condition flags for this node
{
	TrSigNode *node = (TrSigNode*) exitL.Head();
	while (node)
	{
		node->ResetCondition();
		node = (TrSigNode*) exitL.Next(node);
	}
	counter_ = startCount;																												// reset the countdown
}

//-----------------------------------------------------------------------------
#endif