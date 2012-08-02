//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Modified by:		Bretton Wade
//	Last Revision:	08/16/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "power mat.h"
#include "view.h"

//------------------------------------------------------------------------------
PowerMat::PowerMat (Bool def, SeqPtr seq, int id) : Player ("Mat"), PowerDevice (seq, id)				//	a power conducting pipe
{																																								//	begin
	route = defState = def;																												//	the mat's natural current flow state
	onPad = FALSE;
}																																								//	end

//------------------------------------------------------------------------------
int	PowerMat::Trigger (const PiecePtr &piece, int type, int what)								//	trigger the device
{																																								//	begin
	Device::Trigger (piece, type, what);
	if (type == DEV_TRIGGER)
		onPad = TRUE;
	return 0;																																			//	return nothing
}																																								//	end

//------------------------------------------------------------------------------
Bool	PowerMat::Play (void)
{
	if (onPad)
	{
		if (defState == route)
			Toggle ();
	}
	else
	{
		if (defState != route)
			Toggle ();
	}
	onPad = FALSE;
	return TRUE;
}

//------------------------------------------------------------------------------
