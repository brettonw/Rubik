//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//
//	01/22/97	JB		Modified face & cilia to use ConstrainedTarget
//	01/23/97	JB		SwitchOff overloaded for death upon loss of power
//	01/23/97	JB		Modified the various armor ratings (tougher now)
//	02/14/97	JB		added a turret for mark's eks turret
//	02/21/97	JB		added judgemental targeting, based on team identification
//									gave the jenny parts eks-friendly guns
//	03/04/97	JB		slowed turret 40's rate of fire.
//	03/06/97	JB		adjusting armor levels on all the turrets
//	03/07/97	JB		JForeHaunch* changed to JHaunch*.  #'s 19 & 20 become Haunches rather than Legs.
//	03/12/97	JB		adjusted damage, rate, armor levels.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include "jenny part thugs.h"
#include "jenny parts.h"
#include "turret.h"
#include "sprite array mgr.h"
#include "referee.h"
#include "world.h"
#include "quadratic.h"
#include "cluster weapon.h"
#include "eks laser gun.h"
#include "eks fireball traik.h"
#include "spring gun.h"
#include "get parameters.h"
#include "team identification.h"
#include "sound enum.h"
#include "sound mgr.h"
#include "random.h"

//------------------------------------------------------------------------------
//	constants
//------------------------------------------------------------------------------
const Real	thrust_fast		= R(5.0);
const Real	thrust_med		= R(3.0);
const Real	thrust_slow		= R(1.5);
const Real	armor_high		= R(700.0);	//	was 30.0
const Real	armor_med			= R(40.0);	//	was 15.0
const Real	armor_low			= R(12.5);	//	was 5.0
const	Real	kMarkPartArmor= R(20.0);
const	Real	kJawPulseTime = R(4.0);

//------------------------------------------------------------------------------
//	jenny part thug constructor
//------------------------------------------------------------------------------
JennyPartThug::JennyPartThug (int id) :																					//	constructor
Player ("Eksekaren"),																														//	constructor (continued)
Device (id)																																			//	constructor (continued)
{
	firing = TRUE;
	SetTeam (TEAM_EKSEKAREN);
}

//------------------------------------------------------------------------------
//	jenny part thug destructor
//------------------------------------------------------------------------------
JennyPartThug::~JennyPartThug (void)
{																																								//	begin
	if (gReferee->Confirm (turret))																								//	if the turret is still part of the world
		gReferee->Remove(turret);																										//	remove the turret
}																																								//	end

//------------------------------------------------------------------------------
//	set the jenny part in motion
//------------------------------------------------------------------------------
Bool	JennyPartThug::Play (void)																								//	play one frame
{																																								//	begin
	if (gReferee->Confirm (turret))																								//	make sure the turret is still alive
	{																																							//
		if (power)																																	//	if there is power
			if ((turret->GetLife () > ZERO))																					//	and the turret is still alive
			{																																					//
				if (gReferee->Confirm (target))																					//	confirm that the target is still around
					if (Target ())																												//	attempt to aim at the target
						return TRUE;																												//
				Real	delta = ((Turret *) turret ())->Point (orient);										//	point at the default orientation
				((Turret *) turret ())->Rotate (delta);																	//	apply the necessary rotations to do so
			}																																					//
		return TRUE;																																//	still playing
	}																																							//
	return FALSE;																																	//	not still playing
}																																								//

//------------------------------------------------------------------------------
//	trigger the device, setting the target if necessary
//------------------------------------------------------------------------------
int		JennyPartThug::Trigger (const PiecePtr &piece, int code, int code2)				//	trigger the device
{																																								//	begin
	Device::Trigger (piece, code, code2);																					//	trigger the device
	if (code == DEV_TRIGGER)																											//	if a piece triggered the piece
	{
		Bool	targetit = TRUE;																											//	assume it's a valid target
		Player *owner = piece->GetOwner ();																					//	get the piece's owner
		if (owner && (owner->Team () == TEAM_EKSEKAREN))														//	if the piece is owned by an eksekaren,
			targetit = FALSE;																													//		don't target it
		if (targetit)																																//	if it's a good target
			SetTarget (piece);																												//	let her rip
	}
	return 0;																																			//
}																																								//	end

