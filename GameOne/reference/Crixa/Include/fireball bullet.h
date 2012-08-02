//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	12/23/96
//
//------------------------------------------------------------------------------

#ifndef	FIREBALL_BULLET
#define	FIREBALL_BULLET

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class FireBullet : public Piece																									//	a fireball bullet
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			damage;																												//	how much damage this bullet does
				PiecePtr	shooter;																											//	the piece that fired this bullet
				SArrayPtr	splut;																												//	the splut sprite
	public:																																				//	members available externally
				FireBullet (Real radius, CPoint location, SArrayPtr spr,								//	constructor
								Real damage, const PiecePtr &shooter, SArrayPtr splut);					//	constructor (continued)
virtual	~FireBullet (void);																											//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	FIREBALL_BULLET
