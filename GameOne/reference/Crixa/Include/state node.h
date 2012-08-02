//-----------------------------------------------------------------------------
//  state info node 
//  copyright © 1997 qualia incorporated. all rights reserved
// 
//  created: 	02/12/97	Used to maintain information about states, also to
//											save information useful for building the state table
// 	modified:	02/17/97	cleaned up for dropin to crixa
// 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef	STATENODE
#define	STATENODE

//-----------------------------------------------------------------------------

#include "list node.h"
#include "queue.h"
#include "condition node.h"

//-----------------------------------------------------------------------------
class	StateNode : public ListNode																								
{																																								
	protected:																																	  // available to derived classes only
		char			name[32];																													// unique name of this state
		Queue			condL;																														// list of condition nodes, each containing a list of triggers
	public:																																				// available to everyone
							StateNode (const cstr c);																					// constructor
		virtual		~StateNode (void);																								// destructor			
		CondNode	*AddNewCondition (void);																					// add a condition node to this statenode
		CondNode	*FindMatch (int trigID, int evtCode, int evtMsg);									// (index2) return nonzero condition match 
		cstr			TheStateName (void) { return name; }															// return the name of this state
		void			ResetState (void);																								// reset the condition nodes for this state
};

//-----------------------------------------------------------------------------
inline StateNode::StateNode (const cstr c)																			// create a new StateNode with a name
{
	strcpy (name,c);																															// copy the name
}

//-----------------------------------------------------------------------------

inline CondNode *StateNode::FindMatch (int trigID, int evtCode, int evtMsg)			// return first CondNode that matches
{
	CondNode *node = (CondNode *) condL.Head();																		// get the list of conditions for this node
	while (node)
	{
		if (node->TestConditions (trigID, evtCode, evtMsg))													// ask each condition node
			return node;																															// return node if found
		node = (CondNode*) condL.Next(node);
	}
	return 0;																																			// no luck, so return null pointer
}

//-----------------------------------------------------------------------------
inline void StateNode::ResetState (void)																				// reset the state of all conditions in this node
{
	CondNode *node = (CondNode *) condL.Head();																		// get the list of conditions for this node
	while (node)
	{
		node->ResetConditions();																										// reset the condition of a node
		node = (CondNode*) condL.Next(node);																				// advance to next node
	}
}

//-----------------------------------------------------------------------------
inline CondNode *StateNode::AddNewCondition (void)															// add a new conditional node to this state
{
	CondNode *node = new CondNode();																							// create a new condition node to hold a bunch of triggers
	condL.Enqueue (node);																													// add node to the condition list for this state (arg2 in table)
	return node;																																	// return the CondNode for initializing
}
                                                                               
//-----------------------------------------------------------------------------
#endif