//------------------------------------------------------------------------------
//	switch off the jenny-turret, when the generator (bosshead) dies
//------------------------------------------------------------------------------
Bool	JennyPartThug::SwitchOff (void)																						//	switch off the power
{																																								//
	((JennyPart *) turret ())->Kill ();																						//	kill the jenny-turret (when the head dies)
	return Device::SwitchOff ();																									//	remove the power
}																																								//

//------------------------------------------------------------------------------
//	default targeting
//------------------------------------------------------------------------------
Bool	JennyPartThug::Target (void)																							//	default jenny part targeting method
{																																								//
	return NoLeadTarget ();																												//	regular no-leading targeting
}																																								//

//------------------------------------------------------------------------------
Bool	JennyPartThug::LeadTarget (void)
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
				if (gWorld->TraceRay (myLoc, deltaVec) >= R(1.0))
				{
					Real	delta = ((Turret *) turret ())->PointAt (modTarget);
					((Turret *) turret ())->Rotate (delta);
					if (FABS (delta) < R(1.0))
						((Turret *) turret ())->Fire ();
					return TRUE;
				}
		}
	}
	return FALSE;
}

//------------------------------------------------------------------------------
Bool	JennyPartThug::NoLeadTarget (void)
{
	Point_2d	tLoc = target->GetBody ()->TLocation (),
						myLoc = turret->GetBody ()->TLocation ();
	Vector_2d	deltaVec = tLoc - myLoc;
	if ((deltaVec | orient) > ZERO)
		if (gWorld->TraceRay (myLoc, deltaVec) >= R(1.0))
		{
			Real	delta = ((Turret *) turret ())->PointAt (tLoc);
			((Turret *) turret ())->Rotate (delta);
			if (FABS (delta) < R(1.0))
			{
				if ((gRandom.RndInteger () & 127) == 0)
					gSoundMgr->PlaySound (snd_anger, turret);
				((Turret *) turret ())->Fire ();
			}
			return TRUE;
		}
	return FALSE;
}

//------------------------------------------------------------------------------
Bool	JennyPartThug::ConstrainedTarget (void)																		//	blahblahblah targeting code
{																																								//
	Point_2d	tLoc = target->GetBody ()->TLocation (),														//	get the target's location
						myLoc = turret->GetBody ()->TLocation ();														//	get the turret's location
	Vector_2d deltaVec = tLoc - myLoc;																						//	determine the direction vector from the turret to the target
	deltaVec.Normalize ();																												//	normalize the delta vector
	PiecePtr	firstPiece;
	Real			dist = gReferee->TraceRay (myLoc, deltaVec, firstPiece);						//	the distance from the turret to the target
	if (firstPiece () == target ())																								//	if we've got a line on the targeted piece
	{																																							//
		Weapon	*weapon = ((Turret *) turret ())->GetWeapon ();											//	get the weapon
		Real		offAngle = deltaVec | orient,																				//	off angle from the turret's natural position to the target
						wRange = weapon ? weapon->Range () : INFINITY;
		if (offAngle > R(0.875))
			if (R(1.0) < dist && dist < wRange)
			{
				Real	delta = ((Turret *) turret ())->PointAt (tLoc);
				((Turret *) turret ())->Rotate (delta);
				if (FABS (delta) < R(1.0))
					((Turret *) turret ())->Fire ();
				return TRUE;
			}
	}
	return FALSE;																																	//	the target was out of range
}																																								//

