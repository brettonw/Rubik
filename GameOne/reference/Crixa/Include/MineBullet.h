//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/01/97
//
//------------------------------------------------------------------------------

#ifndef	MINEBULLET
#define	MINEBULLET

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class MineBullet : public Piece																									//	an exploding bullet
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			damage;																												//	how much damage this bullet does
				PiecePtr	shooter;																											//	the piece that fired this bullet
				SArrayPtr	splut;																												//	the splut sprite
	public:																																				//	members available externally
				MineBullet (Real radius, CPoint location, Real orientation, SArrayPtr spr,	//	constructor
								Real damage, const PiecePtr &shooter, SArrayPtr splut);					//	constructor (continued)
virtual	~MineBullet (void);																													//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	int				PostCollide (const PiecePtr &piece, Sect *sect);							//	do some work after a collision with another piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	MINEBULLET
