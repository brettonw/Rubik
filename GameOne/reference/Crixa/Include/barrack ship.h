//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Last Revision:	02/13/97 
//
//------------------------------------------------------------------------------

#ifndef	BARRACK_SHIP
#define	BARRACK_SHIP

//------------------------------------------------------------------------------
#include "ship.h"
#include "fixed switch sequence.h"

//------------------------------------------------------------------------------
class BarrackShip : public Ship
{
	private:
	protected:
				SeqPtr		switchSeq;
	public:
				BarrackShip (Player *owner, Real radius, CPoint loc, Real orient, cstr name);
virtual	~BarrackShip (void);
virtual	void			Explode (const PiecePtr &piece);
				void			Switch (int state);
};

//------------------------------------------------------------------------------
inline	void	BarrackShip::Switch (int state)
{
	((FixedSwitchSeq *) switchSeq ())->Switch (state);
}

//------------------------------------------------------------------------------

#endif // BARRACK_SHIP
