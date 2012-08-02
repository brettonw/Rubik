//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Modified by:		Bretton Wade
//	Last Revision:	08/16/96
//
//------------------------------------------------------------------------------

#ifndef	POWER_MAT
#define POWER_MAT

//------------------------------------------------------------------------------
#include "player.h"
#include "power device.h"

//------------------------------------------------------------------------------
class PowerMat : public Player, public PowerDevice															//	a mat that switches flow when an object is on it
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Bool			defState;																											//	the default state of the route
				Bool			onPad;																												//	whether or not someone is on the pad
	public:																																				//	members available externally
				PowerMat (Bool def, SeqPtr seq, int id);																//	constructor
virtual	int				Trigger (const PiecePtr &piece, int type, int what);					//	send a triggered event down the line
virtual Bool			Play (void);																									//	play
};																																							//	end

//------------------------------------------------------------------------------

#endif // POWER_MAT