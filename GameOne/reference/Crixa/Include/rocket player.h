//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/16/96
//
//------------------------------------------------------------------------------

#ifndef	ROCKET_PLAYER
#define	ROCKET_PLAYER

//------------------------------------------------------------------------------
#include "player.h"
#include "piece ptr.h"
#include "lock ptr.h"

//------------------------------------------------------------------------------
class RockPlay : public Player																									//	a player for a mine
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	rocket;																												//	pointer to the rocket piece
				LockPtr		target;																												//	the target for the rocket
				Real			delay;																												//	the delay time before manuevering
	public:																																				//	members available externally
				RockPlay (const PiecePtr &rocket, const LockPtr &lock, Real delay);			//	constructor
virtual	~RockPlay (void);																												//	destructor
virtual	Bool			Play (void);																									//	play one frame
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	ROCKET_PLAYER