//------------------------------------------------------------------------------
/*
Bool	JennyPartThug::ConstrainedTargetPulse (void)																		//	blahblahblah targeting code
{																																								//
	Point_2d	tLoc = target->GetBody ()->TLocation (),														//	get the target's location
						myLoc = turret->GetBody ()->TLocation ();														//	get the turret's location
	Vector_2d deltaVec = tLoc - myLoc;																						//	determine the direction vector from the turret to the target
	deltaVec.Normalize ();																												//	normalize the delta vector
	PiecePtr	firstPiece;
	Real			dist = gReferee->TraceRay (myLoc, deltaVec, firstPiece);						//	the distance from the turret to the target
	if (firstPiece () == target ())																								//	if we've got a line on the targeted piece
	{																																							//
		Weapon	*weapon = ((Turret *) turret ())->GetWeapon ();											//	get the weapon
		Real		offAngle = deltaVec | orient,																				//	off angle from the turret's natural position to the target
						wRange = weapon ? weapon->Range () : INFINITY;
		if (offAngle > R(0.875))
			if (R(1.0) < dist && dist < wRange)
			{
				Real	delta = ((Turret *) turret ())->PointAt (tLoc);
				((Turret *) turret ())->Rotate (delta);
				if (FABS (delta) < R(1.0) && firing)
					((Turret *) turret ())->Fire ();
				return TRUE;
			}
	}
	return FALSE;																																	//	the target was out of range
}																																								//
*/

//------------------------------------------------------------------------------
//	jenny face thug
//------------------------------------------------------------------------------
JFaceThug::JFaceThug (CPoint tLoc, Real rLoc, int type, int id) :
JennyPartThug (id)
{
	orient = Vector_2d (rLoc);
	turret = new JFacePart (this, R(16.0), tLoc, rLoc, gSpriteMgr.Fetch ("turret10"));
	((Turret *) turret ())->AdjustThrust (thrust_fast);
	turret->SetArmor (armor_med);
	SpringGun	*sg = new SpringGun (ZERO, R(2.0e2), turret, R(4.0), ZERO,
																gSpriteMgr.Fetch ("bsplut11"), 36);
	sg->AddAmmoCount (10000);
	((Turret *) turret ())->SetWeapon (sg);
	gReferee->Install (turret);
}

//------------------------------------------------------------------------------
JFaceThug::~JFaceThug (void)
{
}

//------------------------------------------------------------------------------
Bool	JFaceThug::Target (void)
{
	return ConstrainedTarget ();
}

//------------------------------------------------------------------------------
//	jenny jaw thug
//------------------------------------------------------------------------------
JJawThug::JJawThug (CPoint tLoc, Real rLoc, int type, int id) :
JennyPartThug (id)
{
	orient = Vector_2d (rLoc);
	SArrayPtr spr;
	switch (type)
	{
		case	11:
			spr = gSpriteMgr.Fetch ("turret11");
			break;
		case	12:
			spr = gSpriteMgr.Fetch ("turret12");
			break;
		default:
			spr = gSpriteMgr.Fetch ("turret01");
	}
	turret = new JJawPart (this, R(16.0), tLoc, rLoc, spr);
	((Turret *) turret ())->AdjustThrust (thrust_med);
	turret->SetArmor (armor_med);
/*
	SpringGun	*sg = new SpringGun (ZERO, R(1.25e2), turret, R(4.0),
																gSpriteMgr.Fetch ("bsplut11"), 36);
	sg->AddAmmoCount (10000);
	((Turret *) turret ())->SetWeapon (sg);
*/
	gReferee->Install (turret);
	nextPulseTime = gReferee->Clock () + kJawPulseTime;
}

//------------------------------------------------------------------------------
JJawThug::~JJawThug (void)
{
}

//------------------------------------------------------------------------------
//	pulsing modulation and targeting
//------------------------------------------------------------------------------
/*
Bool	JJawThug::Target (void)
{
	Real	clock = gReferee->Clock ();
	if (nextPulseTime < clock)
	{
		nextPulseTime = clock + kJawPulseTime;
		firing = !firing;
	}
	return ConstrainedTargetPulse ();
}
*/

