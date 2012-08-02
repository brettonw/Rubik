//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/11/97:	creation. (JB)
//
//  02/24/97	MK		Added SetSound call to set default thrust & anger noises
//									Also added sound defines and modified eks bug.h
//	03/09/97	DS		Added patch to precent floating point errors...must check original
//									source
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "eks bug.h"
#include "eksekaren.h"
#include "referee.h"
#include "target lock.h"
#include "weapon ship.h"
#include "random.h"
#include "floor trigger.h"
#include "Sound Mgr.h"
#include "team identification.h"

// for sprintf debugging only
#include "view.h"


#define  DEFAULT_THRUST	37
#define  DEFAULT_ANGER	44




//------------------------------------------------------------------------------
EksBug::EksBug (Player *paren, int typ) : Player ("Eksekaren")
{
	type = typ;
	parent = paren;
	planner = 0;
	path = 0;
	gEksekarenCount++;
	SetSounds(DEFAULT_THRUST, DEFAULT_ANGER);																			// install the default anger & thrust sounds
	thrusting = FALSE;																														// New bug, set thrusting flag to false;
	SetTeam (TEAM_EKSEKAREN);																											// (JB) Sets the team so that turrets don't hurt bugs, etc.
}

//------------------------------------------------------------------------------
EksBug::~EksBug (void)
{
	if (gReferee->Confirm (ship))	{																								//	if the ship is still part of the world
		gReferee->Remove(ship);																											//	remove the ship
	}
}

//------------------------------------------------------------------------------
Bool	EksBug::Play (void)
{
	if (gReferee->Confirm (ship))
		return PlayAlive ();
	return PlayDead ();
}

//------------------------------------------------------------------------------
Bool	EksBug::PlayAlive (void)
{
	gFloor->Trigger (ship);
	if (ship->GetLife () > ZERO)
		DoState ();
	return TRUE;
}

//------------------------------------------------------------------------------
Bool	EksBug::PlayDead (void)
{
	if (parent)
		((EksBug *) parent)->RegisterDeath (type);
	if(gotplanner)
	{
		gEksekaren->ReleasePlanner ();
		gotplanner = FALSE;
	}																																							// (MK) Make sure planner is released if bug is killed during planning
	gEksekarenCount--;
	if (thrusting)																																// (MK) if we are making a thrusting noise...
	{
		gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);												// stop it and set thrusting = FALSE
		thrusting = FALSE;
	}
	return FALSE;
}

//------------------------------------------------------------------------------
void	EksBug::RegisterDeath (int typ)
{
}

//------------------------------------------------------------------------------
void	EksBug::SetTarget (const PiecePtr &piece)																	//	set the target piece
{																																								//	begin
	if (piece ())
		lock = new TLock (piece);
}																																								//	end

//------------------------------------------------------------------------------
void	EksBug::ChangeState (int stat)
{
	state = stat;
}

//------------------------------------------------------------------------------
Bool	EksBug::GoTo (CPoint pt, Real radius)																			//	fly the ship to a specified point
{																																								//	begin
	Real	rotate,
				thrust = ZERO;
	Bool	update = FALSE;
	Bool	there = ((Ship *) ship ())->GoTo (pt, rotate, thrust, radius);
	((Ship *) ship ())->Rotate (rotate);
	if (FABS (thrust) > EPSILON)
	{
		((Ship *) ship ())->Thrust (thrust);
		update = TRUE;
		if (!thrusting)	{																														// if ship isn't already thrusting
					thrustEMid = gSoundMgr->InsertEMSound(ekthrustsound, ship);						// install the thrusting emitter
					thrusting = TRUE;																											// set thrusting = true
		}
	}
	else if (thrusting)																														// (MK) if we are making a thrusting noise...
		{
			gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);											// stop it and set thrusting = FALSE
			thrusting = FALSE;
	}

	if (update)
		gReferee->Update (ship);
	return there;
}

//------------------------------------------------------------------------------
Bool	EksBug::GoAt (CPoint pt)																									//	fly the ship at a specified point
{																																								//	begin
	Real	rotate = ((Ship *) ship ())->GoTo (pt);
	((Ship *) ship ())->Rotate (rotate);
	Vector_2d	vel = ship->GetBody ()->TVelocity (),
						delta = pt - ship->GetBody ()->TLocation ();
	delta.Normalize ();
	Real	approach = vel | delta;
	vel.Normalize ();
	if (rotate < R(1.0))
	{
		if (((vel | delta) < R(0.9)) || (approach < R(200.0)))
		{
				((Ship *) ship ())->Thrust (R(1.0));
	
				if (!thrusting)	{																												// if ship isn't already thrusting
					thrustEMid = gSoundMgr->InsertEMSound(ekthrustsound, ship);						// install the thrusting emitter
					thrusting = TRUE;																											// set thrusting = true
				}
		}
		else if (thrusting)																													// (MK) if we are making a thrusting noise...
		{
			gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);											// stop it and set thrusting = FALSE
			thrusting = FALSE;
		}
	}
	else if (!(((vel | delta) < R(0.9)) || (approach < R(200.0))))
		if (thrusting)																															// (MK) if we are making a thrusting noise...
		{
			gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);											// stop it and set thrusting = FALSE
			thrusting = FALSE;
		}
		
				
	gReferee->Update (ship);
	return TRUE;
}

//------------------------------------------------------------------------------
void	EksBug::RandomWalk (void)																									//	do a random walk flight pattern
{																																								//	begin
	Real	rotate = (gRandom.RndReal () - R(0.5)) * R(2.0);
	((Ship *) ship ())->Rotate (rotate);
	((Ship *) ship ())->Thrust (R(1.0));
	gReferee->Update (ship);
}																																								//	end

//------------------------------------------------------------------------------
void	EksBug::Stop (void)																												//	stop the ship
{																																								//	begin
	Real	thrust,
				strafe;
	((Ship *) ship ())->Stop (thrust, strafe);
	if (thrust>INFINITY) 
	{
		DEBUG_HARD("EksBug: Stop() caught INFINITY in thrust\n");
		return;
	}
	((Ship *) ship ())->Thrust (thrust);
	if (strafe>INFINITY) 
	{
		DEBUG_HARD("EksBug: Stop() caught INFINITY in thrust\n");
		return;
	}
	((Ship *) ship ())->Strafe (strafe);

	if (thrusting)																																// (MK) if we are making a thrusting noise...
	{
			gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);											// stop it and set thrusting = FALSE
			thrusting = FALSE;
	}


	gReferee->Update (ship);
}																																								//	end

//------------------------------------------------------------------------------
void	EksBug::Drifting (void)
{
	if (thrusting)																																// (MK) if we are making a thrusting noise...
	{
			gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);											// stop it and set thrusting = FALSE
			thrusting = FALSE;
	}
}

//------------------------------------------------------------------------------
void	EksBug::SetSounds(int ekthrust, int ekanger)
{
		ekthrustsound = ekthrust;
		ekangersound = ekanger;
}

//------------------------------------------------------------------------------
