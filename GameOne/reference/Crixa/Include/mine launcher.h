//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/12/96
//
//------------------------------------------------------------------------------

#ifndef	MINE_LAUNCHER
#define	MINE_LAUNCHER

//------------------------------------------------------------------------------
#include "gun.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class MineL : public Gun																												//	a mine laying weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	mineSprite;																										//	sprite array for the projectile
	public:																																				//	members available externally
				MineL (Real rate, const PiecePtr &owner, CPoint muzzle,	Real radius, 		//	constructor 
									Real damage, SArrayPtr mine, int shotSound);									//	constructor (continued)
virtual	~MineL (void);																													//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	MINE_LAUNCHER
