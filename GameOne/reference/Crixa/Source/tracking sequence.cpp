//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/20/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "tracking sequence.h"
#include "rotating.h"
#include "referee.h"

//------------------------------------------------------------------------------
TrackSeq::TrackSeq (const SeqPtr &seq, const PiecePtr &obj, CVector off)				//	constructor
{																																								//	begin
	sequence = seq;
	object = obj;
	loc = off;
	loc2 (ZERO, ZERO);
}																																								//	end

//------------------------------------------------------------------------------
TrackSeq::TrackSeq (const SeqPtr &seq, const PiecePtr &obj, CVector off, CVector off2)//	constructor
{																																								//	begin
	sequence = seq;
	object = obj;
	loc = off;
	loc2 = off2;
}																																								//	end

//------------------------------------------------------------------------------
TrackSeq::~TrackSeq (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	TrackSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	if (gReferee->Confirm (object))
	{
		Rotating	*rot = (Rotating *) object->GetVisual ();													//	get the rotating visual associated with the object
		CVector		norm = rot->GetDirection ()[0],																		//	get the vector for the owner facing direction
							perp = rot->GetDirection ()[1];																		//	get the perpendicular vector to the norm vector
		Body			*body = object->GetBody ();																				//	get the object's body
		Point_2d	track = loc2 + (perp * loc[X]) + (norm * loc[Y]) + body->TLocation ();		//	compute the location of the tracking point
		return sequence->Step (track - where);																			//	return the result of stepping the sequence
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
void	TrackSeq::Reset (void)																										//	return the sequence to its starting state
{																																								//	begin
	sequence->Reset ();
}																																								//	end

//------------------------------------------------------------------------------
