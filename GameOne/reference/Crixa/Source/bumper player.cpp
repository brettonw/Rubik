//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/16/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "bumper player.h"
#include "bumper.h"
#include "referee.h"
#include "sprite array mgr.h"
#include "sound mgr.h"
#include "state mgr.h"

//------------------------------------------------------------------------------
BumpPlayer::BumpPlayer (Bool flow, CPoint loc, Real width, Real height, SArrayPtr spr, int id)
: Player ("Bumper"), PowerDevice (SeqPtr (), id)
{
	route = flow;
	bounce = ZERO;
	bumper = new Bumper (this, loc, width, height, spr);
	gReferee->Install (bumper, FULL_INTERSECT);
}

//------------------------------------------------------------------------------
BumpPlayer::~BumpPlayer (void)
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	BumpPlayer::Play (void)
{
	bounce -= gReferee->Interval ();
	if (((Bumper *) bumper ())->Bumped ())
	{
		((Bumper *) bumper ())->UnBump ();
		PiecePtr	bumpedBy = ((Bumper *) bumper ())->BumpedBy ();
		Toggle ();
		Trigger (bumpedBy, DEV_TRIGGER, 0);

		// for State Manager
		if (route)
			gStateMgr->Trigger(identifier,bumpedBy,DEV_TRIGGER,1);										// power is flowin'
		else
			gStateMgr->Trigger(identifier,bumpedBy,DEV_TRIGGER,0);										// power ain't flowin'
	}
	return TRUE;
}

//------------------------------------------------------------------------------
void	BumpPlayer::Toggle (void)																									//	toggle the route state
{																																								//	begin
	if (power)
	{
		if (bounce <= ZERO)
		{
			Device::Toggle ();
			gSoundMgr->PlaySound (24);
			bounce = R(1.0);
		}
	}
}																																								//	end

//------------------------------------------------------------------------------

int	BumpPlayer::Trigger (const PiecePtr &piece, int type, int what)							//	State Manager Hook for trigger the device
{																																								//	begin
	if (type==DEV_TRIGGER)																												//	if DEV_TRIGGER
		Toggle();																																		//	toggle this bumperstate
	#if 0
		DEBUG_STR("BumpPlayer%d: Received TriggerType %d\n",identifier,type);
		DEBUG_STR("    device state ON=%d, ROUTE=%d, POWER=%d\n",on,route,power);
	#endif
	return Device::Trigger(piece, type, what);																		//	allow the device to get the trigger
}																																								//	end