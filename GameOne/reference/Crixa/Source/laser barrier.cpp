//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/13/96
//
//	Modified by:    Mark E. Kern
//									02/13/97			Quickie bug fix for player respawning too close to a barrier
//																Overides the regular piece Contains fucntion to always return
//																FALSE.
//																Also added sound of object hitting barrier
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "laser barrier.h"
#include "piece ptr.h"
#include "polygon.h"
#include "invisible.h"
#include "sequence ptr.h"
#include "particle sequence.h"
#include "random.h"
#include "view.h"
#include "Sound Mgr.h"

#define HITBARRIER 52

//------------------------------------------------------------------------------
LaserBarrier::LaserBarrier (Player *own, CPoint loc, Real width, Real height, Real dam)
: Piece (own)
{
	damage = dam;
	body = new Body (loc, ZERO, INFINITY, INFINITY);
	visual = new Invisible (body);
	geometry = Poly::Rectangle (width, height);
	armor = ZERO;
}

//------------------------------------------------------------------------------
int		LaserBarrier::PostCollide (const PiecePtr &piece, Sect *sect)
{
/*
	Vector_2d negimpact = -piece->GetBody ()->TVelocity ();
	Point_2d impact = piece->GetBody ()->TLocation ();
	for (int i = 0; i < 16; i++)
	{
		Vector_2d	dir = negimpact + ((gRandom.RndReal () - R(0.5)));
 		dir *= (gRandom.RndReal () + R(0.25)) * R(-2.0e2);
		gView->Install (new PartSeq (impact, dir, Pixel (30, 0, 0), R(0.5) + gRandom.RndReal (), 4));
	}
/*
/*
	Body *body = piece->GetBody ();
	for (int i = 0; i < 4; i++)
	{
		Vector_2d	dir = gDirection[gRandom.RndInteger ()][0] * i * i;
		gView->Install (new PartSeq (body->TLocation (), body->TVelocity () + dir, Pixel (31, 8, 0), R(1.0) + gRandom.RndReal (), 3));
	}*/

	gSoundMgr->PlaySound(HITBARRIER, piece, FALSE);																// Zap sound of piece hitting barrier
	piece->Damage (PiecePtr (this), damage);
	return PIECE_ALIVE | PIECE_UPDATE;
}

//------------------------------------------------------------------------------
Real	LaserBarrier::TraceRay (CPoint or, CVector direction)											//	return the time at which a ray intersects this piece
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


/*******************************************\
(MK) 02/13/97 Quickie fix for when the program checks to see
if anything is contained in a laser barrier piece.
This is because the regular contains function located
in class piece uses a bounds function that only
returns a radius, not a rectangular area.
\*******************************************/
Bool	LaserBarrier::Contains(CPoint location)
{	
	return FALSE;
	// Always say nothing is contained by this barrier. 
	// This doesn't affect anything except those calls
	// to Contains from gReferree that check all pieces in
	// the world.

}