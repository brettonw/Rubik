//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/21/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "mine launcher.h"
#include "mine.h"
#include "independent.h"
#include "mine player.h"
#include "rotating.h"
#include "referee.h"

//------------------------------------------------------------------------------
MineL::MineL (Real rat, const PiecePtr &own, CPoint muz, Real r, Real d,				//	constructor 
							SArrayPtr min, int snd) :																					//	constructor (continued)
Gun (rat, ZERO, own, muz, ZERO, r, d, snd)																			//	constructor (continued)
{																																								//	begin
	mineSprite = min;																															//	copy the mine sprite array
	velocity = ZERO;																															//	set the initial velocity to zero
}																																								//	end

//------------------------------------------------------------------------------
MineL::~MineL (void)																														//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	MineL::Shoot (void)																												//	shoot the projectile
{																																								//	begin
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Point_2d	muzPt = ownerBody->TLocation () + muzzleOffset;											//	compute the actual muzzle point
	Mine			*mine = new Mine (owner->GetOwner (), radius, muzPt, mineSprite, damage);		//	create a new mine
	Body			*mineBody = mine->GetBody ();																				//	get the mine's body
	mineBody->AddVelocity ((muzzleOffset * R(10.0)) + ownerBody->TVelocity (), ZERO);
	gReferee->Install (mine);																											//	put the mine into the referee
	MinePlay	*player = new MinePlay (mine, R(0.5), R(1.0e0));
	gIndependent->Install (player);
}																																								//	end

//------------------------------------------------------------------------------
