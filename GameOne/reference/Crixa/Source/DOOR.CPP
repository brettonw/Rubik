//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/07/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "door.h"
#include "polygon.h"
#include "rotating.h"
#include "constrained body.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
Door::Door (Player *own, CPoint loc, Real width, Real height, SArrayPtr spr,		//	constructor
						CVector axis) :																											//	constructor (continued)
Piece (own)																																			//	constructor (continued)
{																																								//	begin
	Real	mass = width * height * R(1.0e3);																				//	compute the mass of the door
	ConBody	*cbody = new ConBody (loc, ZERO, mass, INFINITY);											//	allocate the body
	cbody->SetConstraint (axis);																									//	set the body constraint axis
	body = cbody;																																	//	copy the constrained body pointer to the body
	visual = new Rotating (body, spr);																						//	allocate the visual
	geometry = Poly::Rectangle (width, height);																		//	allocate the geometry, a rectangle of specified size
	armor = ZERO;																																	//	make the door indestructable
	hardness = ParmReal (DOOR_HARDNESS);																					//	set the door hardness
}																																								//	end

//------------------------------------------------------------------------------
Door::~Door (void)																															//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Real	Door::TraceRay (CPoint or, CVector direction)															//	return the time at which a ray intersects this piece
{																																								//	begin
	Point_2d	origin = or - body->TLocation ();																		//	pre-subtract the body position from the origin
	Poly			*poly = (Poly *) geometry;																					//	get a pointer to the polygon geometry
	Real			time = INFINITY;																										//	the intersection time
	for (int i = 0; i < poly->Count (); i++)																			//	loop over all of the vertices and edges
	{																																							//	begin
		CVector		edge = poly->Edge (i);																						//	get the current edge
		Vector_2d	N (edge[Y], -edge[X]);																						//	compute its normal vector
		Real			vel = direction | N;																							//	compute the closin velocity along the normal
		if (vel < ZERO)																															//	if the circle is actually converging on this edge
		{																																						//	begin
			Real	itime = ((poly->Vertex (i) - origin) | N) / vel;										//	compute the actual intersection time from the circle's point of view
			if ((itime >= ZERO) && (itime < time))																		//	if the intersection time is positive, and it is less than our previously computed values
			{																																					//	begin
				Point_2d	ipt = origin + (direction * itime);														//	compute the intersection point of the circle's center point and the convolved edge
				Real			etime = (ipt - poly->Vertex (i)) | edge;											//	compute the intersection time from the edge's point of view
				if ((etime >= ZERO) && (etime <= poly->Length (i)))											//	if the intersection occurs in the segment defined by the edge
					time = itime;																													//	save the intersection time
			}																																					//	end
		}																																						//	end
	}																																							//	end
	return time;																																	//	return the found intersection time
}																																								//	end

//------------------------------------------------------------------------------
