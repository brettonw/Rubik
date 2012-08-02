//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	09/18/96
//
//------------------------------------------------------------------------------

#ifndef	MINE
#define	MINE

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "sprite array ptr.h"
#include "lock ptr.h"

//------------------------------------------------------------------------------
class Mine : public Piece																												//	a mine
{																																								//	begin
	friend	class MinePlay;																												//	the mine player class can touch me anywhere
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			damage;																												//	how much damage this bullet does
				LockPtr		target;																												//	the lock on the target
				Bool			damaged;																											//	true if the mine has been damaged
	public:																																				//	members available externally
				Mine (Player *owner, Real radius, CPoint location, SArrayPtr spr, 			//	constructor
							Real damage);																											//	constructor (continued)
virtual	~Mine (void);																														//	destructor
virtual	int				PreCollide (const PiecePtr &piece, Sect *sect);								//	do some work before a collision with another piece
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the piece
virtual	Real			TraceRay (CPoint origin, CVector direction);									//	return the time at which a ray intersects this piece
				void			Explode (void);																								//	explode the mine
				Bool			Damaged (void);																								//	return the damaged flag
};																																							//	end

//------------------------------------------------------------------------------
inline	Bool	Mine::Damaged (void)																							//	return the damaged flag
{																																								//	begin
	return damaged;																																//	return whether or not the mine has been damaged
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	MINE
