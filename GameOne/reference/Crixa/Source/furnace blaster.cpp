//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/05/97:	creation.  (JB)
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "furnace blaster.h"
#include "rotating.h"
#include "get parameters.h"
#include "random.h"
#include "plasma bubble.h"
#include "view.h"

//------------------------------------------------------------------------------
FurnaceBlaster::FurnaceBlaster (Real ran, const PiecePtr &own, CPoint muz,			//	constructor
																Real s, Real d, SArrayPtr plas, int snd) :			//	constructor (continued)
PlasmaGun (ran, own, muz, s, d, plas, snd)																			//	constructor (continued)
{																																								//	begin
	numBubbles = ParmInteger (PLASMA_BUBBLES);																		//	grab the number of bubbles per shot
}																																								//	end

//------------------------------------------------------------------------------
FurnaceBlaster::~FurnaceBlaster (void)																					//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	NOTE:	this is almost exactly like PlasmaGun::Shoot (), but with more jitter.
//				a result of a hack, this function would benefit from a modification of
//				the plasma gun to allow for variable jitter.  this will do for now.
//------------------------------------------------------------------------------
void	FurnaceBlaster::Shoot (void)																							//	blast the furnace
{																																								//	begin
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Point_2d	muzzlePt = ownerBody->TLocation () + muzzleOffset;									//	compute the actual muzzle point
	for (int i = 0; i < numBubbles; i++)																					//	bubble creation loop
	{																																							//	begin
		Real	dist = gRandom.RndReal () + R(0.1);																		//	create a random distance for the bubble to appear at
		Vector_2d	jitter = (perp / dist) * (R(25.0) * (gRandom.RndReal () - R(0.5)));//	add some fluctuation to the bubble's point
		Point_2d	plasmaPt = muzzlePt + (norm * (dist * range)) + jitter;						//	calculate the plasma bubble's creation point (from the muzzle point)
		Vector_2d	deltaVec = (plasmaPt - muzzlePt) / speed;													//	calculate the bubble's spped vector
		PBubble	*bubble = new PBubble (plasmaSprite, muzzlePt, deltaVec +						//	create a new bubble
											ownerBody->TVelocity (), damage, owner, speed);						//	create a new bubble (continued)
		gView->Install (SeqPtr (bubble));																						//	install the bubble into the view sequencer (big hack)
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
