//------------------------------------------------------------------------------
//	Copyright ©1997 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			02/07/97: creation. (JB)
//
//------------------------------------------------------------------------------

/*//----------------------------------------------------------------------------

DESCRIPTION:
the gnatty ship is a custom ship for the gnatty dread, which blows itself up
if it is committing suicide, otherwise it dies in a rather orthodox manner.

NOTE:  the suicide function is probably where the blowing up should take
place, rather than the SelfDestruct function in the Gnatty class itself.

*///----------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "gnatty ship.h"

//------------------------------------------------------------------------------
GnattyShip::GnattyShip (Player *owner, Real radius, CPoint tLoc, Real rLoc,
												SArrayPtr	ship, int numWeapons) :
WeaponShip (owner, radius, tLoc, rLoc, ship, numWeapons)
{
}

//------------------------------------------------------------------------------
GnattyShip::~GnattyShip (void)
{
}

//------------------------------------------------------------------------------
void	GnattyShip::Explode (const PiecePtr &piece)
{
	if (piece ())
		WeaponShip::Explode (piece);
	else
		Suicide (piece);
}

//------------------------------------------------------------------------------
void	GnattyShip::Suicide (const PiecePtr &piece)
{
}

//------------------------------------------------------------------------------
