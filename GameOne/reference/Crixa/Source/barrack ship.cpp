//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	02/13/97
//
//	02/28/97	JB		using a real visual sequence instead of a hacked visual.
//	03/07/97	JB		using boss explosion sequence on destruction.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "barrack ship.h"
#include "sprite array mgr.h"
#include "time sequence.h"
#include "sequence visual.h"
#include "circle.h"
#include "get parameters.h"
#include "boss explosion sequence.h"
#include "view.h"

//------------------------------------------------------------------------------
const cstr kSleepingSuffix = "_s";
const cstr kBirthingSuffix = "_b";

//------------------------------------------------------------------------------
BarrackShip::BarrackShip (Player *owner, Real rad, CPoint tloc, Real rloc, cstr baseName) ://	constructor
Ship (owner)																																		//	constructor (continued)
{
	int length = strlen (baseName);																								//	length of the name
	cstr workingName = new char [length + 3];																			//	allocate a buffer plus room for adjustment
	strcpy (workingName, baseName);																								//	copy the name
	strcat (workingName, kSleepingSuffix);																				//	append the sleeping suffix
	SArrayPtr sleepSpr = gSpriteMgr.Fetch (workingName);													//	get the sprite
	strcpy (workingName, baseName);																								//	copy the name
	strcat (workingName, kBirthingSuffix);																				//	append the birthing suffix
	SArrayPtr birthSpr = gSpriteMgr.Fetch (workingName);													//	get the sprite
	delete[] workingName;																													//	nuke the buffer
	switchSeq = SeqPtr (new FixedSwitchSeq (Point_2d (ZERO, ZERO),								//	make a fixed switch sequence
											new TimeSeq (sleepSpr, TRUE), new TimeSeq (sleepSpr, FALSE)));//	continued

	rDamping = R(-2.0e-1);																												//	set a starting value for rotational damping
	body = new Body (tloc, rloc, INFINITY, INFINITY);															//	create a fixed body
	visual = new SeqVisual (body, switchSeq);																			//	attach the switch sequence to the body
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius

	stun = ZERO;																																	//	set the current stun time to 0
	stunMax = ZERO;																																//	set the stun max to a hundredth of a second
	dead = FALSE;																																	//	the ship is not dead
	numNozzles = 0;																																//	set the number of nozzle points to 0
	thrSprite = gSpriteMgr.Fetch ("eksthr");
	resources = ParmInteger (RESOURCE_SIZE);
}

//------------------------------------------------------------------------------
BarrackShip::~BarrackShip (void)
{
}

//------------------------------------------------------------------------------
void	BarrackShip::Explode (const PiecePtr &piece)
{
	gView->Install (new BossExplSeq (this, R(2.0), 3));														//	build the explosion
}

//------------------------------------------------------------------------------
