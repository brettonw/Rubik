//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/19/96
//
//------------------------------------------------------------------------------

#ifndef	WEAPON
#define	WEAPON

//------------------------------------------------------------------------------
#include "piece ptr.h"
#include "lock ptr.h"

//------------------------------------------------------------------------------
enum	{WEAPON_READY = 0, WEAPON_RELOAD, WEAPON_HEAT, WEAPON_EMPTY};							//	weapon status values

//------------------------------------------------------------------------------
class Weapon																																		//	a weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Real			reload;																												//	remaining reload time for this weapon
				Real			rate;																													//	firing rate for this weapon
				Real			range;																												//	effective range of this weapon
				PiecePtr	owner;																												//	the weapon owner
	public:																																				//	members available externally
				Weapon (Real rate, Real range, const PiecePtr &owner);									//	constructor
virtual	~Weapon (void);																													//	destructor
virtual	void			Reload (Real time);																						//	reload the weapon
virtual	void			SetLock (const LockPtr &lock);																//	set the weapon lock, if applicable
virtual	Bool			Fire (void);																									//	fire the weapon
virtual	Real			Range (void);																									//	return the weapon range
virtual	Real			Velocity (void) = 0;																					//	return the muzzle velocity of the weapon
virtual	int				Status (void) = 0;																						//	return the status of the current weapon
virtual	int				AmmoCount (void) = 0;																					//	return the ammo count for the weapon
virtual	void			AddAmmoCount (int ammo) = 0;																	//	add ammo to the weapon
virtual	CPoint		MuzzlePt (void) = 0;																					//	return the weapon muzzle point
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	WEAPON