//------------------------------------------------------------------------------
//	jenny mandible thug
//------------------------------------------------------------------------------
JMandibleThug::JMandibleThug (CPoint tLoc, Real rLoc, int type, int id) :
JennyPartThug (id)
{
	orient = Vector_2d (rLoc);
	SArrayPtr spr;
	switch (type)
	{
		case	13:
			spr = gSpriteMgr.Fetch ("turret13");
			break;
		case	14:
			spr = gSpriteMgr.Fetch ("turret14");
			break;
		default:
			spr = gSpriteMgr.Fetch ("turret01");
	}
	turret = new JMandiblePart (this, R(16.0), tLoc, rLoc, spr);
	((Turret *) turret ())->AdjustThrust (thrust_med);
	turret->SetArmor (armor_med);

	EksLaserGun	*lg = new EksLaserGun (R(5.0), R(1.0e2), turret, Point_2d (ZERO, R(8.0)),
										R(1.0), Pixel (23, 31, 15), gSpriteMgr.Fetch ("bsplut11"), 36);
	lg->AddAmmoCount (1000);
	((Turret *) turret ())->SetWeapon (lg);

	gReferee->Install (turret);
}

//------------------------------------------------------------------------------
JMandibleThug::~JMandibleThug (void)
{
}

//------------------------------------------------------------------------------
Bool	JMandibleThug::Target (void)
{
	return NoLeadTarget ();
}

//------------------------------------------------------------------------------
//	jenny leg thug
//------------------------------------------------------------------------------
JLegThug::JLegThug (CPoint tLoc, Real rLoc, int type, int id) :
JennyPartThug (id)
{
	orient = Vector_2d (rLoc);
	Real			rad;
	SArrayPtr	spr;
	switch (type)
	{
		case	15:
			spr = gSpriteMgr.Fetch ("turret15");
			rad = R(16.0);
			break;
		case	16:
			spr = gSpriteMgr.Fetch ("turret16");
			rad = R(16.0);
			break;
		case	17:
			spr = gSpriteMgr.Fetch ("turret17");
			rad = R(16.0);
			break;
		case	18:
			spr = gSpriteMgr.Fetch ("turret18");
			rad = R(16.0);
			break;
		case	21:
			spr = gSpriteMgr.Fetch ("turret21");
			rad = R(24.0);
			break;
		case	22:
			spr = gSpriteMgr.Fetch ("turret22");
			rad = R(24.0);
			break;
		default:
			spr = gSpriteMgr.Fetch ("turret01");
			rad = R(1.0);
	}
	turret = new JLegPart (this, rad, tLoc, rLoc, spr);
	((Turret *) turret ())->AdjustThrust (thrust_med);
	turret->SetArmor (armor_med);
	gReferee->Install (turret);
}

//------------------------------------------------------------------------------
JLegThug::~JLegThug (void)
{
}

//------------------------------------------------------------------------------
//	jenny cilia thug
//------------------------------------------------------------------------------
JCiliaThug::JCiliaThug (CPoint tLoc, Real rLoc, int type, int id) :
JennyPartThug (id)
{
	orient = Vector_2d (rLoc);
	SArrayPtr	spr;
	switch (type)
	{
		case	23:
			spr = gSpriteMgr.Fetch ("turret23");
			break;
		case	24:
			spr = gSpriteMgr.Fetch ("turret24");
			break;
		default:
			spr = gSpriteMgr.Fetch ("turret01");
	}
	turret = new JCiliaPart (this, R(8.0), tLoc, rLoc, spr);
	((Turret *) turret ())->AdjustThrust (thrust_fast);
	turret->SetArmor (armor_low);
	EksFTraik	*tr = new EksFTraik (R(1.0), INFINITY, turret,
									Point_2d (ZERO, R(4.0)), R(5.0e4), R(0.7), R(1.0),
									gSpriteMgr.Fetch ("ekbull"), gSpriteMgr.Fetch ("bsplut20"),
									snd_ekpulse, ZERO);
	tr->SetHeating (R(0.75), R(1.0));
	tr->AddAmmoCount (1000);
	((Turret *) turret ())->SetWeapon (tr);

	gReferee->Install (turret);
}

//------------------------------------------------------------------------------
JCiliaThug::~JCiliaThug (void)
{
}

//------------------------------------------------------------------------------
Bool	JCiliaThug::Target (void)
{
	return NoLeadTarget ();
}

