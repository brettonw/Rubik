//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/12/96
//
//------------------------------------------------------------------------------

#ifndef	GUN
#define	GUN

//------------------------------------------------------------------------------
#include "weapon.h"

//------------------------------------------------------------------------------
class Gun : public Weapon																												//	a projectile weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				shotSound;																										//	the sound to play when the gun fires
				Point_2d	muzzle;																												//	the muzzle point
				Real			force;																												//	ejection force for the projectile
				Real			radius;																												//	radius of the projectile
				Real			damage;																												//	damage of the projectile
				Real			velocity;																											//	muzzle velocity of the weapon
				Real			heatRate;																											//	the rate of heating for the weapon
				Real			coolRate;																											//	the rate of cooling for the weapon
				Real			heat;																													//	the current heat value
				int				ammo;																													//	how much ammo is left
				int				status;																												//	the weapon status
	public:																																				//	members available externally
				Gun (Real rate, Real range, const PiecePtr &owner, CPoint muzzle,				//	constructor
							Real force, Real radius, Real damage, int shotSound);							//	constructor (continued)
virtual	~Gun (void);																														//	destructor
virtual	void			Reload (Real time);																						//	reload the weapon
virtual	Bool			Fire (void);																									//	fire the weapon
virtual	Real			Velocity (void);																							//	return the muzzle velocity of the weapon
virtual	int				Status (void);																								//	return the status of the current weapon
virtual	int				AmmoCount (void);																							//	return the ammo count for the weapon
virtual	void			AddAmmoCount (int ammo);																			//	add ammo to the weapon
virtual	void			Shoot (void) = 0;																							//	shoot the projectile
				void			SetHeating (Real heat, Real cool);														//	set the heating and cooling rate of the weapon
virtual	CPoint		MuzzlePt (void);																							//	return the weapon muzzle point
};																																							//	end

//------------------------------------------------------------------------------
inline	void	Gun::SetHeating (Real h, Real c)																	//	set the heating and cooling rate of the weapon
{																																								//	begin
	heatRate = h;																																	//	copy the heating rate
	coolRate = c;																																	//	copy the cooling rate
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	GUN
