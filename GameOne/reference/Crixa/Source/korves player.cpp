//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/20/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "korves player.h"
#include "referee.h"
#include "human.h"
#include "floor trigger.h"
#include "sprite array mgr.h"
#include "get parameters.h"
#include "world.h"
#include "target lock.h"
#include "random.h"
#include "view.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "sound mgr.h"

//------------------------------------------------------------------------------
Signature	KorvesPlay::sign (3, 3, 2.5e-1, 4, 5.0e-1, 5, 2.5e-1);

//------------------------------------------------------------------------------
void	KorvesPlay::NewShip (void)																								//	new ship function
{																																								//	begin
	ship = new KorvesShip (	this, 
												ParmReal (KORVES_RADIUS), 
												tLoc, 
												rLoc, 
												gSpriteMgr.Fetch (ParmString (KORVES_SPRITE)));				//	create the korves base
	((Ship *) ship ())->SetStun (R(0.25));
	ship->SetSignature (&sign);
	gReferee->Install (ship);
}																																								//	end

//------------------------------------------------------------------------------
void	KorvesPlay::ChangeState (int stat)																				//	switch to a new state
{																																								//	begin
	switch (state)
	{
		case KORVES_PLANNING:
			Human::ReleasePlanner ();
			break;
		case KORVES_FOLLOWING_PATH:
			delete path;
			break;
	}
	state = stat;
}																																								//	end

//------------------------------------------------------------------------------
void	KorvesPlay::DoState (void)																											//	do the appropriate action for the current state and situation
{																																								//	begin
	CPoint		loc = ship->GetBody ()->TLocation ();
	if (lock () && lock->Confirm ())
	{
		Vector_2d	toTarget = lock->Location () - loc;
		if (gWorld->TraceRay (loc, toTarget) > R(1.0))
			switch (state)
			{
				case KORVES_TRACKING_TARGET:
					{
						Weapon	*weapon = ((WeaponShip *) ship ())->GetCurrentWeapon ();
						Real		range = weapon->Range ();
						if ((toTarget | toTarget) > (range * range))
							GoTo (lock->Location (R(0.1)));
						else
							Attack (lock->Location ());
					}
					break;
				default:
					ChangeState (KORVES_TRACKING_TARGET);
					break;
			}
		else
			switch (state)
			{
				case KORVES_DRIFT:
					planner = Human::GrabPlanner ();
					if (planner)
					{
						planner->StartPlan (loc, lock->Location ());
						ChangeState (KORVES_PLANNING);
						Stop ();
					}
					break;
				case KORVES_PLANNING:
					{
						for (int i = 0; i < 4; i++)
							if (planner->Plan () == FALSE)
							{
								path = planner->BuildPath ();
								if (path)
									ChangeState (KORVES_FOLLOWING_PATH);
								else
									ChangeState (KORVES_DRIFT);
								break;
							}
					}
					break;
				case KORVES_FOLLOWING_PATH:
					while (gWorld->TraceRay (loc, path->GetCurrent () - loc) < R(1.0))
						if (path->Previous () == FALSE)
						{
							ChangeState (KORVES_DRIFT);
							return;
						}
					if (GoTo (path->GetCurrent ()) == FALSE)
						if (path->Next () == FALSE)
							ChangeState (KORVES_DRIFT);
					break;
				case KORVES_TRACKING_TARGET:
					ChangeState (KORVES_DRIFT);
					break;
			}
	}
	else
		ChangeState (KORVES_DRIFT);
}																																								//	end

//------------------------------------------------------------------------------
Bool	KorvesPlay::GoTo (CPoint pt)																										//	fly the ship to a specified point
{																																								//	begin
	Real	rotate = ((WeaponShip *) ship ())->AimAt (pt),
				thrust = ZERO,
				strafe = ZERO;
	Bool	there = ((Ship *) ship ())->EliteGoTo (pt, thrust, strafe, R(6.0e1));
	((Ship *) ship ())->Rotate (rotate);
	((Ship *) ship ())->Thrust (thrust);
	((Ship *) ship ())->Strafe (strafe);
	gReferee->Update (ship);
	return there;
}																																								//	end

//------------------------------------------------------------------------------
void	KorvesPlay::Attack (CPoint pt)																									//	fly the ship at a specified point and fire
{																																								//	begin
	Real	rotate = ((WeaponShip *) ship ())->AimAt (pt);
	((Ship *) ship ())->Rotate (rotate);
	if (FABS (rotate) < R(1.0))
		((WeaponShip *) ship ())->Fire ();
	((Ship *) ship ())->Thrust (R(1.0));
	gReferee->Update (ship);
}																																								//	end

//------------------------------------------------------------------------------
void	KorvesPlay::Stop (void)																													//	stop the ship
{																																								//	begin
	Real	thrust,
				strafe;
	((Ship *) ship ())->Stop (thrust, strafe);
	((Ship *) ship ())->Thrust (thrust);
	((Ship *) ship ())->Strafe (strafe);
	gReferee->Update (ship);
}																																								//	end

//------------------------------------------------------------------------------
KorvesPlay::KorvesPlay (CPoint tloc, Real rloc)																	//	constructor
{																																								//	begin
	tLoc = tloc;
	rLoc = rloc;
	kill = FALSE;
	state = KORVES_DRIFT;
}																																								//	end

//------------------------------------------------------------------------------
KorvesPlay::~KorvesPlay (void)																									//	destructor
{																																								//	begin
	if (gReferee->Confirm (ship))																									//	if the ship is still part of the world
		gReferee->Remove(ship);																											//	remove the ship
	ChangeState (KORVES_DRIFT);
}																																								//	end

//------------------------------------------------------------------------------
Bool	KorvesPlay::Play (void)																										//	play one frame
{																																								//	begin
	if (gReferee->Confirm (ship))
	{
		gFloor->Trigger (ship);																											//	check the floor for triggers
		if (ship->GetLife () > ZERO)
			DoState ();
	}
	else
	{
		ChangeState (KORVES_DRIFT);
		NewShip ();
	}
	return TRUE;
}																																								//	end

//------------------------------------------------------------------------------
void	KorvesPlay::SetGoal (const LockPtr &lok, Bool kil)												//	set the korves base goal
{																																								//	begin
	lock = lok;
	kill = kil;
}																																								//	end

//------------------------------------------------------------------------------
