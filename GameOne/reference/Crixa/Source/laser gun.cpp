//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "laser gun.h"
#include "view.h"
#include "rotating.h"
#include "random.h"
#include "referee.h"
#include "moving sequence.h"
#include "stationary sequence.h"
#include "particle sequence.h"
#include "beam sequence.h"
#include "time sequence.h"
#include "sprite array mgr.h"
#include "pi.h"

//------------------------------------------------------------------------------
LaserGun::LaserGun (Real rat, Real ran, const PiecePtr &own, CPoint muz,				//	constructor
										Real dam, Pixel col, SArrayPtr splt, int snd) :							//	constructor	(continued)
Gun (rat, ran, own, muz, ZERO, ZERO, dam, snd)																	//	constructor (continued)
{																																								//	begin
	color = col;																																	//	copy the color
	splut = splt;
	velocity = INFINITY;																													//	set the velocity of the weapon to be inifinity
}																																								//	end

//------------------------------------------------------------------------------
LaserGun::~LaserGun (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	LaserGun::Shoot (void)																										//	shoot the projectile
{																																								//	begin
/*
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
*/
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	Vector_2d	norm (ownerBody->RLocation ()),
						perp (norm[Y], -norm[X]),
						muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Point_2d	muzzlePt = ownerBody->TLocation () + muzzleOffset;									//	compute the actual muzzle point
	PiecePtr	victim;
	Real			time = gReferee->TraceRay (muzzlePt, norm, victim);
	if (time > range)
		time = range;
	Vector_2d	distance = time * norm;
	Point_2d	hitPt = muzzlePt + distance;
	if (time < range)
	{
		Vector_2d	victVel;
		if (victim->GetBody ())
			victVel = victim->GetBody ()->TVelocity ();
		else
			victVel (ZERO, ZERO);
		gView->Install (new MoveSeq (new TimeSeq (splut), hitPt, FALSE, 3, Vector_2d (ZERO, ZERO), gReferee->Interval (), victVel, splut->Count () * gReferee->Interval (), Vector_2d (ZERO, ZERO)));
		victim->Damage (owner, damage);
		for (int i = 0; i < 3; i++)
		{
			Real		multiplier = (gRandom.RndInteger () & 0x01) ? R(1.0) : R(-1.0),
							rand = (R(1.0) - POW (gRandom.RndReal (), R(0.33333333))) * multiplier,
							theta = ACOS (rand) - PI_OVER_TWO;
			Vector_2d	dir = Rotate (Vector_2d (theta), norm);
			dir *= (gRandom.RndReal () + R(0.25)) * R(-2.0e2);
			gView->Install (new PartSeq (hitPt, dir, color, R(0.5) + gRandom.RndReal (), 4));
		}
	}
	gView->Install (new BeamSeq (time, color, rate + R(0.25), muzzle, owner), UNDER_LAYER);
}																																								//	end

//------------------------------------------------------------------------------
