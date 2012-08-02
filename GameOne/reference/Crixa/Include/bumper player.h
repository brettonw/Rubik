//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/15/96
//
//------------------------------------------------------------------------------

#ifndef	BUMPER_PLAYER
#define	BUMPER_PLAYER

//------------------------------------------------------------------------------
#include "player.h"
#include "power device.h"
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class BumpPlayer : public Player, public PowerDevice
{
	private:
	protected:
				PiecePtr	bumper;
				Real			bounce;
	public:
				BumpPlayer (Bool flow, CPoint loc, Real width, Real height, SArrayPtr spr, int id);
virtual	~BumpPlayer (void);
virtual Bool			Play (void);
virtual int				Trigger (const PiecePtr &piece, int type, int what);					//	trigger the device
virtual	void			Toggle (void);																								//	toggle the route state
};

//------------------------------------------------------------------------------
#endif	//	BUMPER_PLAYER