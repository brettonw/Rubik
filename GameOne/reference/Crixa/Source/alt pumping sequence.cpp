//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	03/06/97	JB		creation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "alt pumping sequence.h"
#include "referee.h"
#include "moving sequence.h"
#include "time sequence.h"
#include "view.h"
#include "random.h"
#include "pi.h"

//------------------------------------------------------------------------------
AltPumpSeq::AltPumpSeq (const SArrayPtr &spr, const PiecePtr &obj, CVector off) :
PumpSeq (spr, obj, off)
{
}

//------------------------------------------------------------------------------
AltPumpSeq::~AltPumpSeq (void)
{
	
}

//------------------------------------------------------------------------------
Bool	AltPumpSeq::Step (CPoint where)
{
	if (gReferee->Confirm (object))
	{
		if (--elapsed < 0)
		{
			elapsed = delay;
			Body			*body = object->GetBody ();																			//	get the object's body
			Point_2d	track = body->TLocation () + loc;
			track += Vector_2d (gRandom.RndReal (), gRandom.RndReal ()) * R(5.0);
			SeqPtr		seq = new MoveSeq (	new TimeSeq (sprite), track, FALSE, 3, 
																		Vector_2d (ZERO, ZERO), gReferee->Interval (),
																		body->TVelocity (), gReferee->Interval () * sprite->Count () * R(0.75), 
																		Vector_2d (ZERO, ZERO));
			gView->Install (seq, OVER_LAYER);//(gRandom.RndInteger () & 0x01) ? PIECE_LAYER : UNDER_LAYER);
		}
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------------------------------------------
