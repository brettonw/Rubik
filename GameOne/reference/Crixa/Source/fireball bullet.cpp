//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//	02/09/97	MK		Added PiecePtr info to Fireball PlaySound call when fireball hits object.
//	02/28/97	DS		
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "fireball bullet.h"
#include "circle.h"
#include "pi.h"
#include "piece ptr.h"
#include "view.h"
#include "time sequence.h"
#include "moving sequence.h"
#include "sprite array mgr.h"
#include "random.h"
#include "sound mgr.h"
#include "fireball visual.h"
#include "referee.h"

//------------------------------------------------------------------------------
FireBullet::FireBullet (Real rad, CPoint loc, SArrayPtr spr, Real dam,					//	constructor
								const PiecePtr &shoot, SArrayPtr splt) :												//	constructor (continued)
Piece (shoot () ? shoot->GetOwner () : 0)																				//	constructor (continued)
{																																								//	begin
	Real	mass = PI * rad * rad * R(1.0e2);																				//	compute the mass as the area of the plate times one hundred
	body = new Body (loc, ZERO, mass, INFINITY);																	//	allocate the body, rInertia is infinite
	visual = new FireBall (body, spr);
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	damage = dam;																																	//	save the damage potential of this bullet
	life = R(3.0);																																//	bullet has a lifetime of 3 seconds
	shooter = shoot;																															//	copy the shooter
	splut = splt;
}																																								//	end

//------------------------------------------------------------------------------
FireBullet::~FireBullet (void)																									//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Bool	FireBullet::Step (Real time)																							//	step the piece ahead in time
{																																								//	begin
	life -= time;																																	//	subtract the time from the life value
	if (life < ZERO)																															//	if the life is negative
		life = ZERO;																																//	set life to zero
	return Piece::Step (time);																										//	do a noremal step
}																																								//	end

//------------------------------------------------------------------------------
int		FireBullet::PostCollide (const PiecePtr &piece, Sect *sect)								//	do some work after a collision with another piece
{																																								//	begin
	gSoundMgr->PlaySound (12, this);																							// Sound of Fireball bullet hitting an object
	Point_2d	hitPt = sect->point + (Vector_2d (gRandom.RndReal () - R(0.5), gRandom.RndReal () - R(0.5)) * R(1.0e1));
	Vector_2d	victVel;
	if (piece->GetBody ())
		victVel = piece->GetBody ()->TVelocity ();
	else
		victVel (ZERO, ZERO);
	gView->Install (new MoveSeq (new TimeSeq (splut), hitPt, FALSE, 3, Vector_2d (ZERO, ZERO), gReferee->Interval (), victVel, splut->Count () * gReferee->Interval (), Vector_2d (ZERO, ZERO)));
	//gView->Install (SeqPtr (new StatSeq (sect->point + (Vector_2d (gRandom.RndReal (), gRandom.RndReal ()) * R(5.0)), new TimeSeq (splut))));
	piece->Damage (shooter, damage);																						//	damage the piece we just hit
	return PIECE_DEAD;																													//	the bullet goes away now, so return dead
}																																								//	end

//------------------------------------------------------------------------------
