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
#include "pumping sequence.h"
#include "rotating.h"
#include "referee.h"
#include "moving sequence.h"
#include "time sequence.h"
#include "view.h"
#include "random.h"

//------------------------------------------------------------------------------
PumpSeq::PumpSeq (const SArrayPtr &spr, const PiecePtr &obj, CVector off)				//	constructor
{																																								//	begin
	sprite = spr;
	object = obj;
	loc = off;
	delay = sprite->Count () >> 2;
	elapsed = 0;
}																																								//	end

//------------------------------------------------------------------------------
PumpSeq::~PumpSeq (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	PumpSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	if (gReferee->Confirm (object))
	{
		if (--elapsed < 0)
		{
			elapsed = delay;
			Rotating	*rot = (Rotating *) object->GetVisual ();													//	get the rotating visual associated with the object
			CVector		norm = rot->GetDirection ()[0],																		//	get the vector for the owner facing direction
								perp = rot->GetDirection ()[1];																		//	get the perpendicular vector to the norm vector
			Body			*body = object->GetBody ();																				//	get the object's body
			Point_2d	track = (perp * loc[X]) + (norm * loc[Y]) + body->TLocation ();		//	compute the location of the tracking point
			Vector_2d	jiggle (gRandom.RndReal () - R(0.5), gRandom.RndReal () - R(0.5));
			SeqPtr		seq = new MoveSeq (	new TimeSeq (sprite), track + (jiggle * R(4.0)), FALSE, 3, 
																		Vector_2d (ZERO, ZERO), gReferee->Interval (),
																		body->TVelocity (), gReferee->Interval () * sprite->Count () * R(0.75), 
																		Vector_2d (ZERO, ZERO));
			gView->Install (seq, (gRandom.RndInteger () & 0x01) ? PIECE_LAYER : UNDER_LAYER);
		}
		return TRUE;
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
