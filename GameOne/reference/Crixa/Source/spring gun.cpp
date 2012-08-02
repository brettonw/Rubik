//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/20/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "spring gun.h"
#include "spring.h"
#include "view.h"
#include "rotating.h"
#include "random.h"
#include "referee.h"
#include "stationary sequence.h"
#include "beam sequence.h"
#include "time sequence.h"
#include "fade pixel sequence.h"
#include "world.h"
#include "pi.h"

//------------------------------------------------------------------------------
SpringGun::SpringGun (Real rat, Real ran, const PiecePtr &own, Real hol,				//	constructor 
											Real stn, SArrayPtr splt,	int snd) :											//	constructor	(continued)
Gun (rat, ran, own, Point_2d (ZERO, ZERO), ZERO, ZERO, ZERO, snd)								//	constructor (continued)
{																																								//	begin
	holdTime = hol;
	stunTime = stn;
	splut = splt;
	velocity = INFINITY;																													//	set the velocity of the weapon to be inifinity
	hold = ZERO;
	shotSound = snd;																															//  set the sound
}																																								//	end

//------------------------------------------------------------------------------
SpringGun::~SpringGun (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	SpringGun::Reload (Real time)																							//	reload the weapon
{																																								//	begin
	Gun::Reload (time);																														//	do the parental thing
	hold -= time;																																	//	count down the target acquisition counter
}																																								//	end

//------------------------------------------------------------------------------
void	SpringGun::Shoot (void)																										//	shoot the projectile
{																																								//	begin
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	Vector_2d	norm (ownerBody->RLocation ());
	if ((hold > ZERO) && gReferee->Confirm (target) && (target->GetGeometry ()->Type () == CIRCLE_GEOM))
	{
		Body	*targetBody = target->GetBody ();
		Vector_2d	delta = targetBody->TLocation () - ownerBody->TLocation ();
		if (gWorld->TraceRay (ownerBody->TLocation (), delta) > R(1.0))
		{
			hold = holdTime;
			DrawSpring (targetBody->TLocation (), ownerBody->TLocation (), 24, MakePixel (31, 31, 0));
			Real	m1 = ownerBody->TInertia (),
						m2 = targetBody->TInertia (),
						c = (m1 * m2) / (2 * (m1 + m2));
			Spring (owner, target, R(1.0e3), c, range * R(0.5));											//	original spring calculation
			gReferee->Update (target);
			gReferee->Update (owner);
		}
	}
	else
	{
		PiecePtr	victim;
		Real			time = gReferee->TraceRay (ownerBody->TLocation (), norm, victim);
		if (time > range)
			time = range;
		Point_2d	targetPt = ownerBody->TLocation () + (time * norm);
		if (time < range)
		{
			target = victim;
			target->Stun (stunTime);
			gView->Install (new StatSeq (targetPt, new TimeSeq (splut)));
			hold = holdTime;
		}
		else
			target = PiecePtr (0);
		gView->Install (new BeamSeq (time, MakePixel (16, 16, 0), rate * R(0.5) + R(0.1), muzzle, owner), UNDER_LAYER);
	}
}																																								//	end

//------------------------------------------------------------------------------
