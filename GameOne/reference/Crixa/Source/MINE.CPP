//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//	Modified by:		Mark Kern
//	Last Revision		01/09/97		Added PiecePtr info to all PlaySound calls (2) 
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "mine.h"
#include "sequence visual.h"
#include "direction.h"
#include "circle.h"
#include "pi.h"
#include "piece ptr.h"
#include "quadratic.h"
#include "target lock.h"
#include "view.h"
#include "fireball bullet.h"
#include "referee.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "sprite array mgr.h"
#include "particle sequence.h"
#include "sound mgr.h"
#include "random.h"

//------------------------------------------------------------------------------
Mine::Mine (Player *own, Real rad, CPoint loc, SArrayPtr spr, Real dam) :				//	constructor
Piece (own)																																			//	constructor (continued)
{																																								//	begin
	body = new Body (loc, ZERO, PI * rad * rad * R(1.0e1), INFINITY);							//	allocate the body
	visual = new SeqVisual (body, new TimeSeq (spr, TRUE, 3, 0, R(0.25), 1, ZERO, 2, ZERO));								//	allocate the visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	damage = dam;																																	//	save the damage potential of this Mine
	damaged = FALSE;																															//	the mine has not been damaged
}																																								//	end

//------------------------------------------------------------------------------
Mine::~Mine (void)																															//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
int		Mine::PreCollide (const PiecePtr &piece, Sect *sect)											//	do some work before a collision with another piece
{																																								//	begin
	if (piece->GetBody () && (piece->GetOwner () != owner))												//	if the contacting piece is a valid target
	{
		target = new TLock (piece);																									//	lock on to the piece
		gSoundMgr->PlaySound (15,this);
	}
	return Piece::PreCollide (piece, sect);																				//	return the parental results of the pre-collision
}																																								//	end

//------------------------------------------------------------------------------
void	Mine::Damage (const PiecePtr &piece, Real damage)													//	damage the piece
{																																								//	begin
	damaged = TRUE;																																//	set the damaged flag
}																																								//	end

//------------------------------------------------------------------------------
Real	Mine::TraceRay (CPoint origin, CVector direction)													//	return the time at which a ray intersects this piece
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the ship
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
void	Mine::Explode (void)																											//	explode the mine
{																																								//	begin
	if (life > ZERO)
	{
		gSoundMgr->PlaySound (7,this);
		Real			radius = ((Circle *) geometry)->Radius ();
		gView->Install (SeqPtr (new StatSeq (body->TLocation (), new TimeSeq (gSpriteMgr.Fetch ("explo1")))));
		for (int i = 0; i < 1024; i += 128)
		{
			int					index = i + (gRandom.RndInteger () >> 3);
			Real				force = R(3.0e5) * (gRandom.RndReal () + R(0.25));
			Real				partMass = PI * R(1.0e2);
			Vector_2d		offset = gDirection[index][0],
									rVel = (offset * (force / partMass)) + body->TVelocity ();
			FireBullet	*part = new FireBullet (R(1.0), body->TLocation () + (offset * (radius * R(9.0e-1))), gSpriteMgr.Fetch ("plasma2"), damage, this, gSpriteMgr.Fetch ("rsplut8"));	//	create a new bullet
			Body				*partBody = part->GetBody ();														//	get the bullet's body
			partBody->AddVelocity (rVel, ZERO);											//	add the velocity of the owner to the bullet
			gReferee->Install (part, MOBILE_INTERSECT);														//	put the bullet into the referee
		}
		Real			vel = (gRandom.RndReal () + R(0.25)) * R(2.5e2);
		for (i = 0; i < 24; i++)
		{
			Vector_2d	dir = gDirection[gRandom.RndInteger ()][0] * vel;
			gView->Install (new PartSeq (body->TLocation (), body->TVelocity () + dir, MakePixel (31, 19, 0), R(1.0) + gRandom.RndReal (), 4));
		}
		life = ZERO;
	}
}																																								//	end

//------------------------------------------------------------------------------
