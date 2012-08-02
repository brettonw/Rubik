//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/20/96
//
//------------------------------------------------------------------------------

#ifndef	KORVES_SHIP
#define	KORVES_SHIP

//------------------------------------------------------------------------------
#include "weapon ship.h"

//------------------------------------------------------------------------------
class KorvesShip : public WeaponShip																						//	a korves base
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				KorvesShip (Player *owner, Real radius, CPoint loc, Real rloc,					//	constructor
										SArrayPtr ship);																						//	constructor (continued)
virtual	~KorvesShip (void);																											//	destructor
virtual	Bool			AddResource (int type, int number);														//	add a resource to the piece
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	KORVES_SHIP
