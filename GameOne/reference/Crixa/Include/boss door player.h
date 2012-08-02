//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	03/12/97	JB		creation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef BOSS_DOOR_PLAYER
#define BOSS_DOOR_PLAYER

//------------------------------------------------------------------------------
#include "door player.h"

//------------------------------------------------------------------------------
class BossDoorPlayer : public DoorPlayer
{
	private:
	protected:
				int				emID;
	public:
				BossDoorPlayer (CPoint tLoc, int type, int trigger);
virtual	~BossDoorPlayer (void);
virtual	Bool			Play (void);
				void			DoorStart (int which);
				void			DoorStop (int which);
};

//------------------------------------------------------------------------------

#endif // BOSS_DOOR_PLAYER
