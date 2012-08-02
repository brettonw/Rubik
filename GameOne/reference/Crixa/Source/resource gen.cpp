//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/28/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "resource gen.h"
#include "resource.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "view.h"

//------------------------------------------------------------------------------
ResourceGen::ResourceGen (int typ, int id, Point_2d pos, Real force, Real del)
: Player ("Resource Generator"), Device (id)
{
	type = typ;
	ejection = Vector_2d(pos) + Vector_2d (ZERO, R(-82.0));
	orient = ZERO;
	delay = del;
 	push = Vector_2d (ZERO, -force);
	time = ZERO;
}

//------------------------------------------------------------------------------
ResourceGen::~ResourceGen (void)
{
}

//------------------------------------------------------------------------------
int	ResourceGen::Trigger (const PiecePtr &piece, int code, int code2)						//	trigger the resource generator
{
	Device::Trigger (piece, code, code2);
	if (code == DEV_TRIGGER)
		Eject ();
	return 0;
}

//------------------------------------------------------------------------------
Bool	ResourceGen::Play (void)
{
	if (power)
		if (time > ZERO)
		{
			time -= gReferee->Interval ();
			if (time < ZERO)
				time = ZERO;
		}
	return TRUE;
}

//------------------------------------------------------------------------------
#include "stationary sequence.h"
#include "time sequence.h"

void ResourceGen::Eject (void)
{
	if (time == ZERO && power)
	{
		Resource *resource = new Resource  (R(4.0), ejection, orient, gSpriteMgr.Fetch ("res"), type, 20);
		Body *body = resource->GetBody ();
		body->AddVelocity (push, ZERO);
		gReferee->Install (PiecePtr (resource));
		gView->Install (SeqPtr (new StatSeq (ejection, new TimeSeq (gSpriteMgr.Fetch ("etel")))));
		time = delay;
	}
}

//------------------------------------------------------------------------------
