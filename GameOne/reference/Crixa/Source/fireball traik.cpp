//------------------------------------------------------------------------------
//	Copyright �1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/01/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "fireball traik.h"
#include "fireball bullet.h"
#include "rotating.h"
#include "direction.h"
#include "referee.h"
#include "pi.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
FTraik::FTraik (Real rat, Real ran, const PiecePtr &own, CPoint muz, Real f,		//	constructor
							Real r, Real d, SArrayPtr bul, SArrayPtr splt, int snd, Real rec)://	constructor (continued)
Gun (rat, ran, own, muz, f, r, d, snd)																					//	constructor (continued)
{																																								//	begin
	recoil = rec;																																	//	copy the recoil factor
	bulletSprite = bul;																														//	copy the bullet sprite array
	splutSprite = splt;																														//	copy the bullet splut sprite
	overMass = R(1.0) / (r * r * PI * R(1.0e2));																	//	compute the reciprocal of the mass of the bullet
	velocity = f * overMass;																											//	compute the muzzle velocity of the bullet
}																																								//	end

//------------------------------------------------------------------------------
FTraik::~FTraik (void)																													//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	FTraik::Shoot (void)																											//	shoot the weapon projectile
{																																								//	begin
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]),							//	compute the offset of the muzzle point
						F = norm * force,																										//	compute the firing force for the weapon
						fVel = (norm * velocity) + ownerBody->TVelocity ();									//	compute the resultant velocity
	Real			rLoc = ATAN2 (fVel[Y], fVel[X]);																		//	compute the angle of the resultant velocity
	while (rLoc < ZERO)
		rLoc += TWO_PI;
	Point_2d		muzPt = ownerBody->TLocation () + muzzleOffset;										//	compute the actual muzzle point
	FireBullet	*bull = new FireBullet (radius, muzPt,														//	create a new bullet
																bulletSprite, damage, owner, splutSprite);			//	create a new bullet	 (continued)
	Body			*bulletBody = bull->GetBody ();																			//	get the bullet's body
	bulletBody->ApplyForce (F);																										//	apply the shot force to the bullet
	ownerBody->ApplyForce (F * recoil, muzPt);																		//	apply the recoil force to the owner
	bulletBody->AddVelocity (ownerBody->TVelocity (), ZERO);											//	add the velocity of the owner to the bullet
	gReferee->Install (bull, MOBILE_INTERSECT);																		//	put the bullet into the referee
}																																								//	end

//------------------------------------------------------------------------------
