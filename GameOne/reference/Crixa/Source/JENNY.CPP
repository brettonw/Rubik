//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/04/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "jenny.h"
#include "rotating.h"
#include "circle.h"
#include "pi.h"
#include "eksekaren.h"
#include "quadratic.h"

//	hacks for explosions
#include "view.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "particle.h"
#include "bullet.h"
#include "direction.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "random.h"
#include "pi.h"
#include "circle.h"

//------------------------------------------------------------------------------
Jenny::Jenny (Real rad, CPoint tLoc, Real rLoc, SArrayPtr sprites) : Piece (0)	//	constructor
{																																								//	begin
	//Real	mass = PI * rad * rad * R(1.0e3);																									//	compute the mass as the area of the plate
	//body = new Body (tLoc, rLoc, mass, R(0.5) * mass * rad * rad);								//	allocate the body, rInertia is of a solid sphere
	body = new Body (tLoc, rLoc, INFINITY, INFINITY);															//	allocate the body, totally immobile
	visual = new Rotating (body, sprites);																				//	allocate the visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	deadTime = INFINITY;
}																																								//	end

//------------------------------------------------------------------------------
Jenny::~Jenny (void)																														//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Bool	Jenny::Step (Real time)																										//	step the piece ahead in time
{																																								//	begin
	return Piece::Step (time);
	body->Step (time);																														//	step the body ahead in time
	if (deadTime < INFINITY)
	{
		deadTime -= time;
		if (lastPop - deadTime > R(0.005))
		{
			lastPop = deadTime;
			static		cstr	explo[2] = {"explo2", "explo1"};
			Real			radius = ((Circle *) geometry)->Radius (),
								force = R(3.0e5) * (gRandom.RndReal () + R(0.25));
			int				index = gRandom.RndInteger ();
			Vector_2d	offset = gDirection[index][0];
			Particle	*particle = new Particle (body->TLocation () + (offset * radius), MakePixel (31, 8 + (index & 15), 0), R(0.125), R(0.5) + (R(0.5) * gRandom.RndReal ()));	//	create a new bullet
			Body			*particleBody = particle->GetBody ();														//	get the bullet's body
			particleBody->ApplyForce (offset * force);																//	apply the shot force to the bullet
			gReferee->Install (particle, MOBILE_INTERSECT);														//	put the bullet into the referee
			SArrayPtr	explosion = gSpriteMgr.Fetch (explo[index & 1]);
			gView->Install (SeqPtr (new StatSeq (body->TLocation () + (offset * radius), new TimeSeq (explosion))));
		}
		if (deadTime <= ZERO)
		{
			//	jenny should also report a score here, but Jeremy has modifed it, so he should handle that
			//	see the normal ship class for an example
			Real			radius = ((Circle *) geometry)->Radius ();
			Point_2d	loc = body->TLocation ();
			SArrayPtr explosion[2];
			explosion[0] = gSpriteMgr.Fetch ("explo1");
			explosion[1] = gSpriteMgr.Fetch ("explo3");
			for (int i = 0; i < 3; i++)
			{
				int				index = gRandom.RndInteger (),
									rnd = gRandom.RndInteger () & 1;
				Vector_2d	offset = gDirection[index][0];
				gView->Install (SeqPtr (new StatSeq (loc + (offset * radius * R(0.5)), new TimeSeq (explosion[rnd]))));
			}
			int	count = int (PI * radius * radius * R(1.5e-2));
			for (i = 0; i < count; i += 2)
			{
				int				index = gRandom.RndInteger ();
				Real			force = R(3.0e5) * (gRandom.RndReal () + R(0.25));
				Vector_2d	offset = gDirection[index][0],
									rVel = (offset * (force * R(2.0e-3))) + body->TVelocity ();
				Bullet		*particle = new Bullet (R(1.25), loc + (offset * radius), ATAN2 (rVel[Y], rVel[X]), gSpriteMgr.Fetch ("shrapnel"), R(0.25), this, gSpriteMgr.Fetch ("rsplut8"));	//	create a new bullet
				Body			*particleBody = particle->GetBody ();																	//	get the bullet's body
				particleBody->ApplyForce (offset * force);																			//	apply the shot force to the bullet
				particleBody->AddVelocity (body->TVelocity (), ZERO);										//	add the velocity of the owner to the bullet
				gReferee->Install (particle, MOBILE_INTERSECT);														//	put the bullet into the referee
			}
		}
	}
	return (deadTime > ZERO) ? TRUE : FALSE;
}																																								//	end

//------------------------------------------------------------------------------
int		Jenny::PostCollide (const PiecePtr &piece, Sect *sect)										//	do some work after a collision with another piece
{																																								//	begin
	if ((life == ZERO) && (deadTime == INFINITY))
	{
		Real			radius = ((Circle *) geometry)->Radius ();
		lastPop = deadTime = R(0.01) * radius * (R(0.5) + gRandom.RndReal ());
		Point_2d	loc = body->TLocation ();
		SArrayPtr explosion[2];
		explosion[0] = gSpriteMgr.Fetch ("explo1");
		explosion[1] = gSpriteMgr.Fetch ("explo3");
		for (int i = 0; i < 2; i++)
		{
			int				index = gRandom.RndInteger (),
								rnd = gRandom.RndInteger () & 1;
			Vector_2d	offset = gDirection[index][0];
			gView->Install (SeqPtr (new StatSeq (loc + (offset * radius * R(0.5)), new TimeSeq (explosion[rnd]))));
		}
		int	count = int (PI * radius * radius * R(1.5e-2));
		for (i = 0; i < count; i += 2)
		{
			int				index = gRandom.RndInteger ();
			Real			force = R(3.0e5) * (gRandom.RndReal () + R(0.25));
			Vector_2d	offset = gDirection[index][0],
								rVel = (offset * (force * R(2.0e-3))) + body->TVelocity ();
			Bullet		*particle = new Bullet (R(1.25), loc + (offset * radius), ATAN2 (rVel[Y], rVel[X]), gSpriteMgr.Fetch ("shrapnel"), R(0.25), this, gSpriteMgr.Fetch ("rsplut8"));	//	create a new bullet
			Body			*particleBody = particle->GetBody ();																	//	get the bullet's body
			particleBody->ApplyForce (offset * force);																			//	apply the shot force to the bullet
			gReferee->Install (particle, MOBILE_INTERSECT);														//	put the bullet into the referee
		}
	}
	return PIECE_ALIVE | PIECE_UPDATE;																						//	return flags to indicate object is alive
}																																								//	end

//------------------------------------------------------------------------------
void	Jenny::Damage (const PiecePtr &piece, Real damage)												//	damage the piece
{																																								//	begin
	Piece::Damage (piece, damage);
}																																								//	end

//------------------------------------------------------------------------------
Real	Jenny::TraceRay (CPoint origin, CVector direction)												//	return the time at which a ray intersects this piece
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the ship
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
