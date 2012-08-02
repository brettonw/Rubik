//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/21/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "mine player.h"
#include "referee.h"
#include "floor trigger.h"

//------------------------------------------------------------------------------
MinePlay::MinePlay (const PiecePtr &min, Real del, Real vel) : Player ("Mine")	//	constructor
{																																								//	begin
	mine = min;
	delay = del;
	elapsed = delay;
	velocity = vel * vel;
}																																								//	end

//------------------------------------------------------------------------------
MinePlay::~MinePlay (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	MinePlay::Play (void)																											//	play one frame
{																																								//	begin
	if (gReferee->Confirm (mine))
	{
		gFloor->Trigger (mine);																											//	check the floor for triggers
		Body	*body = mine->GetBody ();
		if (((Mine *) mine ())->Damaged ())
			((Mine *) mine ())->Explode ();
		else if (((Mine *) mine ())->target ())
		{
			LockPtr	target = ((Mine *) mine ())->target;
			elapsed -= gReferee->Interval ();
			if (elapsed < ZERO)
				((Mine *) mine ())->Explode ();
		}
		else
		{
			CVector	vel = body->TVelocity ();
			if ((vel | vel) > velocity)
			{
				Body	*body = mine->GetBody ();
				body->ApplyForce (R(-0.5) * body->TVelocity () * body->TInertia ());
				gReferee->Update (mine);
			}
		}
		return TRUE;
	}
	return FALSE;
}																																								//	end

//------------------------------------------------------------------------------
