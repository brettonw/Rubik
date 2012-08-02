//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//
//	02/07/97	JB		added gnatty loading, playing, targeting code
//	02/18/97	JB		removed trigger (127) and commander reading
//	02/21/97	JB		team information
//	02/24/97	DS		moved eksekaren all dead trigger to 128 32 0 from 127 0 0
//
//------------------------------------------------------------------------------

/*
EKSEKAREN RULES:

bugs need to regen from assigned jenny
time cost associated with each bug for construction
cap on the number of bugs that a jenny can have active
jenny will not start production until there is room available

*/

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "eksekaren.h"
#include "player node.h"
#include "level file.h"
#include "text buffer.h"
#include "thug.h"
#include "gnatty.h"
#include "math utility.h"
#include "sprite array mgr.h"
#include "referee.h"
#include "trigger array.h"
#include "get parameters.h"
#include "debugger.h"
#include "team identification.h"

//------------------------------------------------------------------------------
Semaphore	Eksekaren::semaphore;																									//	lock on the planner
Planner		*Eksekaren::planner;																									//	pointer to the planner for the eksekaren

//------------------------------------------------------------------------------
Eksekaren	*gEksekaren;																													//	global eksekaren player
int				gEksekarenCount;																											//	how many eksekaren there are

//------------------------------------------------------------------------------
void	Eksekaren::InstallBarracks (TxtBuffer &txt)
{
	txt >> barrackCount;
	for (int i = 0; i < barrackCount; i++)
	{
		cstr	barrackName;
		int		index,
					type,
					maxKids,
					bugType;
		Real	radius,
					orient,
					armor,
					x,
					y;
		txt >> barrackName																												//	determined by TIFF name
				>> index >> type >> radius >> orient >> armor >> maxKids >> bugType		//	user specified
				>> x >> y;																														//	determined by TIFF position
		orient = DegreesToRadians (orient);																				//	convert degrees to radians
		Point_2d location (x, y);																									//	make a point of the location
		if (index == 0)																														//	little dummy check to make sure index 0 is reserved for bugs with no parent
			DEBUG_STR ("Hey, Level Designer!  %s has an index of 0!  Bo-gus!\n", barrackName);
		Barrack *barrack = new Barrack (this, index, type, radius, orient, armor,	//	make a new barrack
													 location, barrackName, maxKids, bugType);					//	make a new barrack
		barracks.InsertAfter (new PlayerNode (barrack));													//	insert into the barrack list
	}
}

//------------------------------------------------------------------------------
void	Eksekaren::InstallBugs (TxtBuffer &txt)
{
	cstr	nextBug;
	txt >> nextBug;
	while (strcmp (nextBug, "end"))
	{
		int		owner,
					type;
		Real	orient,
					x,
					y;
		cstr	thisBug = nextBug;

		txt >> owner >> type >> orient >> x >> y >> nextBug;												//	get the type, orientation, and location
		orient = DegreesToRadians (orient);																					//	convert the orientation
		Point_2d location (x, y);																										//	create a point from the coordinates
		EksBug *eksBug = 0;

		if (strcmp (thisBug, "hunter") == 0)
			eksBug = new Thug (location, orient, type);																//	create the hunter player
		else if (strcmp (thisBug, "gnatty") == 0)
			eksBug = new Gnatty (type, ParmReal (GD_RADIUS), orient,									//	create a new gnatty
									 location, gSpriteMgr.Fetch (ParmString (GD_SPRITE)));				//	create a new gnatty (continued)

		if (eksBug)
		{
			Barrack *barrack = FindBarrack (owner);
			if (barrack)
				barrack->AttachBug (eksBug);
			else
				independentEks.InsertAfter (new PlayerNode (eksBug));
		}
		else
			DEBUG_SOFT ("Err: %s %d %d %.2f %.2f %.2f\n", thisBug, owner, type, orient, x, y);
	}
}

