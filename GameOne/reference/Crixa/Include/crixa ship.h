//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//
//	02/19/97	JB		ammo hack for adding ammo from ammo upgraders 
//	03/09/97	DS		removed extralife variable...overriding piece life instead.
//------------------------------------------------------------------------------

#ifndef	CRIXA_SHIP
#define	CRIXA_SHIP
 
//------------------------------------------------------------------------------
#include "weapon ship.h"

//------------------------------------------------------------------------------
class CrixaShip : public WeaponShip																							//	a crixa ship
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			extralife;																										//	ds: 0.0 = no armor, 1.0 = level 1 armor, 2.0 = level 2, etc
				Real			power;																												//	ds: power is expendable more like ammo
				int				maxlevel;																											//	ds: max value of power and extralife depend on this multiplier
				int				*maxammo;																											//	jb: array of max ammo values
				int				*ammocost;																										//	jb:	array of costs associated with the ammo
	public:																																				//	members available externally
				CrixaShip (Player *owner, Real radius, CPoint loc, Real rloc,						//	constructor
										SArrayPtr ship);																						//	constructor (continued)
virtual	~CrixaShip (void);																											//	destructor
virtual	void			Damage (const PiecePtr &piece, Real damage);									//	damage the piece
virtual	Bool			AddResource (int type, int number);														//	add a resource to the piece
virtual	void			DropStuff (void);																							//	drop everything we are carrying
				Real			Power();																											//	ds: return power available
				int				MaxLevel();																										//	ds: return maximum level of extralife and power
				void			AddPower(Real pow);																						//	ds: add some amount of power (up to maxlevel * 20)
				Bool			UpgradeLevel(int level);																			//	ds: not as obvious as it seems...
};																																							//	end

//------------------------------------------------------------------------------
inline	Real	CrixaShip::Power() 																								//	ds: return power available
{																																								//	begin
	return power;
}																																								//	end

//------------------------------------------------------------------------------
inline	int		CrixaShip::MaxLevel() 																						//	ds: return maximum level of extralife and power
{																																								//	begin
	return maxlevel;
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	CRIXA_SHIP
