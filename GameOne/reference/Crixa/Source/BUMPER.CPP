//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	and:						Bretton Wade
//	Last Revision:	08/16/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "bumper.h"
#include "polygon.h"
#include "rotating.h"
#include "get parameters.h"

//------------------------------------------------------------------------------
Bumper::Bumper (Player *own, CPoint loc, Real width, Real height,								//	constructor
						SArrayPtr spr) :																										//	constructor (continued)
Piece (own)																																			//	constructor (continued)
{																																								//	begin
	body = new Body (loc, ZERO, INFINITY, INFINITY);															//	allocate the body
	visual = new Rotating (body, spr);																						//	allocate the visual
	geometry = Poly::Rectangle (width, height);																		//	allocate the geometry, a rectangle of specified size
	armor = ZERO;																																	//	make the piece indestructable
	hardness = ParmReal (BUMPER_HARDNESS);																				//	set the bumper hardness
	bumped = damaged = FALSE;
}																																								//	end

//------------------------------------------------------------------------------
Bumper::~Bumper (void)																													//	destructor
{																																								//	begin
	delete geometry;																															//	release the geometry
	delete visual;																																//	release the visual
	delete body;																																	//	release the body
}																																								//	end

//------------------------------------------------------------------------------
Real	Bumper::TraceRay (CPoint or, CVector direction)														//	return the time at which a ray intersects this piece
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
int	Bumper::PostCollide (const PiecePtr &piece, Sect *sect)											//	do some mleh.
{																																								//	begin
	bumped = TRUE;
	bumpPiece = piece;
	return PIECE_ALIVE | PIECE_UPDATE;																						//	always alive
}																																								//	end

//------------------------------------------------------------------------------
void	Bumper::Damage (const PiecePtr &piece, Real damage)												//	damage the piece
{																																								//	begin
	damaged = TRUE;
	Piece::Damage (piece, damage);
}																																								//	end

//------------------------------------------------------------------------------
Bool Bumper::Bumped (void)
{
	if (damaged)
		bumped = FALSE;
	damaged = FALSE;
	return bumped;
}

//------------------------------------------------------------------------------
void Bumper::UnBump (void)
{
	bumped = FALSE;
}
//------------------------------------------------------------------------------
