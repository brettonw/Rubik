//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/16/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "rocket launcher.h"
#include "rocket.h"
#include "independent.h"
#include "rocket player.h"
#include "rotating.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
RocketL::RocketL (const PiecePtr &own, Real rat, Real rad, CPoint muz,					//	constructor 
									Real dam, SArrayPtr rocket, Real tThr, Real rThr, int snd) :	//	constructor (continued)
Gun (rat, INFINITY, own, muz, ZERO, rad, dam, snd)															//	constructor (continued)
{																																								//	begin
	rocketSpr = rocket;																														//	copy the sprite
	tThrust = tThr;
	rThrust = rThr;
}																																								//	end

//------------------------------------------------------------------------------
RocketL::~RocketL (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	RocketL::SetLock (const LockPtr &loc)																			//	set the weapon lock, if applicable
{																																								//	begin
	lock = loc;																																		//	copy the lock
}																																								//	end

//------------------------------------------------------------------------------
void	RocketL::Shoot (void)																											//	shoot the projectile
{																																								//	begin
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Point_2d	muzPt = ownerBody->TLocation () + muzzleOffset;											//	compute the actual muzzle point
	Rocket		*rocket = new Rocket (owner->GetOwner (), radius, muzPt, ownerBody->RLocation (), rocketSpr, gSpriteMgr.Fetch (ParmString(ROCKET_TRAIL_SPRITE)), damage);
	rocket->AdjustThrust (tThrust, rThrust);
	Body			*rockBody = rocket->GetBody ();
	rockBody->AddVelocity ((muzzleOffset * R(2.0)) + ownerBody->TVelocity (), ZERO);
	gReferee->Install (rocket);
	RockPlay	*player = new RockPlay (rocket, lock, R(0.25));
	gIndependent->Install (player);
}																																								//	end

//------------------------------------------------------------------------------
