//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	03/07/97	JB		being rewritten.
//
//------------------------------------------------------------------------------

#ifndef AMMO_UPGRADER
#define AMMO_UPGRADER

//------------------------------------------------------------------------------
#include "player.h"
#include "device.h"
#include "list.h"
#include "emitter device.h"

//------------------------------------------------------------------------------
class	AmmoUpgrader : public Player, public Device
{
	private:
	protected:
				int						type;
				Real					flowRate;
				Real					currentTime;
				List					recentPieces;
				EmitterDevice	soundEmitter;
				Real					bingCountdown;
	public:
				AmmoUpgrader (int type, int trigger, Point_2d loc, Real flowRate);
virtual	~AmmoUpgrader (void);
virtual	int						Trigger (const PiecePtr &piece, int code, int code2);
virtual	Bool					Play (void);
};

//------------------------------------------------------------------------------

#endif // AMMO_UPGRADER