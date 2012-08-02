//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "multipede.h"
#include "referee.h"
#include "floor trigger.h"
#include "traik.h"
#include "laser gun.h"
#include "cluster weapon.h"
#include "sprite array mgr.h"
#include "get parameters.h"
#include "eksekaren.h"
#include "world.h"
#include "target lock.h"
#include "random.h"
#include "view.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "sound mgr.h"
#include "spring.h"
#include "pi.h"

//------------------------------------------------------------------------------
#define	SPRING_TENSION	R(1.0e3)

//------------------------------------------------------------------------------
void	Multipede::NewShip (void)																											//	new ship function
{																																								//	begin

	//	note - need to subtract from the gEksekarenCount when this function fails, but
	//	being as it is still a prototype, I haven't added it... see Thug.cpp for an example

	//gView->Install (new StatSeq (tLoc, new TimeSeq (gSpriteMgr.Fetch ("etel"))));
	WeaponShip	*wship;																														//	pointer to the weapon ship
	Weapon			*t1, *t2;																													//	the weapons
	Real				radius = ParmReal (DR_ONE_RADIUS),
							tthrust = R(5.0e1),//ParmReal (DR_THREE_T_THRUST),
							rthrust = R(3.0e0),//ParmReal (DR_THREE_R_THRUST),
							armor = ZERO,//ParmReal (DR_THREE_ARMOR),
							hardness = ParmReal (DR_ONE_HARDNESS);
	SArrayPtr		sprite = gSpriteMgr.Fetch (ParmString (DR_ONE_SPRITE));
	separation = radius * R(2.25);
	Vector_2d		delta = -Vector_2d (rLoc) * separation;
	for (int i = 0; i < maxCount; i++)
	{
		wship = new WeaponShip (this, radius, tLoc + (delta * Real (i)), rLoc, sprite, 1);
		ship[i] = PiecePtr (wship);
		wship->AdjustThrust (tthrust, rthrust);
		wship->SetArmor (armor);
		wship->SetHardness (hardness);
		t1 = new Traik (R(0.75), R(1.5e2), ship[i], Point_2d (R(6.0), R(8.0)), R(2.0e5), R(0.5), R(0.3), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), 22, R(-5.0e-2)),
		t2 = new Traik (R(0.75), R(1.5e2), ship[i], Point_2d (R(-6.0), R(8.0)), R(2.0e5), R(0.5), R(0.3), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), 22, R(-5.0e-2));
		wship->SetNozzles (1, Point_2d (ZERO, -radius));		//	set the ship thrust nozzle point
	//	wship->SetNozzles (2, Point_2d (radius, ZERO), Point_2d (-radius, ZERO));		//	set the ship thrust nozzle points
		Cluster	*cluster = new Cluster (ship[i], 2, t1, t2);
		cluster->AddAmmoCount (100);
		wship->SetWeapon (0, cluster);
		wship->SetStun (R(0.25));
		gReferee->Install (ship[i]);
	}
	Body	*body = ship[0]->GetBody ();
	damping = body->TInertia () * R(0.25);
	count = maxCount;
}																																								//	end

//------------------------------------------------------------------------------
void	Multipede::ChangeState (int stat)																					//	switch to a new state
{																																								//	begin
	switch (state)
	{
		case PLANNING:
			gEksekaren->ReleasePlanner ();
			break;
		case FOLLOWING_PATH:
			delete path;
			break;
	}
	state = stat;
	if (state == TRACKING_TARGET)
		inRange = FALSE;
}																																								//	end

//------------------------------------------------------------------------------
void	Multipede::DoState (void)																									//	do the appropriate action for the current state and situation
{																																								//	begin
	CPoint		loc = ship[0]->GetBody ()->TLocation ();
	if (lock () && lock->Confirm ())
	{
		Vector_2d	toTarget = lock->Location () - loc;
		PiecePtr	unused;
		if (gWorld->TraceRay (loc, toTarget) > R(1.0))
			switch (state)
			{
				case TRACKING_TARGET:
					gReferee->TraceRay (loc, toTarget, unused);
					if (unused () != (((TLock *) lock ())->Target ()) ())
						ChangeState (DRIFT);
					else
					{
						Weapon	*weapon = ((WeaponShip *) ship[0] ())->GetCurrentWeapon ();
						Real		range = weapon->Range ();
						if (!inRange)
							range *= R(0.8);
						if ((toTarget | toTarget) > (range * range))
							GoTo (lock->Location (R(0.1)), lock->Radius ());
						else
							Attack (lock->Location ());
					}
					break;
				default:
					ChangeState (TRACKING_TARGET);
					break;
			}
		else
			switch (state)
			{
				case DRIFT:
					planner = gEksekaren->GrabPlanner ();
					if (planner)
					{
						planner->StartPlan (loc, lock->Location ());
						ChangeState (PLANNING);
						delay = 0;
						Stop ();
					}
					/*
					else
						RandomWalk ();
					*/
					break;
				case PLANNING:
					{
						for (int i = 0; i < 4; i++)
							if (planner->Plan () == FALSE)
							{
								path = planner->BuildPath ();
								if (path)
									ChangeState (FOLLOWING_PATH);
								else
									ChangeState (DRIFT);
								break;
							}
					}
					break;
				case FOLLOWING_PATH:
					toTarget = path->GetCurrent () - loc;
					while (gWorld->TraceRay (loc, toTarget) < R(1.0))
						if (path->Previous () == FALSE)
						{
							ChangeState (DRIFT);
							return;
						}
					gReferee->TraceRay (loc, toTarget, unused);
					if (unused () != gWorld ())
					{
						if (++delay > 150)
						{
							ChangeState (DRIFT);
							return;
						}
					}
					else
						delay = 0;
					if (GoTo (path->GetCurrent (), ship[0]->GetGeometry ()->Bound () * R(2.0)) == FALSE)
						if (path->Next () == FALSE)
							ChangeState (DRIFT);
					break;
				case TRACKING_TARGET:
					ChangeState (DRIFT);
					break;
			}
	}
	else
		ChangeState (DRIFT);
}																																								//	end

