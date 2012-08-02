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
#include "flechette.h"
#include "rotating.h"
#include "direction.h"
#include "random.h"
#include "referee.h"
#include "view.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "moving sequence.h"
#include "particle sequence.h"
#include "sprite array mgr.h"
#include "pi.h"

//------------------------------------------------------------------------------
Flechette::Flechette (Real rat, Real ran, const PiecePtr &own, CPoint muz,			//	constructor
											Real frc, Real dam, int num, Real spr, Pixel col,					//	constructor (continued)
											SArrayPtr bullt, SArrayPtr splt, int snd) :								//	constructor (continued)
Gun (rat, ran, own, muz, frc, ZERO, dam, snd)																		//	constructor (continued)
{																																								//	begin
	numBlasts = num;																															//	copy the number of blasts
	spread = spr;																																	//	copy the spread angle
	color = col;																																	//	copy the particle color
	bullet = bullt;																																//	copy the bullet sprite
	splut = splt;																																	//	copy the splut sprite
}																																								//	end

//------------------------------------------------------------------------------
Flechette::~Flechette (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Flechette::Shoot (void)																										//	shoot the projectile
{																																								//	begin
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	Point_2d	muzzlePt = ownerBody->TLocation () + muzzleOffset;									//	compute the actual muzzle point
	int				index = rot->GetCurrent () << rot->Shift ();
	Real			hitForce = force * R(5.0e0);
	for (int i = 0; i < numBlasts; i++)
	{
		Real		multiplier = (gRandom.RndInteger () & 0x01) ? R(1.0) : R(-1.0),
						rand = (R(1.0) - POW (gRandom.RndReal (), spread)) * multiplier,
						theta = ACOS (rand) - PI_OVER_TWO;
		Vector_2d	direction = Rotate (Vector_2d (theta), norm);

		PiecePtr	victim;
		Real			time = gReferee->TraceRay (muzzlePt, direction, victim);
		if (time > range)
			time = range;
		Point_2d	hitPt = muzzlePt + (time * direction);
		if (time < range)
		{
			Vector_2d	victVel;
			if (victim->GetBody ())
				victVel = victim->GetBody ()->TVelocity ();
			else
				victVel (ZERO, ZERO);
			gView->Install (new MoveSeq (new TimeSeq (splut), hitPt, FALSE, 3, Vector_2d (ZERO, ZERO), gReferee->Interval (), victVel, splut->Count () * gReferee->Interval (), Vector_2d (ZERO, ZERO)));
//			gView->Install (new StatSeq (hitPt, new TimeSeq (splut)));
			victim->Damage (owner, damage);
			Body	*victimBody = victim->GetBody ();
			if (victimBody)
			{
				victimBody->ApplyForce (direction * hitForce, hitPt);
				gReferee->Update (victim);
			}
		}
		Vector_2d	vel = gRandom.RndReal () * R(0.6) * time * direction;
		Point_2d	showPt = muzzlePt + vel;
		Real	angle = ATAN2 (direction[Y], direction[X]);
		while (angle < ZERO)
			angle += TWO_PI;
		angle *= OVER_TWO_PI;
		int	index = int ((angle * bullet->Count ()) + R(0.5));
		gView->Install (new StatSeq (showPt, new TimeSeq (bullet, FALSE, 1, index, gReferee->Interval () * R(0.2))));
		gView->Install (new PartSeq (muzzlePt, ownerBody->TVelocity () + (vel * R(3.0)), color, R(0.5) + gRandom.RndReal (), 4));
		gView->Install (new PartSeq (showPt, ownerBody->TVelocity () + (vel * R(2.0)), color, R(0.5) + gRandom.RndReal (), 4));
		if (ownerBody)
			ownerBody->ApplyForce (direction * -force, muzzlePt);
	}
}																																								//	end

//------------------------------------------------------------------------------
Real	Flechette::Velocity (void)																								//	return the muzzle velocity of the weapon
{																																								//	begin
	return INFINITY;																															//	return infinite velocity
}																																								//	end

//------------------------------------------------------------------------------
