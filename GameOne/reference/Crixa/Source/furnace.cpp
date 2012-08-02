//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97: creation.  (JB)
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "furnace.h"
#include "rotating.h"
#include "circle.h"
#include "get parameters.h"
#include "quadratic.h"

//------------------------------------------------------------------------------
Furnace::Furnace (Player *own, Real rad, CPoint tLoc, Real rLoc, SArrayPtr spr) ://	constructor
Piece (own)																																			//	constructor (continued)
{																																								//	begin
	body = new Body (tLoc, rLoc, INFINITY, INFINITY);															//	create a body for the furnace
	visual = new Rotating (body, spr);																						//	create a visual representation for the furnace
	geometry = new Circle (rad);																									//	create a geometry for the furnace
	weapon = 0;																																		//	do not give the furnace a weapon
	hardness = ParmReal (TURRET_HARDNESS);																				//	we'll assume it's as hard as a turret
}																																								//	end

//------------------------------------------------------------------------------
Furnace::~Furnace (void)																												//	destructor
{																																								//	begin
	delete geometry;																															//	delete the geometry
	delete visual;																																//	delete the visual
	delete body;																																	//	delete the body
}																																								//	end

//------------------------------------------------------------------------------
Bool	Furnace::Step (Real time)																									//	step the furnace
{																																								//	begin
	if (weapon)																																		//	if the furnace has been outfitted with a weapon
		weapon->Reload (time);																											//	reload the weapon
	body->Step (time);	//	NOTE: not necessarily necessary
	return TRUE;																																	//	'course it's still alive -- it's INVINCIBLE
}																																								//	end

//------------------------------------------------------------------------------
Real	Furnace::TraceRay (CPoint origin, CVector direction)											//	return the time at which a ray intersects this piece
{																																								//	begin
	Real	radius = ((Circle *) geometry)->Radius (),															//	get the radius of the ship
				root1, root2;																														//	place to fetch the roots
	if (Dist (origin - body->TLocation (), direction, radius, root1, root2))			//	if the ray intersects the ship
		if (root1 > ZERO)																														//	if the ray intersects the ship at some later time
			return root1;																															//	return that time
	return INFINITY;																															//	return no intersection for all unhandled cases
}																																								//	end

//------------------------------------------------------------------------------
