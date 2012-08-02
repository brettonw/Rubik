//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/29/97
//
//	02/07/97	MK		Adding JB's turret explosion breakout
//  02/09/97	MK		Added PiecePtr info to PlaySound call
//	02/24/97	DS		Added Flashing Indicator in Damage()
//	03/06/97	DS		Increased computed turret rInertia in constructor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "turret.h"
#include "rotating.h"
#include "circle.h"
#include "pi.h"
#include "direction.h"
#include "quadratic.h"
#include "math utility.h"
#include "get parameters.h"

//	hacks for explosions
#include "view.h"
#include "time sequence.h"
#include "particle sequence.h"
#include "stationary sequence.h"
#include "particle.h"
#include "bullet.h"
#include "direction.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "random.h"
#include "pi.h"
#include "circle.h"
#include "bullet.h"
#include "turret thug.h"
#include "sound mgr.h"

//------------------------------------------------------------------------------
Turret::Turret (Player *own, Real rad, CPoint loc, Real rloc, SArrayPtr spr) :	//	constructor
Piece (own)																																			//	constructor (continued)
{																																								//	begin
	rDamping = R(-5.0e-1);																												//	set a starting value for rotational damping
	maxRThrust = R(0.5) * PI * rad * rad * rad * rad;															//	compute the rotational inertia of a solid sphere
	body = new Body (loc, rloc, INFINITY, R(2.0) * maxRThrust);										//	allocate the body
	visual = new Rotating (body, spr);																						//	allocate the normal visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	weapon = 0;																																		//	clear the weapon pointer
	hardness = ParmReal (TURRET_HARDNESS);																				//	set the turret hardness
	deadTime = INFINITY;
}																																								//	end

//------------------------------------------------------------------------------
Turret::~Turret (void)																													//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
void	Turret::Explode (void)
{
	lastPop = deadTime;
	static		cstr	explo[2] = {"explo2", "explo1"};
	Real			radius = ((Circle *) geometry)->Radius (),
						force = R(3.0e5) * (gRandom.RndReal () + R(0.25));
	int				index = gRandom.RndInteger ();

	Vector_2d	offset = gDirection[index][0];
	Particle	*particle = new Particle (body->TLocation () + (offset * radius), MakePixel (31, 16 + (index & 15), 0), R(0.125), R(0.5) + (R(0.5) * gRandom.RndReal ()));	//	create a new bullet
	Body			*particleBody = particle->GetBody ();														//	get the bullet's body
	particleBody->ApplyForce (offset * force);																//	apply the shot force to the bullet
	particleBody->AddVelocity (body->TVelocity (), ZERO);											//	add the velocity of the owner to the bullet
	gReferee->Install (particle, MOBILE_INTERSECT);														//	put the bullet into the referee
	SArrayPtr	explosion = gSpriteMgr.Fetch (explo[index & 1]);
	gView->Install (SeqPtr (new StatSeq (body->TLocation () + (offset * radius), new TimeSeq (explosion))));
}

//------------------------------------------------------------------------------
Bool	Turret::Step (Real time)																									//	step the piece ahead in time
{																																								//	begin
	if (weapon)																																		//	if the weapon is a valid weapon
		weapon->Reload (time);																											//	reload it
	body->Step (time);																														//	step the body ahead in time
	body->AddVelocity (Vector_2d (ZERO, ZERO), rDamping * body->RVelocity ());		//	damp the rotational velocity
	//return life > ZERO ? TRUE : FALSE;																						//	return true if the piece is still alive
	if (deadTime < INFINITY)
	{
		deadTime -= time;
		if (lastPop - deadTime > R(0.01))
			Explode();
	}
	return (deadTime > ZERO) ? TRUE : FALSE;
}																																								//	end

//------------------------------------------------------------------------------
int		Turret::PostCollide (const PiecePtr &piece, Sect *sect)										//	do some work after a collision with another piece
{																																								//	begin
	return PIECE_ALIVE | PIECE_UPDATE;																						//	return flags to indicate object is alive
}																																								//	end

