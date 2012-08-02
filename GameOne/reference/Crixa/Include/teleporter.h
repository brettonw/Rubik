//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	08/09/96
//
//------------------------------------------------------------------------------

#ifndef TELEPORTER
#define TELEPORTER

//------------------------------------------------------------------------------
#include "player.h"
#include "device.h"

//------------------------------------------------------------------------------
class Teleporter : public Player, public Device																	//	a teleporter
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
		Real				interval;
		Real				countdown;
		Point_2d		teleloc;
		Bool				Teleport (const PiecePtr &piece);
	public:																																				//	members available externally
			Teleporter (int id, CPoint loc);
virtual	int			Trigger (const PiecePtr &piece, int code = 0, int code2 = 0);		//	trigger the teleporter
virtual Bool		Play (void);																										//	play
};																																							//	end

//------------------------------------------------------------------------------

#endif // TELEPORTER