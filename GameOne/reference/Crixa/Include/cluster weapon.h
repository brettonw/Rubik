//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/19/96
//
//------------------------------------------------------------------------------

#ifndef	CLUSTER_WEAPON
#define	CLUSTER_WEAPON

//------------------------------------------------------------------------------
#include "weapon.h"

//------------------------------------------------------------------------------
class Cluster : public Weapon																										//	a cluster of weapons
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				numWeapons;																										//	the number of weapons in this cluster
				int				curWeapon;																										//	the currently selected weapon
				Weapon		**weapon;																											//	the weapon array
	public:																																				//	members available externally
				Cluster (const PiecePtr &owner, int count, ...);												//	constructor
virtual	~Cluster (void);																												//	destructor
virtual	void			Reload (Real time);																						//	reload the weapon
virtual	void			SetLock (const LockPtr &lock);																//	set the weapon lock, if applicable
virtual	Bool			Fire (void);																									//	fire the weapon
virtual	Real			Range (void);																									//	return the weapon range
virtual	Real			Velocity (void);																							//	return the muzzle velocity of the weapon
virtual	int				Status (void);																								//	return the status of the current weapon
virtual	int				AmmoCount (void);																							//	return the ammo count for the weapon
virtual	void			AddAmmoCount (int ammo);																			//	add ammo to the weapon
virtual	CPoint		MuzzlePt (void);																							//	return the weapon muzzle point
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	CLUSTER_WEAPON
