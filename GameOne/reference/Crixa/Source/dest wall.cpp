//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle and	Bretton Wade
//	Last Revision:	01/16/97
//
//  02/17/97	MK		Added Resource Dropping to Destructable Walls
//	02/24/97	DS		Added Flashing Indicator in Damage()
//	03/06/97	DS		Glowing generic resources
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "dest wall.h"
#include "polygon.h"
#include "sequence visual.h"
#include "stationary sequence.h"
#include "sprite sequence.h"
#include "view.h"
#include "random.h"

#include "time sequence.h"
#include "sprite array mgr.h"
#include "get parameters.h"
#include "Sound Mgr.h"
#include "resource.h"
#include "referee.h"
#include "direction.h"


#define DEXPLO  8

//------------------------------------------------------------------------------
DestWall::DestWall (Player *own, Real arm, CPoint loc, Real width, Real height,
										SeqPtr seq, int layr) : Piece (own)
{
	location = loc;
	layer = layr;
	Real mass = width * height * R(1.0e3);
	body = new Body (loc, ZERO, INFINITY, INFINITY);
	visual = new SeqVisual (body, seq);
	geometry = Poly::Rectangle (width, height);
	SetArmor (arm);
	sequence = (SpriteSeq *) seq ();
}

//------------------------------------------------------------------------------
DestWall::~DestWall (void)
{
	delete geometry;
	delete visual;
	delete body;
}

//------------------------------------------------------------------------------
Real	DestWall::TraceRay (CPoint or, CVector direction)													//	return the time at which a ray intersects this piece
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
void	DestWall::Damage (const PiecePtr &piece, Real damage)											//	damage the piece
{																																								//	begin
	Piece::Damage (piece, damage);
	GetVisual()->SetMode(VISFLASH);																								//	ds: flash!
	int count = sequence->Count () - 1;
	int current = (ushort) (count - ceil (life * count));
	if (current != sequence->GetCurrent ())
	{
		gSoundMgr->PlaySound(DEXPLO, this);
		TimeSeq *timeseq = new TimeSeq (gSpriteMgr.Fetch (ParmString (TRAIK_SPLUT)));
		timeseq->SetCurrent (gRandom.RndInteger () % timeseq->Count ());
		gView->Install (SeqPtr (new StatSeq (location + Vector_2d (geometry->Bound () * gRandom.RndReal (),
										geometry->Bound () * gRandom.RndReal ()), SeqPtr (timeseq))));
		sequence->SetCurrent (current);
	}
	if (!(life>ZERO))
	{
	int numSpluts = (gRandom.RndInteger () & 3) + 1;
	for (int i = 0; i < numSpluts; i++)
	{
			TimeSeq *timeseq = new TimeSeq (gSpriteMgr.Fetch ("explo1"));
			timeseq->SetCurrent (gRandom.RndInteger () % timeseq->Count ());
			gView->Install (SeqPtr (new StatSeq (location + Vector_2d (geometry->Bound () * gRandom.RndReal (),
											geometry->Bound () * gRandom.RndReal ()), SeqPtr (timeseq))));
		}
		
		int rescount = gRandom.RndReal () * R(5.0);																	// generate up to five resources

		for (int x = 0; x <= rescount; x++)
			DropResource(RESOURCE_GENERIC, 5);																				// create a generic resource, with a time delay base of five seconds
	}


}																																								//	end

//------------------------------------------------------------------------------


void	DestWall::DropResource (int type, int number)																	//	drop a single resource
{																																													//	begin
	Vector_2d	vector = gDirection[gRandom.RndInteger ()][0];
	Point_2d	location = body->TLocation () + vector;
 	Resource *resource;
	if (type == RESOURCE_GENERIC)
	{
		resource = new Resource  (R(5.0), location, R(0.0), gSpriteMgr.Fetch ("flare"), type, number, R(3.0) + (gRandom.RndReal () * R(2.0)));
		(resource->GetVisual())->SetMode(VISADD);																		//	use additive draw
	}
	else
		resource = new Resource  (R(4.0), location, R(0.0), gSpriteMgr.Fetch ("res"), type, number);
	Body	*bd = resource->GetBody ();
	bd->AddVelocity (body->TVelocity (), ZERO);
	bd->ApplyForce (vector * gRandom.RndReal () * R(8.0e3));
	gReferee->Install (resource, MOBILE_INTERSECT);
}															