//------------------------------------------------------------------------------
//	jenny fore haunch thug
//------------------------------------------------------------------------------
JHaunchThug::JHaunchThug (CPoint tLoc, Real rLoc, int type, int id) :
JennyPartThug (id)
{
	orient = Vector_2d (rLoc);
	SArrayPtr	spr;
	Real radius;
	switch (type)
	{
		case	19:
			spr = gSpriteMgr.Fetch ("turret19");
			radius = R(48.0);
			break;
		case	20:
			spr = gSpriteMgr.Fetch ("turret20");
			radius = R(48.0);
			break;
		case	25:
			spr = gSpriteMgr.Fetch ("turret25");
			radius = R(40.0);
			break;
		case	26:
			spr = gSpriteMgr.Fetch ("turret26");
			radius = R(40.0);
			break;
		default:
			spr = gSpriteMgr.Fetch ("turret01");
	}
	turret = new JHaunchPart (this, radius, tLoc, rLoc, spr);
	((Turret *) turret ())->AdjustThrust (thrust_slow);
	turret->SetArmor (armor_high);
	gReferee->Install (turret);
}

//------------------------------------------------------------------------------
JHaunchThug::~JHaunchThug (void)
{
}

//------------------------------------------------------------------------------
//	mark's turret
//------------------------------------------------------------------------------
JMarkThug::JMarkThug (CPoint tLoc, Real rLoc, int type, int id) :
JennyPartThug (id)
{
	orient = Vector_2d (rLoc);
	SArrayPtr spr;
	Real radius;
	switch (type)
	{
		case	40:
			radius = R(15.0);
			spr = gSpriteMgr.Fetch ("turret40");
			break;
		
		case	41:
			radius = R(14.0);
			spr = gSpriteMgr.Fetch ("turret41");
			break;
		
//		case	42:
//			spr = gSpriteMgr.Fetch ("turret42");
//			break;
		
		default:
			radius = R(15.0);
			spr = gSpriteMgr.Fetch ("turret01");
	}
	turret = new JMarkPart (this, radius, tLoc, rLoc, spr);
	((Turret *) turret ())->AdjustThrust (thrust_med);
	turret->SetArmor (kMarkPartArmor);
	switch (type)
	{
		case	40:
		{
			EksFTraik	*tr1 = new EksFTraik (R(0.5), INFINITY, turret, Point_2d (R(18.0), R(18.0)),
										 R(5.0e4), R(0.7), R(0.5), gSpriteMgr.Fetch ("ekbull"),
										 gSpriteMgr.Fetch ("bsplut20"), 37, ZERO);
			EksFTraik	*tr2 = new EksFTraik (R(0.5), INFINITY, turret, Point_2d (R(-18.0), R(18.0)),
										 R(5.0e4), R(0.7), R(0.5), gSpriteMgr.Fetch ("ekbull"),
										 gSpriteMgr.Fetch ("bsplut20"), 37, ZERO);
//			tr1->SetHeating (R(0.75), R(1.0));
//			tr2->SetHeating (R(0.75), R(1.0));
			Cluster	*c1 = new Cluster (turret, 2, tr1, tr2);
			c1->AddAmmoCount (1000);
			((Turret *) turret ())->SetWeapon (c1);
		}	break;
		
		case	41:
		{
			EksLaserGun	*lg1 = new EksLaserGun (30, R(1.8e2), turret, Point_2d (R(11.0), R(27.0)),
											 R(0.5), Pixel (15, 15, 0), gSpriteMgr.Fetch (ParmString (LASER_SPLUT)), 19);
			EksLaserGun	*lg2 = new EksLaserGun (30, R(1.8e2), turret, Point_2d (R(-11.0), R(27.0)),
											 R(0.5), Pixel (15, 15, 0), gSpriteMgr.Fetch (ParmString (LASER_SPLUT)), 19);
			Cluster *c1 = new Cluster (turret, 2, lg1, lg2);
			c1->AddAmmoCount (1000);
			((Turret *) turret ())->SetWeapon (c1);
		}	break;
		
//		case	42:
//			break;
		
		default:
			break;
	}

	gReferee->Install (turret);
}

//------------------------------------------------------------------------------
JMarkThug::~JMarkThug (void)
{
}

//------------------------------------------------------------------------------
