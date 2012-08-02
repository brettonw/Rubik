//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/12/97
//
//  02/20/97				MK						Added XX_XX_RATE, XX_XX_RANGE & XX_XX_DAMAGE parameters to Hunters (DR, HK, PRINCESS)
//									MK						Added HK_Elite type to Hunters, plus parameters & minetraik.h weapon set
//									MK						Added Weapon Sound defines for Hunter weapons
//	02/21/97				MK						Added Angry  Sound when target crosses into weapon range
//  02/21/97				MK						Bugs multitask in planning better...black magic..no explanation
//																Well..maybe just a little..reconfigured so that planner is released more often
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "thug.h"
#include "referee.h"
#include "floor trigger.h"

#include "weapon ship.h"
#include "traik.h"
#include "laser gun.h"
#include "cluster weapon.h"

#include "eksekaren.h"
#include "world.h"
#include "sprite array mgr.h"
#include "target lock.h"
#include "random.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "view.h"
#include "sound mgr.h"
#include "get parameters.h"

#include "minetraik.h"


#define  SCOUT_WEP_SOUND	19
#define  DR_WEP_SOUND	64
#define  HK_WEP_SOUND	22
#define  PRINCESS_WEP_SOUND	22
#define  ELITE_WEP_SOUND	67

#define  SCTHRUST	63
#define  SCANGER	62
#define  HKTHRUST	61
#define  HKANGER	60

//------------------------------------------------------------------------------
Thug::Thug (CPoint tloc, Real rloc, int typ) :																	//	constructor
EksBug (0, typ)																																			//	constructor (continued)
{
	state = DRIFT;
	NewShip (rloc, tloc);
}

//------------------------------------------------------------------------------
Thug::~Thug (void)
{
}

