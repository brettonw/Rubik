//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//  Revised by:			Mark Kern
//  Last Revision:  02/09/97			Added PiecePtr info to explosion sound
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "ship explosion sequence.h"
#include "circle.h"
#include "referee.h"
#include "view.h"
#include "sound mgr.h"
#include "sprite array mgr.h"
#include "particle sequence.h"
#include "time sequence.h"
#include "moving sequence.h"
#include "particle.h"
#include "fireball bullet.h"
#include "direction.h"
#include "pi.h"
#include "random.h"

//------------------------------------------------------------------------------
ShipExpl::ShipExpl (const PiecePtr &ship) :																			//	constructor
ExplSeq (ship->GetBody ()->TLocation (), ship->GetBody ()->TVelocity (),				//	constructor (continued)
					ship->GetBody ()->TInertia () * R(2.5e-4),														//	constructor (continued)	
					((Circle *) ship->GetGeometry ())->Radius (), 2)											//	constructor (continued)
{																																								//	begin
	elapsed = 0;																																	//	the explosion is just starting
	particles = int (ship->GetBody ()->TInertia () * R(5.0e-3));
	//particles = int (ship->GetBody ()->TInertia () * R(1.0e-2));
	big[0] = gSpriteMgr.Fetch ("explo1");
	big[1] = gSpriteMgr.Fetch ("explo3");
	little[0] = big[0];
	little[1] = gSpriteMgr.Fetch ("explo2");
	little[2] = gSpriteMgr.Fetch ("plasma");
	little[3] = little[1];
	gSoundMgr->PlaySound (6, ship);
}																																								//	end

//------------------------------------------------------------------------------
ShipExpl::~ShipExpl (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	ShipExpl::Pop (void)																											//	make a new pop
{																																								//	begin
	if (elapsed < pops)
		BigPop ();
	else
		if (timeLeft < gReferee->Interval ())
			BigPop ();
		else
			SmallPop ();
	elapsed++;
}																																								//	end

//------------------------------------------------------------------------------
void	ShipExpl::BigPop (void)																										//	a big lobe in the explosion
{																																								//	begin
	int				index = gRandom.RndInteger ();
	Vector_2d	offset = gDirection[index][0];
	int				layer = (gRandom.RndInteger () & 1) ? OVER_LAYER : UNDER_LAYER;
	gView->Install (SeqPtr (new MoveSeq (new TimeSeq (big[index & 1]), location + (offset * radius * R(0.5)), FALSE, 3, Vector_2d (ZERO, ZERO), gReferee->Interval (), velocity, R(0.15), Vector_2d (ZERO, ZERO))), layer);
	for (int i = 0; i < particles; i++)
	{
		index = gRandom.RndInteger ();
		Real				force = R(2.5e5) * (gRandom.RndReal () + R(0.5)),
								partMass = PI * R(1.25) * R(1.25) * R(1.0e2);
		Vector_2d		offset = gDirection[index][0],
								rVel = (offset * (force / partMass)) + velocity;
		FireBullet	*particle = new FireBullet (R(1.25), location + (offset * (radius * R(9.0e-1))), gSpriteMgr.Fetch ("plasma4"), R(0.25), PiecePtr (0), gSpriteMgr.Fetch ("rsplut8"));	//	create a new bullet
		Body				*particleBody = particle->GetBody ();														//	get the bullet's body
		particleBody->AddVelocity (rVel, ZERO);																		//	add the velocity of the owner to the bullet
		gReferee->Install (particle, MOBILE_INTERSECT);														//	put the bullet into the referee
	}
	for (i = 0; i < particles; i++)
	{
		Real			vel = (gRandom.RndReal () + R(0.25)) * R(-3.5e2);
		Vector_2d	dir = gDirection[gRandom.RndInteger ()][0] * vel;
		gView->Install (new PartSeq (location, velocity + dir, MakePixel (31, 0, 0), R(0.5) + gRandom.RndReal (), 3), (gRandom.RndInteger () & 1) ? OVER_LAYER : UNDER_LAYER);
	}
}																																								//	end

//------------------------------------------------------------------------------
void	ShipExpl::SmallPop (void)																									//	a small lobe in the explosion
{																																								//	begin
	Real			force = R(3.0e5) * (gRandom.RndReal () + R(0.25));
	int				index = gRandom.RndInteger ();
	Vector_2d	offset = gDirection[index][0];
	Particle	*particle = new Particle (location + (offset * (radius * R(9.0e-1))), MakePixel (31, 16 + (index & 15), 0), R(0.125), R(0.5) + (R(0.5) * gRandom.RndReal ()));	//	create a new bullet
	Body			*particleBody = particle->GetBody ();														//	get the bullet's body
	particleBody->ApplyForce (offset * force);																//	apply the shot force to the bullet
	particleBody->AddVelocity (velocity, ZERO);											//	add the velocity of the owner to the bullet
	gReferee->Install (particle, MOBILE_INTERSECT);														//	put the bullet into the referee
	int				layer = (gRandom.RndInteger () & 1) ? OVER_LAYER : UNDER_LAYER;
	gView->Install (SeqPtr (new MoveSeq (new TimeSeq (little[index & 2]), location + (offset * radius), FALSE, 3, Vector_2d (ZERO, ZERO), gReferee->Interval (), velocity, R(0.1), Vector_2d (ZERO, ZERO))), layer);
}																																								//	end

//------------------------------------------------------------------------------
