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
#include "plasma gun.h"
#include "direction.h"
#include "referee.h"
#include "rotating.h"
#include "get parameters.h"
#include "random.h"
#include "view.h"
#include "plasma bubble.h"

//------------------------------------------------------------------------------
PlasmaGun::PlasmaGun (Real ran, const PiecePtr &own, CPoint muz, Real s, Real d,//	constructor
								SArrayPtr plas, int snd):																				//	constructor (continued)
Gun (ZERO, ran, own, muz, ZERO, ZERO, d, snd)																		//	constructor (continued)
{																																								//	begin
	plasmaSprite = plas;																													//	copy the plasma sprite array
	speed = s;																																		//	copy the blossom speed
	velocity = range / speed;																											//	set the muzzle velocity to the range travelled at speed
}																																								//	end

//------------------------------------------------------------------------------
PlasmaGun::~PlasmaGun (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	PlasmaGun::Shoot (void)																										//	shoot the projectile
{																																								//	begin
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Point_2d	muzzlePt = ownerBody->TLocation () + muzzleOffset;									//	compute the actual muzzle point
	for (int i = 0; i < ParmInteger (PLASMA_BUBBLES); i++)
	{
		Real	distance = gRandom.RndReal () + R(0.1);
		Vector_2d	jitter = (perp / distance) * (R(2.0) * (gRandom.RndReal () - R(0.5)));
		Point_2d	plasmaPt = muzzlePt + (norm * (distance * range)) + jitter;
		Vector_2d	deltaVec = (plasmaPt - muzzlePt) / speed;
		PBubble	*bubble = new PBubble (plasmaSprite, muzzlePt, deltaVec + ownerBody->TVelocity (), damage, owner, speed);
		gView->Install (SeqPtr (bubble));
	}
}																																								//	end

//------------------------------------------------------------------------------