//------------------------------------------------------------------------------
Bool	Multipede::GoTo (CPoint pt, Real radius)																	//	fly the ship to a specified point
{																																								//	begin
	Real	rotate,
				thrust;
	Bool	there = FALSE;
	if (gReferee->Confirm (ship[1]))
	{
		Body	*body = ship[0]->GetBody ();
		Vector_2d	angle (body->RLocation ()),
							delta = body->TLocation () - ship[1]->GetBody ()->TLocation ();
	/*
		delta.Normalize ();
	//	Real			maxAngle = COS (TWO_PI / Real (count + 1));
		Real			maxAngle = COS (TWO_PI / Real (count));
		if ((angle | delta) > maxAngle)
	*/
		{
			rotate = ((Ship *) ship[0] ())->PointAt (pt);
			((Ship *) ship[0] ())->Rotate (rotate);
		}
		delta = body->TLocation () - pt;
		if (delta.Norm () < radius)
			there = TRUE;
		else
		((Ship *) ship[0] ())->Thrust (R(0.1));
	}
	else
	{
		there = ((Ship *) ship[0] ())->GoTo (pt, rotate, thrust, radius);
		((Ship *) ship[0] ())->Rotate (rotate);
		if (FABS (thrust) > EPSILON)
			((Ship *) ship[0] ())->Thrust (thrust);
	}
	return there;
}																																								//	end

//------------------------------------------------------------------------------
void	Multipede::Attack (CPoint pt)																							//	fly the ship at a specified point and fire
{																																								//	begin
	if (gReferee->Confirm (ship[1]))
	{
	/*
		Body	*body = ship[0]->GetBody ();
		Vector_2d	angle (body->RLocation ()),
							delta = body->TLocation () - ship[1]->GetBody ()->TLocation ();
		delta.Normalize ();
//		Real			maxAngle = COS (TWO_PI / Real (count + 1));
		Real			maxAngle = COS (TWO_PI / Real (count));
		if ((angle | delta) > maxAngle)
*/
		{
			Real	rotate = ((Ship *) ship[0] ())->PointAt (pt);
			((Ship *) ship[0] ())->Rotate (rotate);
		}
		((Ship *) ship[0] ())->Thrust (R(0.1));
	}
	else
	{
		Real	rotate = ((WeaponShip *) ship[0] ())->AimAt (pt);
		((Ship *) ship[0] ())->Rotate (rotate);
		if (FABS (rotate) < R(1.0))
			((WeaponShip *) ship[0] ())->Fire ();
	}
}																																								//	end

//------------------------------------------------------------------------------
void	Multipede::RandomWalk (void)																							//	do a random walk flight pattern
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Multipede::Stop (void)																										//	stop the ship
{																																								//	begin
	Real	thrust,
				strafe;
	((Ship *) ship[0] ())->StopY (thrust, strafe);
	((Ship *) ship[0] ())->Thrust (thrust);
	//((Ship *) ship ())->Strafe (strafe);
}																																								//	end

//------------------------------------------------------------------------------
Multipede::Multipede (CPoint tloc, Real rloc, int coun) : Player ("Eksekaren")	//	constructor
{																																								//	begin
	tLoc = tloc;
	rLoc = rloc;
	maxCount = count = coun;
	ship = new PiecePtr[count];
	state = DRIFT;
}																																								//	end

//------------------------------------------------------------------------------
Multipede::~Multipede (void)																										//	destructor
{																																								//	begin
	for (int i = 0; i < count; i++)
		if (gReferee->Confirm (ship[i]))																									//	if the ship is still part of the world
			gReferee->Remove(ship[i]);																											//	remove the ship
	delete[] ship;
	ChangeState (DRIFT);
}																																								//	end

//------------------------------------------------------------------------------
Bool	Multipede::Play (void)																										//	play one frame
{																																								//	begin
	int	last = 0;
	for (int i = 0; i < count; i++)
		if (gReferee->Confirm (ship[i]))
		{
			gFloor->Trigger (ship[i]);																									//	check the floor for triggers
			ship[last++] = ship[i];
		}
	count = last;
	if (count)
	{
		if (ship[0]->GetLife () > ZERO)
		{
			Real	thrust,
						strafe;
			DoState ();
			gReferee->Update (ship[0]);
			for (int i = 1; i < count; i++)
			{
				Spring (ship[i - 1], ship[i], SPRING_TENSION, damping, separation);
				if (ship[i]->GetLife () > ZERO)
				{
					((Ship *) ship[i] ())->StopX (thrust, strafe);
					((Ship *) ship[i] ())->Strafe (strafe);
					Real	rotate = ((Ship *) ship[i] ())->PointAt (ship[i - 1]->GetBody ()->TLocation ());
					((Ship *) ship[i] ())->Rotate (rotate);
				}
				gReferee->Update (ship[i]);
			}
		}
	}
	else
	{
		ChangeState (DRIFT);
		NewShip ();
	}
	return TRUE;
}																																								//	end

//------------------------------------------------------------------------------
void	Multipede::SetTarget (const PiecePtr &piece)															//	set the target piece
{																																								//	begin
	if (piece ())
		lock = new TLock (piece);
}																																								//	end

//------------------------------------------------------------------------------
