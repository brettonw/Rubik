//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:				Jeremy Biddle
//	Revisions:				02/07/97
//
//------------------------------------------------------------------------------

#ifndef GNATTY_SHIP
#define GNATTY_SHIP

//------------------------------------------------------------------------------
#include "weapon ship.h"

//------------------------------------------------------------------------------
class GnattyShip : public WeaponShip
{
	private:
	protected:
	public:
				GnattyShip (Player *owner, Real radius, CPoint tLoc, Real rLoc,
										SArrayPtr	ship, int numWeapons);
virtual	~GnattyShip (void);
virtual	void			Explode (const PiecePtr &piece);
				void			Suicide (const PiecePtr &piece);
};

//------------------------------------------------------------------------------

#endif // GNATTY_SHIP
