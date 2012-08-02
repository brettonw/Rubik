//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/06/97
//
//	02/11/97	MK		Added Turret Turning sounds, plus "triggered" sound flag
//	03/04/97	JB		modified the turret's rate of fire
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "turret thug.h"
#include "referee.h"
#include "world.h"
#include "sprite array mgr.h"
#include "fireball traik.h"
#include "cluster weapon.h"
#include "quadratic.h"
#include "Sound Mgr.h"

#define  ROTATESOUND 45

//------------------------------------------------------------------------------
TurretThug::TurretThug (CPoint tLoc, Real rLoc, int type, int id) : Player ("Turret"), Device (id)	//	constructor
{																																								//	begin
	orient = Vector_2d (rLoc);																										//	copy the default orientation
	turret = new Turret (this, R(10.0), tLoc, rLoc, gSpriteMgr.Fetch ("turret01"));
	((Turret *) turret ())->AdjustThrust (R(3.0));
	turret->SetArmor (R(5.0));
	FTraik	*t1 = new FTraik (R(0.5), INFINITY, turret, Point_2d (R(4.0), R(6.0)), R(5.0e4), R(0.7), R(0.5), gSpriteMgr.Fetch ("eksthr"), gSpriteMgr.Fetch ("bsplut11"), 21, ZERO),
					*t2 = new FTraik (R(0.5), INFINITY, turret, Point_2d (R(-4.0), R(6.0)), R(5.0e4), R(0.7), R(0.5), gSpriteMgr.Fetch ("eksthr"), gSpriteMgr.Fetch ("bsplut11"), 21, ZERO);
	t1->SetHeating (R(0.75), R(1.0));
	t2->SetHeating (R(0.75), R(1.0));
	Cluster	*c1 = new Cluster (turret, 2, t1, t2);
	c1->AddAmmoCount (1000);
	((Turret *) turret ())->SetWeapon (c1);
	gReferee->Install (turret);
}																																								//	end

//------------------------------------------------------------------------------
TurretThug::~TurretThug (void)																									//	destructor
{																																								//	begin
	if (gReferee->Confirm (turret))																								//	if the turret is still part of the world
		gReferee->Remove(turret);																										//	remove the turret
}																																								//	end

//------------------------------------------------------------------------------
Bool	TurretThug::Play (void)																										//	play one frame
{																																								//	begin
	if (gReferee->Confirm (turret))
	{
		if (power)
			if ((turret->GetLife () > ZERO))
				if (gReferee->Confirm (target))
				{
					Point_2d	tLoc = target->GetBody ()->TLocation (),
										myLoc = turret->GetBody ()->TLocation ();
					Vector_2d tVel = target->GetBody ()->TVelocity ();
					Weapon *weapon = ((Turret *) turret ())->GetWeapon ();
					Real	bulletspeed = weapon->Velocity (),
								a = (tVel | tVel) - bulletspeed * bulletspeed,
								b = R(2.0) * ((tLoc | tVel) - (myLoc | tVel)),
								c = (tLoc | tLoc) + (myLoc | myLoc) - (R(2.0) * (tLoc | myLoc)),
								t1,
								t2;
					if (Quadratic (a, b, c, t1, t2))
					{
						if (t1 < ZERO)
							t1 = t2;
						if (t1 >= ZERO)
						{
							tVel *= t1;
							Point_2d	modTarget = tLoc + tVel;
							Vector_2d	deltaVec = modTarget - myLoc;
							if ((deltaVec | orient) > ZERO)
							{
								if (gWorld->TraceRay (myLoc, deltaVec) >= R(1.0))
								{
									Real	delta = ((Turret *) turret ())->PointAt (modTarget);
									((Turret *) turret ())->Rotate (delta);

									
									if (FABS (delta) < R(1.0))
										((Turret *) turret ())->Fire ();
									else
										// (MK) Rotate Sound
										if (!triggered){
											gSoundMgr->PlaySound(ROTATESOUND, turret, FALSE);
											triggered = TRUE;
										}
										
								}
								else
								{
									Real	delta = ((Turret *) turret ())->Point (orient);
									((Turret *) turret ())->Rotate (delta);
									triggered = FALSE;
								}
							}
							else
							{
								Real	delta = ((Turret *) turret ())->Point (orient);
								((Turret *) turret ())->Rotate (delta);
							}
						}
						else
						{
							Real	delta = ((Turret *) turret ())->Point (orient);
							((Turret *) turret ())->Rotate (delta);
						}
					}
					else
					{
						Real	delta = ((Turret *) turret ())->Point (orient);
						((Turret *) turret ())->Rotate (delta);
					}
				}
				else
				{
					Real	delta = ((Turret *) turret ())->Point (orient);
					((Turret *) turret ())->Rotate (delta);
				}
		return TRUE;
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
int		TurretThug::Trigger (const PiecePtr &piece, int code, int code2)					//	trigger the device
{																																								//	begin
	Device::Trigger (piece, code, code2);
	if (code == DEV_TRIGGER){
		SetTarget (piece);
	};
	return 0;
}																																								//	end

//------------------------------------------------------------------------------
