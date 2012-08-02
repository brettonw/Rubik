//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	01/13/97
//
//  Modified by:    Mark E. Kern
//  Last Revision:  02/13/97
//									Added Teleport sound calls
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "teleporter.h"
#include "referee.h"
#include "view.h"
#include "sprite array mgr.h"
#include "sequence ptr.h"
#include "stationary sequence.h"
#include "time sequence.h"
#include "Sound Mgr.h"

#define TELEPORT 55

//------------------------------------------------------------------------------
Teleporter::Teleporter (int id, CPoint loc) : Player ("Teleporter"), Device (id)
{
//	interval = ParmString (TELE_INTERVAL);
//	countdown = ZERO;
	teleloc = loc;
}

//------------------------------------------------------------------------------
int	Teleporter::Trigger (const PiecePtr &piece, int code, int code2)						//	trigger the teleporter
{
	Device::Trigger (piece, code, code2);
	if (code == DEV_TRIGGER)
		Teleport (piece);
	
//		if (countdown == ZERO)
//			countdown = interval;
	return 0;
}

//------------------------------------------------------------------------------
Bool	Teleporter::Play (void)
{
/*
	if (countdown > ZERO)
	{
		countdown -= gReferee->Interval ();
		if (countdown <= ZERO)
		{
			countdown = ZERO;
			Teleport (piece);
		}
	}
*/
	return TRUE;
}

//------------------------------------------------------------------------------
Bool	Teleporter::Teleport (const PiecePtr &piece)
{
	if (power)
	{
		PiecePtr destpiece;
		if (!gReferee->Contains (teleloc, destpiece))
		{
			// Play Sound at pre-teleport location
			gSoundMgr->PlaySound(TELEPORT,piece,FALSE);	


			gView->Install (SeqPtr (new StatSeq (piece->GetBody ()->TLocation (), new TimeSeq (gSpriteMgr.Fetch ("etel")))));
			piece->GetBody ()->SetLocation (teleloc);
			gView->Install (SeqPtr (new StatSeq (piece->GetBody ()->TLocation (), new TimeSeq (gSpriteMgr.Fetch ("etel")))));
			gReferee->Update (piece);

			// Play Sound at new teleported location
			gSoundMgr->PlaySound(TELEPORT,piece,FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

//------------------------------------------------------------------------------