//------------------------------------------------------------------------------
Eksekaren::Eksekaren (void) : Player ("Eksekaren")															//	constructor
{																																								//	begin
	LevFile	lev ("Eksekaren");
	planner = new Planner (2);
	TxtBuffer txt (&lev);																													//	load the eksekaren file
	trigger = 128;	//	temporary!  get rid of it! 
	gEksekarenCount = 0;
	InstallBarracks (txt);
	InstallBugs (txt);
	SetTeam (TEAM_EKSEKAREN);
}																																								//	end

//------------------------------------------------------------------------------
Eksekaren::~Eksekaren (void)																										//	destructor
{																																								//	begin
	delete planner;
}																																								//	end

//------------------------------------------------------------------------------
Bool	Eksekaren::Play (void)																										//	play one frame
{																																								//	begin
	PlayerNode	*node = (PlayerNode *) barracks.Head (),
							*killNode;
	while (node)
	{
		Bool ok = node->player->Play ();
		if (!ok)
			killNode = node;
		node = (PlayerNode *) barracks.Next (node);
		if (!ok)
		{
			barracks.Remove (killNode);
			delete (killNode);
		}
	}
	node = (PlayerNode *) independentEks.Head ();
	while (node)
	{
		Bool ok = node->player->Play ();
		if (!ok)
			killNode = node;
		node = (PlayerNode *) independentEks.Next (node);
		if (!ok)
		{
			independentEks.Remove (killNode);
			delete (killNode);
		}
	}
	return TRUE;																																	//	the eksekaren don't go away
}																																								//	end

//------------------------------------------------------------------------------
void	Eksekaren::SetTarget (const PiecePtr &targ)																//	set the thug target
{																																								//	begin
	if (gEksekarenCount <= 0)
		gTrigger->Trigger (trigger, PiecePtr (0), 32, 0);														//	128,32,0 is ALL EKS DEAD 
	else
	{
		target = targ;
		SetTargetBarracks ();
		SetTargetIndependent ();
	}
}																																								//	end

//------------------------------------------------------------------------------
void	Eksekaren::SetTargetBarracks (void)
{
	PlayerNode *node = (PlayerNode *) barracks.Head ();
	while (node)
	{
		EksBug *eksBug = (EksBug *) node->player;
		eksBug->SetTarget (target);
		node = (PlayerNode *) barracks.Next (node);
	}
}

//------------------------------------------------------------------------------
void	Eksekaren::SetTargetIndependent (void)
{
	PlayerNode *node = (PlayerNode *) independentEks.Head ();
	while (node)
	{
		EksBug *eksBug = (EksBug *) node->player;
		eksBug->SetTarget (target);
		node = (PlayerNode *) independentEks.Next (node);
	}
}

//------------------------------------------------------------------------------
Planner	*Eksekaren::GrabPlanner (void)																					//	return the planner if it is not already in use
{																																								//	begin
	if (semaphore.Grab ())																												//	if grabbing the semaphore is successful
		return planner;																															//	return the planner
	return 0;																																			//	return zero to indicate that the planner is in use
}																																								//	end

//------------------------------------------------------------------------------
void	Eksekaren::ReleasePlanner (void)																					//	free up the planner for someone else to use
{																																								//	begin
	semaphore.Release ();																													//	release the semaphore
}																																								//	end

//------------------------------------------------------------------------------
void	Eksekaren::AcceptChildren (List &children)
{
	PlayerNode *node = (PlayerNode *) children.Head ();
	while (node)
	{
		children.Remove (node);
		((EksBug *) node->player)->SetParent (0);
		independentEks.InsertAfter (node);
		node = (PlayerNode *) children.Head ();
	}
	SetTargetIndependent ();
}

//------------------------------------------------------------------------------
Barrack *Eksekaren::FindBarrack (int owner)
{
	PlayerNode *node = (PlayerNode *) barracks.Head ();
	while (node)
	{
		Barrack *barrack = (Barrack *) node->player;
		if (barrack->Identification () == owner)
			return barrack;
		node = (PlayerNode *) barracks.Next (node);
	}
	return 0;
}

//------------------------------------------------------------------------------
