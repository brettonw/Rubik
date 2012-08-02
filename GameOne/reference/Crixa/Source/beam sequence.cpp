//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "beam sequence.h"
#include "random.h"
#include "fade pixel sequence.h"
#include "rotating.h"
#include "display.h"

//------------------------------------------------------------------------------
BeamSeq::BeamSeq (Real dist, Pixel color, Real time, CPoint muz, PiecePtr own)	//	constructor
{																																								//	begin
	muzzle = muz;
	owner = own;
	count = int (dist) >> 1;
	if (count <= 0)
		count = 1;
	dots = new Dot[count];
	for (int i = 0; i < count; i++)
	{
		dots[i].offset = gRandom.RndReal () * gRandom.RndReal () * dist;
		dots[i].seq = new FadeSeq (color, time);
	}
}																																								//	end

//------------------------------------------------------------------------------
BeamSeq::~BeamSeq (void)																												//	default destructor
{																																								//	begin
	delete[] dots;																																//	delete the dot array
}																																								//	end

//------------------------------------------------------------------------------
Bool	BeamSeq::Step (CPoint where)																							//	step to the next event
{																																								//	begin
/*
	Rotating	*rot = (Rotating *) owner->GetVisual ();														//	get the rotating visual associated with the owner
	CVector		norm = rot->GetDirection ()[0],																			//	get the vector for the owner facing direction
						perp = rot->GetDirection ()[1];																			//	get the perpendicular vector to the norm vector
	Vector_2d	muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
*/
	Body			*ownerBody = owner->GetBody ();																			//	get the owner's body
	Vector_2d	norm (ownerBody->RLocation ()),
						perp (norm[Y], -norm[X]),
						muzzleOffset = (perp * muzzle[X]) + (norm * muzzle[Y]);							//	compute the offset of the muzzle point
	Point_2d	origin = ownerBody->TLocation () + muzzleOffset;										//	compute the actual muzzle point
	for (int i = count - 1; i > 0; i--)
		dots[i].seq->Step ((origin + (dots[i].offset * norm)) - where);
	return dots[0].seq->Step ((origin + (dots[0].offset * norm)) - where);
}																																								//	end

//------------------------------------------------------------------------------
