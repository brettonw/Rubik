//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/22/97
//
//------------------------------------------------------------------------------

#ifndef	MINE_PLAYER
#define	MINE_PLAYER

//------------------------------------------------------------------------------
#include "player.h"
#include "mine.h"

//------------------------------------------------------------------------------
class MinePlay : public Player																									//	a player for a mine
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				PiecePtr	mine;																													//	pointer to the mine piece
				Real			delay;																												//	delay time before exploding the mine
				Real			elapsed;																											//	the amount of time since triggering
				Real			velocity;																											//	maximum velocity of floating mine
	public:																																				//	members available externally
				MinePlay (const PiecePtr &mine, Real del, Real vel);										//	constructor
virtual	~MinePlay (void);																												//	destructor
virtual	Bool			Play (void);																									//	play one frame
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	MINE_PLAYER