//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//	Modified:				2/9/97 (MK) - Changed PlaySound Call in PostCollide to include piece information
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "MineBullet.h"
#include "rotating.h"
#include "circle.h"
#include "pi.h"
#include "piece ptr.h"

#include "view.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "particle sequence.h"
#include "moving sequence.h"
#include "sprite array mgr.h"
#include "random.h"
#include "sound mgr.h"
#include "referee.h"

#include "fireball bullet.h"
#include "fireball visual.h"

#define  MBEXPLOSOUND 9


//------------------------------------------------------------------------------
MineBullet::MineBullet (Real rad, CPoint loc, Real rloc, SArrayPtr spr, Real dam,				//	constructor
								const PiecePtr &shoot, SArrayPtr splt) :												//	constructor (continued)
Piece (shoot () ? shoot->GetOwner () : 0)																				//	constructor (continued)
{																																								//	begin
	Real	mass = PI * rad * rad * R(1.0e2);																				//	compute the mass as the area of the plate times one hundred
	body = new Body (loc, rloc, mass, R(0.5) * mass * rad * rad);									//	allocate the body, rInertia is of a solid sphere
	visual = new FireBall (body, spr);																						//	allocate the visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	damage = dam;																																	//	save the damage potential of this bullet
	life = R(4.0);																																//	bullet has a lifetime of 4 seconds
	shooter = shoot;																															//	copy the shooter
	splut = splt;
}																																								//	end

//------------------------------------------------------------------------------
MineBullet::~MineBullet (void)																													//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Bool	MineBullet::Step (Real time)																									//	step the piece ahead in time
{																																								//	begin
	life -= time;																																	//	subtract the time from the life value
	if (life < ZERO)																															//	if the life is negative
		life = ZERO;																																//	set life to zero
	return Piece::Step (time);																										//	do a noremal step
}																																								//	end

//------------------------------------------------------------------------------
int		MineBullet::PostCollide (const PiecePtr &piece, Sect *sect)										//	do some work after a collision with another piece
{																																								//	begin
	gSoundMgr->PlaySound (MBEXPLOSOUND, this);
	Point_2d	hitPt = sect->point + (Vector_2d (gRandom.RndReal () - R(0.5), gRandom.RndReal () - R(0.5)) * R(1.0e1));
	Vector_2d	victVel;
	if (piece->GetBody ())
		victVel = piece->GetBody ()->TVelocity ();
	else
		victVel (ZERO, ZERO);
	gView->Install (new MoveSeq (new TimeSeq (splut), hitPt, FALSE, 3, Vector_2d (ZERO, ZERO), gReferee->Interval (), victVel, splut->Count () * gReferee->Interval (), Vector_2d (ZERO, ZERO)));
	//gView->Install (SeqPtr (new StatSeq (sect->point + (Vector_2d (gRandom.RndReal (), gRandom.RndReal ()) * R(5.0)), new TimeSeq (splut))));
	
	// (MK)
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
	//	life = ZERO;
		piece->Damage (shooter, damage);																						//	damage the piece we just hit
		return PIECE_DEAD;
	// (MK)
	
	
	
}																																								//	end

//------------------------------------------------------------------------------
