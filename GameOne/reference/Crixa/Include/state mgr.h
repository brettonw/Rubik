//-----------------------------------------------------------------------------
//	State Manager
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	02/12/97	DS	Created!
//	02/17/97	DS	Prepared for dropin to Crixa
//	02/18/97	DS	Modified to provide name of current state, also
//								changed implementation to looping in state logic
//
//-----------------------------------------------------------------------------

#ifndef STATEMGR
#define STATEMGR

//-----------------------------------------------------------------------------
#include "file.h"
#include "queue.h"
#include "condition node.h"
#include "state node.h"
#include "disk file.h"
#include "text buffer.h"

//-----------------------------------------------------------------------------
class StateMgr : public Device																									// Maintains a list of states
{
	private:
		StateNode *Fetch (cstr name);																								// find the state node with this name
		StateNode *Find (cstr name);																								// variation that doesn't create a new node
		bool			ReadStateFile (TxtBuffer &textin);																// read the state file, return TRUE if success
		StateNode *start;																														// starting state 
		StateNode *current;																													// current state in table
		List			stateNodeList;																										// list of state nodes

		int				curQ;
		Queue			queueList[2];																											// triggers to be broadcast on step
	public:
		 					StateMgr (void);																									// constructor
		virtual		~StateMgr (void);																									// destructor
		void			Trigger (int trigID, const PiecePtr &p, int evtCode, int evtMsg);	// test triggers for exit conditions
		void			Step (void);																											// send queued response triggers, if any
		const cstr	CurrentStateName (void);																				// return a pointer to the state name string
};

//-----------------------------------------------------------------------------
extern	StateMgr	*gStateMgr;																										// external declaration for users

//-----------------------------------------------------------------------------
inline const cstr StateMgr::CurrentStateName (void)															// returns a pointer to the current statename
{
	return current->TheStateName();																								// danger: screwy user could mess up the real string
}


//------------------------------------------------------------------------------
inline StateNode *StateMgr::Fetch (cstr name)																			// finds (and allocates if necessary) a statenode
{
	StateNode* node = (StateNode*) stateNodeList.Head();
	while (node)
	{
		if (strcmp(node->TheStateName(),name)==0)																			// if found the node
			return node;																																// then we found it! return the node
		node = (StateNode*) stateNodeList.Next(node);																	// advance to next node in list
	}

	node = new StateNode(name);																											// didn't find it, so make a new entry
	stateNodeList.InsertAfter(node);
	return node;																																		// here it is
}

//------------------------------------------------------------------------------
inline StateNode *StateMgr::Find (cstr name)																			// finds a statenode by name
{
	StateNode* node = (StateNode*) stateNodeList.Head();
	while (node)
	{
		if (strcmp(node->TheStateName(),name)==0)																			// if found the node
			return node;																																// then we found it! return the node
		node = (StateNode*) stateNodeList.Next(node);																	// advance to next node in list
	}
	return 0;																																				// not found, so return null pointer
}

//-----------------------------------------------------------------------------
#endif