//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//
//	Revisions:			02/07/97:	added the list of gnattys to the eks roster. (JB)
//									02/11/97: ripping stuff apart.
//
//------------------------------------------------------------------------------

#ifndef	EKSEKAREN
#define	EKSEKAREN

//------------------------------------------------------------------------------
#include "list.h"
#include "player.h"
#include "piece ptr.h"
#include "semaphore.h"
#include "planner.h"
#include "text buffer.h"
#include "barrack.h"

//------------------------------------------------------------------------------
class Eksekaren : public Player																									//	the ENEMY!
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int							trigger;																								//	trigger id for the all dead trigger
				int							barrackCount;
				List						barracks;
				List						independentEks;
				PiecePtr				target;
static	Semaphore				semaphore;																							//	lock on the planner
static	Planner					*planner;																								//	pointer to the planner for the eksekaren
	public:																																				//	members available externally
				Eksekaren (void);																												//	constructor
virtual	~Eksekaren (void);																											//	destructor
				void						InstallBarracks (TxtBuffer &txt);												//	load and install the jennys
				void						InstallBugs (TxtBuffer &txt);														//	load and install the bugs
virtual	Bool						Play (void);																						//	play one frame
				void						SetTarget (const PiecePtr &target);											//	set the thug target
				void						SetTargetBarracks (void);
				void						SetTargetIndependent (void);
static	Planner					*GrabPlanner (void);																		//	return the planner if it is not already in use
static	void						ReleasePlanner (void);																	//	free up the planner for someone else to use
				Barrack					*FindBarrack (int owner);
				void						AcceptChildren (List &children);
};																																							//	end

//------------------------------------------------------------------------------
extern	Eksekaren	*gEksekaren;																									//	global eksekaren player
extern	int				gEksekarenCount;																							//	how many eksekaren there are

//------------------------------------------------------------------------------

#endif	//	EKSEKAREN