//------------------------------------------------------------------------------
void	Turret::Damage (const PiecePtr &piece, Real damage)												//	damage the piece
{																																								//	begin
	Piece::Damage (piece, damage);																								//	do the parental behavior
	GetVisual()->SetMode(VISFLASH);																								//	ds: flash!
	if ((life == ZERO) && (deadTime == INFINITY))
	{
		if (piece ())
		{
			Player	*pOwner = piece->GetOwner ();
			if (pOwner)
				if (pOwner != owner)
					pOwner->AddScore (0, 0, 1, 0, 0, value);
				else
					pOwner->AddScore (0, 0, 0, 0, 1, 0);
		}
		gSoundMgr->PlaySound (9, this);																							// Turret Gets Destroyed Sound
		Real			radius = ((Circle *) geometry)->Radius ();
		lastPop = deadTime = R(0.01) * radius * (R(0.5) + gRandom.RndReal ());
		Point_2d	loc = body->TLocation ();
		extern	SArrayMgr	gSpriteMgr;
		SArrayPtr explosion[2];
		explosion[0] = gSpriteMgr.Fetch ("explo3");
		explosion[1] = gSpriteMgr.Fetch ("explo3");
		for (int i = 0; i < 3; i++)
		{
			int				index = gRandom.RndInteger (),
								rnd = gRandom.RndInteger () & 1;
			Vector_2d	offset = gDirection[index][0];
			gView->Install (SeqPtr (new StatSeq (loc + (offset * radius * R(0.5)), new TimeSeq (explosion[rnd]))));
		}
		int	count = int (PI * radius * radius * R(0.015));
		for (i = 0; i < count; i++)
		{
			int				index = gRandom.RndInteger ();
			Real			force = R(3.0e5) * (gRandom.RndReal () + R(0.25));
			Vector_2d	offset = gDirection[index][0],
								rVel = (offset * (force * R(2.0e-3))) + body->TVelocity ();
			Bullet	*particle = new Bullet (R(1.25), loc + (offset * radius), ATAN2 (rVel[Y], rVel[X]), gSpriteMgr.Fetch ("shrapnel"), R(0.25), this, gSpriteMgr.Fetch ("rsplut8"));	//	create a new bullet
			Body			*particleBody = particle->GetBody ();																	//	get the bullet's body
			particleBody->ApplyForce (offset * force);																			//	apply the shot force to the bullet
			particleBody->AddVelocity (body->TVelocity (), ZERO);											//	add the velocity of the owner to the bullet
			gReferee->Install (particle, MOBILE_INTERSECT);														//	put the bullet into the referee
		}
	}
}																																								//	end

//------------------------------------------------------------------------------
Real	Turret::TraceRay (CPoint origin, CVector direction)												//	return the time at which a ray intersects this piece
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the ship
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
void	Turret::Rotate (Real thrust)																							//	rotate the turret about its center of mass
{																																								//	begin
	body->ApplyTorque (thrust * maxRThrust);																			//	apply the scaled torque to the body
}																																								//	end

//------------------------------------------------------------------------------
Real	Turret::Point (CVector vec)																								//	return the amount of thrust necessary to rotate the turret to a given vector
{																																								//	begin
	Real	angle = ATAN2 (vec[Y], vec[X]),																					//	compute the angle of the goal vector
				delta = angle - body->RLocation ();																			//	compute the difference between that and the current location
	while (delta > PI)																														//	while the angle is out of range
		delta -= TWO_PI;																														//	subtract 2 PI to bring it into range
	while (delta < -PI)																														//	while the angle is out of range
		delta += TWO_PI;																														//	add two pi to bring it into range
	Real	deltaVelocity = (delta / gReferee->Interval ()) - body->RVelocity (),		//	compute the velocity change necessary to bring the angle to the desired one in the next frame
				thrust = (body->RInertia () * deltaVelocity) / maxRThrust;							//	compute the fraction of thrust necessary to achieve the velocity change
	if (FABS (thrust) > R(1.0))																										//	if the fraction is greater than unit value
		thrust = Sign (thrust);																											//	clamp it
	return thrust;																																//	return the thrust fraction
}																																								//	end

//------------------------------------------------------------------------------
Real	Turret::PointAt (CPoint pt)																								//	point the turret at a selected location
{																																								//	begin
	return Point (pt - body->TLocation ());																				//	point at the difference vector betweent the point and my location
}																																								//	end

//------------------------------------------------------------------------------
