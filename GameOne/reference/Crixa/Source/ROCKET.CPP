//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Modified by:		Jeremy Biddle
//	Last Revision:	01/20/96
//  Modified by:    Mark E Kern
//									02/12/96					Added Rocket Emitter Sounds
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "rocket.h"
#include "rotating.h"
#include "circle.h"
#include "pi.h"
#include "direction.h"
#include "quadratic.h"
#include "math utility.h"
#include "referee.h"
#include "view.h"
#include "time sequence.h"
#include "stationary sequence.h"
#include "moving sequence.h"
#include "particle sequence.h"
#include "sprite array mgr.h"
#include "random.h"
#include "direction.h"
#include "Sound Mgr.h"

#define  ROCKETEM 47																														// Sound of Rocket Emitter

//------------------------------------------------------------------------------
Rocket::Rocket (Player *own, Real rad, CPoint loc, Real rloc,										//	constructor
						SArrayPtr spr, SArrayPtr thr, Real dam) : Piece (own)								//	constructor (continued)
{																																								//	begin
	maxTThrust = PI * rad * rad * R(1.0e2);																				//	compute the mass as the area of the plate times a thickness
	maxRThrust = R(0.5) * maxTThrust * rad * rad;																	//	compute the rotational inertia of a solid sphere
	body = new Body (loc, rloc, maxTThrust, maxRThrust);													//	allocate the body
	visual = new Rotating (body, spr);																						//	allocate the visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	thrustSprite = thr;
	damage = dam;
	// Sound Emittter (MK)
	rockEMid = gSoundMgr->InsertEMSound(ROCKETEM,this);														// (MK) insert rocket thrust emiiter
}																																								//	end

//------------------------------------------------------------------------------
Rocket::~Rocket (void)																													//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
int		Rocket::PreCollide (const PiecePtr &piece, Sect *sect)										//	do some work before a collision with another piece
{																																								//	begin
	if (owner != piece->GetOwner ())
		for (int i = 0; i < 12; i++)
		{
			Vector_2d	dir = gDirection[gRandom.RndInteger ()][0] * ((gRandom.RndReal () + R(0.25)) * R(2.0e2));
			gView->Install (new PartSeq (sect->point, dir + body->TVelocity (), MakePixel (31, 0, 0), R(0.5) + gRandom.RndReal (), 4));
		}
	return PIECE_ALIVE | PIECE_UPDATE;																						//	return alive, so we get the impact effect
}																																								//	end

//------------------------------------------------------------------------------
int		Rocket::PostCollide (const PiecePtr &piece, Sect *sect)										//	do some work after a collision with another piece
{																																								//	begin
	gSoundMgr->RemoveEMSound(ROCKETEM, rockEMid);																	//  remove rocket sound emitter
	if (owner == piece->GetOwner ())
		return PIECE_ALIVE | PIECE_UPDATE;
	piece->Damage (this, damage);																									//	damage the piece we just hit
	for (int i = 0; i < 12; i++)
	{
		Vector_2d	dir = gDirection[gRandom.RndInteger ()][0] * ((gRandom.RndReal () + R(0.25)) * R(2.0e2));
		gView->Install (new PartSeq (sect->point, dir + body->TVelocity (), MakePixel (31, 0, 0), R(0.5) + gRandom.RndReal (), 4));
	}

	SArrayPtr splut = gSpriteMgr.Fetch("rsplut8");
	Point_2d	hitPt = sect->point + (Vector_2d (gRandom.RndReal () - R(0.5), gRandom.RndReal () - R(0.5)) * R(1.0e1));
	Vector_2d	victVel;
	if (piece->GetBody ())
		victVel = piece->GetBody ()->TVelocity ();
	else
		victVel (ZERO, ZERO);
	gView->Install (new MoveSeq (new TimeSeq (splut), hitPt, FALSE, 3, Vector_2d (ZERO, ZERO), gReferee->Interval (), victVel, splut->Count () * gReferee->Interval (), Vector_2d (ZERO, ZERO)));

	return PIECE_DEAD;																														//	the bullet goes away now, so return dead
}																																								//	end

//------------------------------------------------------------------------------
Real	Rocket::TraceRay (CPoint origin, CVector direction)												//	return the time at which a ray intersects this piece
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the ship
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
void	Rocket::Thrust (Real thrust)																							//	thrust the ship in the forward/backward axis
{																																								//	begin
	Rotating	*rot = (Rotating *) visual;																					//	get a pointer to the rotating visual
	Vector_2d	zeroVec (ZERO, ZERO);
	for (int i = 0; i < 3; i++)
	{
		Vector_2d	tVel = body->TVelocity () + ((R(-1.0e2) * gRandom.RndReal ()) * rot->GetDirection ()[0]);
		SeqPtr		seq = new MoveSeq (new TimeSeq (thrustSprite), body->TLocation (), FALSE, 3, 
							zeroVec, gReferee->Interval (), tVel, thrustSprite->Count () * gReferee->Interval () * R(0.7), zeroVec);
		gView->Install (seq, (gRandom.RndInteger () & 1) ? OVER_LAYER : UNDER_LAYER);
	}
	body->ApplyForce (rot->GetDirection ()[0] * thrust * maxTThrust);							//	apply the thrust to the body along the thrust vector
}																																								//	end

//------------------------------------------------------------------------------
void	Rocket::Rotate (Real thrust)																							//	rotate the ship about its center of mass
{																																								//	begin
	body->ApplyTorque (thrust * maxRThrust);																			//	apply the scaled torque to the body
}																																								//	end

//------------------------------------------------------------------------------
Real	Rocket::Point (CVector vec)																								//	return the amount of thrust necessary to rotate the ship to a given vector
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
Real	Rocket::Go (CVector ax)																										//	compute the necessary orientation to move the rocket at a specified vector
{																																								//	begin
	Vector_2d	axis = ax;																													//	copy the goal axis
	if (axis.Normalize () > ZERO)																									//	make it length one, if this is successful
	{																																							//	make it length one
		if ((axis | body->TVelocity ()) > ZERO)																			//	if the velocity along the axis is positive
			return Point (-Reflect (body->TVelocity (), axis));												//	the pointing direction to obtain that motion is the reflection of the current direction about the axis
		return Point (-body->TVelocity () + axis);																	//	otherwise, we have to gradually bring ourselves into line
	}																																							//	end
	return ZERO;																																	//	return zero to indicate no action is necessary
}																																								//	end

//------------------------------------------------------------------------------