//------------------------------------------------------------------------------
void	Thug::NewShip (Real rLoc, CPoint tLoc)																											//	new ship function
{																																								//	begin
	gView->Install (new StatSeq (tLoc, new TimeSeq (gSpriteMgr.Fetch ("etel"))));
	WeaponShip	*wship;																														//	pointer to the weapon ship
	Weapon			*t1, *t2;																													//	the weapons
	switch (type)
	{
		case LBUG:
			wship = new WeaponShip (this, ParmReal (LBUG_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (LBUG_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (LBUG_T_THRUST), ParmReal (LBUG_R_THRUST));
			wship->SetArmor (ParmReal (LBUG_ARMOR));
			wship->SetHardness (ParmReal (LBUG_HARDNESS));
			wship->AdjustResources (ParmInteger (LBUG_RESOURCES));
			t1 = new LaserGun (ParmReal(LBUG_FIRE_RATE), ParmReal(LBUG_RANGE), ship, Point_2d (R(7.0), R(7.0)), ParmReal(LBUG_DAMAGE), MakePixel (15, 15, 31), gSpriteMgr.Fetch ("eksthr"), SCOUT_WEP_SOUND);
			t2 = new LaserGun (ParmReal(LBUG_FIRE_RATE), ParmReal(LBUG_RANGE), ship, Point_2d (R(-7.0), R(7.0)), ParmReal(LBUG_DAMAGE), MakePixel (15, 15, 31), gSpriteMgr.Fetch ("eksthr"), SCOUT_WEP_SOUND);
			wship->SetNozzles (1, Point_2d (ZERO, - (ParmReal (LBUG_RADIUS))));		//	set the ship thrust nozzle points
			SetSounds(SCTHRUST, SCANGER);
			break;
		case HUNTER1:
			wship = new WeaponShip (this, ParmReal (HK_ONE_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (HK_ONE_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (HK_ONE_T_THRUST), ParmReal (HK_ONE_R_THRUST));
			wship->SetArmor (ParmReal (HK_ONE_ARMOR));
			wship->SetHardness (ParmReal (HK_ONE_HARDNESS));
			wship->AdjustResources (ParmInteger (HK_ONE_RESOURCES));
			t1 = new Traik (ParmReal (HK_ONE_FIRE_RATE), ParmReal (HK_ONE_RANGE), ship, Point_2d (R(8.0), R(8.0)), ParmReal (HK_ONE_TRAIK_FORCE), R(0.4), ParmReal (HK_ONE_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), HK_WEP_SOUND, R(-5.0e-2)),
			t2 = new Traik (ParmReal (HK_ONE_FIRE_RATE), ParmReal (HK_ONE_RANGE), ship, Point_2d (R(-8.0), R(8.0)), ParmReal (HK_ONE_TRAIK_FORCE), R(0.4), ParmReal (HK_ONE_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), HK_WEP_SOUND, R(-5.0e-2));
			wship->SetNozzles (1, Point_2d (ZERO, - (ParmReal (HK_ONE_RADIUS))));		//	set the ship thrust nozzle points
			SetSounds(HKTHRUST, HKANGER);
			break;
		case HUNTER2:
			wship = new WeaponShip (this, ParmReal (HK_TWO_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (HK_TWO_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (HK_TWO_T_THRUST), ParmReal (HK_TWO_R_THRUST));
			wship->SetArmor (ParmReal (HK_TWO_ARMOR));
			wship->SetHardness (ParmReal (HK_TWO_HARDNESS));
			wship->AdjustResources (ParmInteger (HK_TWO_RESOURCES));
			t1 = new Traik (ParmReal (HK_TWO_FIRE_RATE), ParmReal (HK_TWO_RANGE), ship, Point_2d (R(6.0), R(8.0)), ParmReal (HK_TWO_TRAIK_FORCE), R(0.5), ParmReal (HK_TWO_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), HK_WEP_SOUND, R(-5.0e-2)),
			t2 = new Traik (ParmReal (HK_TWO_FIRE_RATE), ParmReal (HK_TWO_RANGE), ship, Point_2d (R(-6.0), R(8.0)), ParmReal (HK_TWO_TRAIK_FORCE), R(0.5), ParmReal (HK_TWO_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), HK_WEP_SOUND, R(-5.0e-2));
			wship->SetNozzles (2, Point_2d (R(15.0), R(-7.0)), Point_2d (R(-15.0), R(-7.0)));		//	set the ship thrust nozzle points
			SetSounds(HKTHRUST, HKANGER);
			break;
		case HUNTER3:
			wship = new WeaponShip (this, ParmReal (HK_THREE_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (HK_THREE_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (HK_THREE_T_THRUST), ParmReal (HK_THREE_R_THRUST));
			wship->SetArmor (ParmReal (HK_THREE_ARMOR));
			wship->SetHardness (ParmReal (HK_THREE_HARDNESS));
			wship->AdjustResources (ParmInteger (HK_THREE_RESOURCES));
			t1 = new Traik (ParmReal (HK_THREE_FIRE_RATE), ParmReal (HK_THREE_RANGE), ship, Point_2d (R(6.0), R(8.0)), ParmReal (HK_THREE_TRAIK_FORCE), R(0.5), ParmReal (HK_TWO_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), HK_WEP_SOUND, R(-5.0e-2)),
  		t2 = new Traik (ParmReal (HK_THREE_FIRE_RATE), ParmReal (HK_THREE_RANGE), ship, Point_2d (R(-6.0), R(8.0)), ParmReal (HK_THREE_TRAIK_FORCE), R(0.5), ParmReal (HK_TWO_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), HK_WEP_SOUND, R(-5.0e-2));
			wship->SetNozzles (2, Point_2d (R(11.0), R(-7.0)), Point_2d (R(-11.0), R(-7.0)));		//	set the ship thrust nozzle points
			SetSounds(HKTHRUST, HKANGER);
			break;
		case DRONE1:
			wship = new WeaponShip (this, ParmReal (DR_ONE_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (DR_ONE_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (DR_ONE_T_THRUST), ParmReal (DR_ONE_R_THRUST));
			wship->SetArmor (ParmReal (DR_ONE_ARMOR));
			wship->SetHardness (ParmReal (DR_ONE_HARDNESS));
			wship->AdjustResources (ParmInteger (DR_ONE_RESOURCES));
			t1 = new Traik (ParmReal (DR_ONE_FIRE_RATE), ParmReal (DR_ONE_RANGE), ship, Point_2d (R(6.0), R(8.0)), ParmReal (DR_ONE_TRAIK_FORCE), R(0.5), ParmReal (DR_ONE_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), DR_WEP_SOUND, R(-5.0e-2)),
			t2 = new Traik (ParmReal (DR_ONE_FIRE_RATE), ParmReal (DR_ONE_RANGE), ship, Point_2d (R(-6.0), R(8.0)), ParmReal (DR_ONE_TRAIK_FORCE), R(0.5), ParmReal (DR_ONE_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), DR_WEP_SOUND, R(-5.0e-2));
			wship->SetNozzles (1, Point_2d (ZERO, - (ParmReal (DR_ONE_RADIUS))));		//	set the ship thrust nozzle points
			// use default thrust & anger sounds
			wship->SetDeathTrigger(128,32,16);			// register trigger death for state mgr
			break;
		case DRONE2:
			wship = new WeaponShip (this, ParmReal (DR_TWO_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (DR_TWO_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (DR_TWO_T_THRUST), ParmReal (DR_TWO_R_THRUST));
			wship->SetArmor (ParmReal (DR_TWO_ARMOR));
			wship->SetHardness (ParmReal (DR_TWO_HARDNESS));
			wship->AdjustResources (ParmInteger (DR_TWO_RESOURCES));
			t1 = new Traik (ParmReal (DR_TWO_FIRE_RATE), ParmReal (DR_TWO_RANGE), ship, Point_2d (R(6.0), R(8.0)), ParmReal (DR_TWO_TRAIK_FORCE), R(0.5), ParmReal (DR_TWO_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), DR_WEP_SOUND, R(-5.0e-2)),
			t2 = new Traik (ParmReal (DR_TWO_FIRE_RATE), ParmReal (DR_TWO_RANGE), ship, Point_2d (R(-6.0), R(8.0)), ParmReal (DR_TWO_TRAIK_FORCE), R(0.5), ParmReal (DR_TWO_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), DR_WEP_SOUND, R(-5.0e-2));
			wship->SetNozzles (1, Point_2d (ZERO, - (ParmReal (DR_TWO_RADIUS))));		//	set the ship thrust nozzle points
			// use default thrust & anger sounds
			wship->SetDeathTrigger(128,32,16);			// register trigger death for state mgr
			break;
		case DRONE3:
			wship = new WeaponShip (this, ParmReal (DR_THREE_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (DR_THREE_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (DR_THREE_T_THRUST), ParmReal (DR_THREE_R_THRUST));
			wship->SetArmor (ParmReal (DR_THREE_ARMOR));
			wship->SetHardness (ParmReal (DR_THREE_HARDNESS));
			wship->AdjustResources (ParmInteger (DR_THREE_RESOURCES));
			t1 = new Traik (ParmReal (DR_THREE_FIRE_RATE), ParmReal (DR_THREE_RANGE), ship, Point_2d (R(6.0), R(8.0)), ParmReal (DR_THREE_TRAIK_FORCE), R(0.5), ParmReal (DR_THREE_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), DR_WEP_SOUND, R(-5.0e-2)),
			t2 = new Traik (ParmReal (DR_THREE_FIRE_RATE), ParmReal (DR_THREE_RANGE), ship, Point_2d (R(-6.0), R(8.0)), ParmReal (DR_THREE_TRAIK_FORCE), R(0.5), ParmReal (DR_THREE_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), DR_WEP_SOUND, R(-5.0e-2));
			wship->SetNozzles (1, Point_2d (ZERO, - (ParmReal (DR_THREE_RADIUS))));		//	set the ship thrust nozzle points
			// use default thrust & anger sounds
			wship->SetDeathTrigger(128,32,16);			// register trigger death for state mgr
			break;
		case PRINCESS:
			wship = new WeaponShip (this, ParmReal (PRINCESS_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (PRINCESS_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (PRINCESS_T_THRUST), ParmReal (PRINCESS_R_THRUST));
			wship->SetArmor (ParmReal (PRINCESS_ARMOR));
			wship->SetHardness (ParmReal (PRINCESS_HARDNESS));
			wship->AdjustResources (ParmInteger (PRINCESS_RESOURCES));
			t1 = new Traik (ParmReal (PRINCESS_FIRE_RATE), ParmReal (PRINCESS_RANGE), ship, Point_2d (R(6.0), R(8.0)), ParmReal (PRINCESS_TRAIK_FORCE), R(0.5), ParmReal (PRINCESS_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), PRINCESS_WEP_SOUND, R(-5.0e-2)),
			t2 = new Traik (ParmReal (PRINCESS_FIRE_RATE), ParmReal (PRINCESS_RANGE), ship, Point_2d (R(-6.0), R(8.0)), ParmReal (PRINCESS_TRAIK_FORCE), R(0.5), ParmReal (PRINCESS_DAMAGE), gSpriteMgr.Fetch ("streak"), gSpriteMgr.Fetch ("bsplut11"), PRINCESS_WEP_SOUND, R(-5.0e-2));
			break;
		case HKELITE:
			wship = new WeaponShip (this, ParmReal (HK_ELITE_RADIUS), tLoc, rLoc, gSpriteMgr.Fetch (ParmString (HK_ELITE_SPRITE)), 1);
			ship = PiecePtr (wship);
			wship->AdjustThrust (ParmReal (HK_ELITE_T_THRUST), ParmReal (HK_ELITE_R_THRUST));
			wship->SetArmor (ParmReal (HK_ELITE_ARMOR));
			wship->SetHardness (ParmReal (HK_ELITE_HARDNESS));
			wship->AdjustResources (ParmInteger (HK_ELITE_RESOURCES));
			t1 = new MineTraik (ParmReal (HK_ELITE_FIRE_RATE), ParmReal (HK_ELITE_RANGE), ship, Point_2d (R(6.0), R(8.0)), ParmReal (HK_ELITE_TRAIK_FORCE), R(0.5), ParmReal (HK_ELITE_DAMAGE), gSpriteMgr.Fetch ("tacmine"), gSpriteMgr.Fetch ("bsplut11"), ELITE_WEP_SOUND, R(-5.0e-2)),
			t2 = new MineTraik (ParmReal (HK_ELITE_FIRE_RATE), ParmReal (HK_ELITE_RANGE), ship, Point_2d (R(-6.0), R(8.0)), ParmReal (HK_ELITE_TRAIK_FORCE), R(0.5), ParmReal (HK_ELITE_DAMAGE), gSpriteMgr.Fetch ("tacmine"), gSpriteMgr.Fetch ("bsplut11"), ELITE_WEP_SOUND, R(-5.0e-2));
			wship->SetNozzles (2, Point_2d (R(11.0), -(ParmReal (HK_ELITE_RADIUS))), Point_2d (R(-11.0), -(ParmReal (HK_ELITE_RADIUS))));		//	set the ship thrust nozzle points
			break;
		default:
			break;
	}
	Cluster	*cluster = new Cluster (ship, 2, t1, t2);
	cluster->AddAmmoCount (100);
	wship->SetWeapon (0, cluster);
	wship->SetStun (R(0.25));
	playerspotted = FALSE;																														// Haven't seen player yet																																// I don't have the planner
	gReferee->Install (ship);
}																																								//	end

//------------------------------------------------------------------------------
void	Thug::ChangeState (int stat)																							//	switch to a new state
{	
																																								//	begin
	switch (state)
	{
		case PLANNING:
			gEksekaren->ReleasePlanner ();
			break;
		case FOLLOWING_PATH:
			gEksekaren->ReleasePlanner ();
			delete path;
			break;
	}
	EksBug::ChangeState (stat);
	if (state == TRACKING_TARGET){
		inRange = FALSE;
		}
}																																								//	end

//------------------------------------------------------------------------------
void	Thug::TrackingTarget (CPoint loc, CVector toTarget)
{
	PiecePtr	firstHit;


	gReferee->TraceRay (loc, toTarget, firstHit);
	if (firstHit () != (((TLock *) lock ())->Target ()) ())
		ChangeState (DRIFT);
	else
	{
		playerspotted = TRUE;
		Weapon	*weapon = ((WeaponShip *) ship ())->GetCurrentWeapon ();
		Real		range = weapon->Range ();
		if (!inRange)
			range *= R(0.8);
		if ((toTarget | toTarget) > (range * range))
		{
			GoAt (lock->Location (R(0.1)));
			inRange = FALSE;
		}
		else
		{
			Attack (lock->Location ());
			if(!inRange)
					gSoundMgr->PlaySound(ekangersound, ship);																	// (MK) Play Angry Sound!
			inRange = TRUE;
		}
	}
}

//------------------------------------------------------------------------------
void	Thug::DoState (void)																											//	do the appropriate action for the current state and situation
{																																								//	begin
	CPoint		loc = ship->GetBody ()->TLocation ();
	if (lock () && lock->Confirm ())
	{
		Vector_2d	toTarget = lock->Location () - loc;
		PiecePtr	unused;
		if (gWorld->TraceRay (loc, toTarget) > R(1.0))
			switch (state)
			{
				case TRACKING_TARGET:
					TrackingTarget (loc, toTarget);
					break;
				default:
					ChangeState (TRACKING_TARGET);
					break;
			}
		else
			switch (state)
			{
				case DRIFT:
				{
					if (playerspotted){
						planner = gEksekaren->GrabPlanner ();
						if (planner)
						{
							planner->StartPlan (loc, lock->Location ());
							ChangeState (PLANNING);
							delay = 0;
							Stop ();
						}
						else
								gEksekaren->ReleasePlanner ();
					}
				}	break;

				case PLANNING:
					{
						if (playerspotted)
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
					if (GoTo (path->GetCurrent (), ship->GetGeometry ()->Bound () * R(2.0)) == FALSE)
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
}												
//------------------------------------------------------------------------------
void	Thug::Attack (CPoint pt)																									//	fly the ship at a specified point and fire
{																																								//	begin
	Real	rotate = ((WeaponShip *) ship ())->AimAt (pt);
	((Ship *) ship ())->Rotate (rotate);
	if (FABS (rotate) < R(1.0))
		((WeaponShip *) ship ())->Fire ();
	Vector_2d	vel = ship->GetBody ()->TVelocity (),
						delta = pt - ship->GetBody ()->TLocation ();
	delta.Normalize ();
	Real	approach = vel | delta;
	vel.Normalize ();

	gEksekaren->ReleasePlanner ();
	if (rotate < R(1.0))
	{
		if ((vel | delta) < R(0.9))
			{
					((Ship *) ship ())->Thrust (R(0.25));
		
					if (!thrusting)	{																																// if ship isn't already thrusting
						thrustEMid = gSoundMgr->InsertEMSound(ekthrustsound, ship);										// install the thrusting emitter
						thrusting = TRUE;																															// set thrusting = true
					}
			}
		else if (thrusting)																																// (MK) if we are making a thrusting noise...
		{
			gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);																	// stop it and set thrusting = FALSE
			thrusting = FALSE;
		}
	}
	else if (!(((vel | delta) < R(0.9)) || (approach < R(200.0))))
		if (thrusting)																																// (MK) if we are making a thrusting noise...
		{
			gSoundMgr->RemoveEMSound(ekthrustsound, thrustEMid);																	// stop it and set thrusting = FALSE
			thrusting = FALSE;
		}

	gReferee->Update (ship);
}																																								//	end

//------------------------------------------------------------------------------
