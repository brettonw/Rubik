//------------------------------------------------------------------------------
//	state mgr.cpp
//	copyright ©1997 qualia, inc. All rights reserved.
//
//	02/12/97	DS	created State Manager
//	02/17/97	DS	cleaned up for crixa dropin
//	02/18/97	DS	modified state logic to support better looping
//	02/21/97	DS	added counter logic support
//	03/09/97	DS	using queues instead of lists to assure in-order execution
//
//------------------------------------------------------------------------------

/*
DESCRIPTION:

The State Manager maintains a list of states for the level, controlled by the
"level.lsm" file that describes a state table.

Game conditions are signaled by various Triggers in the Device hierarchy.
The State Manager watches for particular combinations of Triggers. When
a combination is matched, the State Manager sends its own Triggers to
special devices (such as "end of level").

FUNCTION:

The State Manager's Trigger() function is called by gTrigger (modified to pass
the information) with the TrigID, PiecePtr, EventCode, and EventMsg. This is
checked against an internal list of "exit state triggers". When all of the
triggers have been matched, the State Manager advances to the next state.
It also queues up a separate list of "broadcast triggers" to send through
gTrigger. The broadcast triggers are sent when the Stage Manager's Step()
function is called.
	
*/

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "state mgr.h"
#include "level file.h"
#include "cpnode.h"
#include "debugger.h"

//------------------------------------------------------------------------------
StateMgr	*gStateMgr = 0;																													// allocation of global 

//------------------------------------------------------------------------------
StateMgr::StateMgr (void)	: Device (0)																						// constructor
{
	curQ=0;																																					// initialize dis

	LevFile	lev("StateMgr");																												// open our level subfile
	TxtBuffer	txt(&lev);																														// use text buffer version of the subfile

	if (!ReadStateFile(txt))
	{
		DEBUG_SOFT("Unable to parse State file. Check file contents.\n");
		exit(1);
	}
	DEBUG_SOFT("\ngStateMgr: Starting in [%s]\n",start->TheStateName() );
}


//------------------------------------------------------------------------------
void StateMgr::Trigger (int trigID, const PiecePtr &p, int evtCode, int evtMsg)	// trigger callback
{
	CondNode* node = current->FindMatch(trigID, evtCode, evtMsg);									// look for matching condition node
	if (node)																																			// hey, found a matching condition!
	{
		DEBUG_SOFT("gStateMgr: [%s] -> ",current->TheStateName());

		// queue up the send triggers
		CPNode *cnode = new CPNode(node,p);																					// create a pointer to the condition node
		queueList[1-curQ].Enqueue(cnode);																						// add the pointer to the queue list

		// advance to next state
		if (node->IsClearAll())																											// should all conditions be cleared when exiting?
			current->ResetState();																										// yep, so reset the state conditions
		else
			node->ResetConditions();																									// nope, reset just this one
			
		current = (StateNode*) node->TheNextState();																// go to next state
		DEBUG_SOFT("[%s]\n",current->TheStateName());	
	}
}

//------------------------------------------------------------------------------
void StateMgr::Step (void)																											// step callback to release queued triggers
{

	CPNode *node = (CPNode *) queueList[curQ].Dequeue();													// get the first element
	while (node)
	{
		node->SendTriggers();																												// send all da triggers!
		node = (CPNode *) queueList[curQ].Dequeue();
	}
	curQ=1-curQ;																																	// flip the queue around
}

//------------------------------------------------------------------------------
bool StateMgr::ReadStateFile (TxtBuffer &textin)
{
	cstr	stateIn;																																	// used for input...text buffer passes references 
	cstr	stateOut;										
	cstr	temp;
	int		trigID, eventCode, eventMsg;
	int		junk;

	while (textin.Current()<textin.Length())
	{
		textin >> stateIn;																														// read the state
		StateNode* snodeIN = Fetch(stateIn);																					// get the named state

		textin >> temp;																																// read the delimiter

		if (strcmp(temp,"->"))																												// if not found, then parse error
		{
			DEBUG_SOFT("first delimiter not found\n");
			return FALSE;
		}

		CondNode* cnode = snodeIN->AddNewCondition();																	// add a condition node to the state

		bool read = TRUE;																															// read loop flag

		textin >> temp;																																// *** tokenize.cpp rev2/15/97 needed ***
		while (read)
		{
			int num = sscanf(temp,"%d %d %d %d",&trigID,&eventCode,&eventMsg,&junk); 		// read in the trigger
			cnode->AddCondition(trigID,eventCode,eventMsg);															// add condition to this cnode
			if (num!=3)																																	// if not three arguments read, then parse error
			{
				DEBUG_SOFT("condition triggers bad format\n");
				return FALSE;			
			}

			textin >> temp;																															// read the next token

			if (strcmp(temp,"%")==0)																										// check for "% num" counter specifier
			{
				textin >> junk;
				if (junk < 1)
				{
					DEBUG_SOFT("bad count specifier\n");
					return FALSE;
				}
				cnode->SetCounter(junk);
				textin >> temp;
			}

			if (strcmp(temp,"->")==0)																										// the normal behavior is to clear just the condition that was met
			{
				cnode->SetClearAll(FALSE);																								// when state exited, just this cnode will be cleared
				read = FALSE;
			}

			if (strcmp(temp,">>")==0)																										// to wipe all conditions
			{
				cnode->SetClearAll(TRUE);																									// when state exited, statenode will nuke all conditions that accumulate
				read = FALSE;
			}
		}

		textin >> stateOut;																														// get the output state name
		StateNode *snodeOUT = Fetch(stateOut); 																				// get the named state, but don't do anything with it

		cnode->SetNextState(snodeOUT);																								// point the cnode to the next state

		textin >> temp;
		while (temp[0]!='#')
		{																																							// read in a bunch of conditions
			int num = sscanf(temp,"%d %d %d %d",&trigID,&eventCode,&eventMsg,&junk); 		// read in the trigger
			cnode->AddTrigger(trigID,eventCode,eventMsg);																// add condition to this cnode
			if (num!=3)																																	// if not three arguments read, then parse error
			{	
				DEBUG_SOFT("exit triggers bad format\n");
				return FALSE;			
			}
			textin >> temp;
		}
	}

	textin.Reset();																																	// reset file to beginning so we can get the first state
	textin >> stateIn;																															// read the first state again
	start = Find(stateIn);																													// set start StateNode*
	current = start;																																// set current StateNode*

	return TRUE;																																		// made it this far, so everything must be OK
}

//------------------------------------------------------------------------------
StateMgr::~StateMgr (void)
{
	stateNodeList.Reset();																													// delete all state nodes
	queueList[0].Reset();
	queueList[1].Reset();
}

//------------------------------------------------------------------------------

