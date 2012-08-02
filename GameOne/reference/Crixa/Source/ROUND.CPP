//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/09/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "round.h"
#include "rotating.h"
#include "circle.h"
#include "pi.h"
#include "quadratic.h"

//------------------------------------------------------------------------------
Round::Round (Real rad, CPoint tLoc, Real rLoc, SArrayPtr sprites) : Piece (0)	//	constructor
{																																								//	begin
	Real	mass = PI * rad * rad;																									//	compute the mass as the area of the plate
	body = new Body (tLoc, rLoc, mass, R(0.5) * mass * rad * rad);								//	allocate the body, rInertia is of a solid sphere
	visual = new Rotating (body, sprites);																				//	allocate the visual
	geometry = new Circle (rad);																									//	allocate the geometry, a circle of specified radius
	armor = ZERO;																																	//	make this thing unbreakable
}																																								//	end

//------------------------------------------------------------------------------
Round::~Round (void)																														//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Real	Round::TraceRay (CPoint origin, CVector direction)												//	return the time at which a ray intersects this piece
{																																								//	begin
	Real			radius = ((Circle *) geometry)->Radius (),													//	get the radius of the ship
						root1, root2;																												//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
