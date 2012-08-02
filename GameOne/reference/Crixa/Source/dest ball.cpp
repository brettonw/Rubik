//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97:	creation. (JB)
//									02/05/97:	some finishing touches. (JB)
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "dest ball.h"
#include "random.h"
#include "pi.h"
#include "rotating.h"
#include "circle.h"
#include "quadratic.h"
#include "floor trigger.h"

//------------------------------------------------------------------------------
DestBall::DestBall (Player *own, Real armor, CPoint loc, Real rad,							//	constructor
										SArrayPtr spr) : Piece (own)																//	constructor (continued)
{																																								//	begin
	rad *= R(0.95);																																//	shrink the size of the rocks because of some collision problems
	Real	mass = TWO_PI * rad * rad,																							//	determine the mass for the object
				rInertia = R(0.5) * mass;																								//	determine the rotational inertial.  WAS: * rad * rad
	body = new Body (loc, gRandom.RndReal () * TWO_PI, mass, rInertia);						//	create a body for the ball
	visual = new Rotating (body, spr);																						//	create a sprite for the ball
	geometry = new Circle (rad);																									//	create a circle for the ball
	SetArmor (armor);																															//	set the armor level for the ball
}																																								//	end

//------------------------------------------------------------------------------
DestBall::~DestBall (void)																											//	destructor
{																																								//	begin
	delete body;																																	//	delete!
	delete visual;																																//	delete!!
	delete geometry;																															//	delete!!!
}																																								//	end

//------------------------------------------------------------------------------
Bool	DestBall::Step (Real time)																								//	step the dest ball through the interval
{																																								//	begin
	gFloor->Trigger (this);																												//	activate any floor triggers
	return Piece::Step (time);																										//	step the Piece through the interval
}																																								//	end

//------------------------------------------------------------------------------
Real	DestBall::TraceRay (CPoint origin, CVector direction)											//	traceray routine
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the ship
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
