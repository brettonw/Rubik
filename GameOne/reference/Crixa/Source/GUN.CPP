//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/10/97
//
//	Modified by:		Dave Seah
//	Last Revision:	01/23/97				Changed gun reload status in AddAmmo
//																	Modified Fire() weapon status
//  Modified by:		Mark Kern
//	Last Revision		02/09/97				Changed PlaySound(shotsound) to include pieceptr info
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "gun.h"
#include "sound mgr.h"

//------------------------------------------------------------------------------
Gun::Gun (Real rat, Real ran, const PiecePtr &own, CPoint muz, Real frc,				//	constructor
					Real rad, Real dam, int snd) :																				//	constructor (continued)
Weapon (rat, ran, own)																													//	constructor (continued)
{																																								//	begin
	muzzle = muz;																																	//	copy the muzzle point
	force = frc;																																	//	copy the ejection force
	radius = rad;																																	//	copy the projectile radius
	damage = dam;																																	//	copy the projectile damage
	heatRate = ZERO;																															//	default heating rate
	coolRate = ZERO;																															//	default cooling rate
	heat = ZERO;																																	//	no heat on the weapon
	status = WEAPON_EMPTY;																												//	start with the weapon empty
	ammo = 0;																																			//	copy the starting ammo count
	shotSound = snd;																															//	copy the shot sound
}																																								//	end

//------------------------------------------------------------------------------
Gun::~Gun (void)																																//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Gun::Reload (Real time)																										//	reload the weapon
{																																								//	begin
	heat -= time;																																	//	cool the gun
	if (heat < ZERO)																															//	if the heat value is negative
		heat = ZERO;																																//	clamp it to zero
	Weapon::Reload (time);																												//	do the parental thing
	if ((reload >= rate) && (ammo > 0))																						//	if the weapon is reloaded and has ammo
		status = WEAPON_READY;																											//	set the weapon status to ready
}																																								//	end

//------------------------------------------------------------------------------
Bool	Gun::Fire (void)																													//	fire the weapon
{																																								//	begin
	if (heat < R(1.0))																														//	if the weapon is cool enough to fire
	{																																							//	begin
		if (ammo > 0)																																//	if the weapon has ammo
		{																																						//	begin
			if (Weapon::Fire ())																											//	if the gun is firing
			{																																					//	begin
				Shoot ();																																//	shoot the weapon projectile
				ammo--;																																	//	decrement the ammo count
				status = (ammo == 0) ? WEAPON_EMPTY : WEAPON_RELOAD;										//	ds:set the weapon status
				heat += heatRate;																												//	heat the weapon
				if (shotSound >= 0)																											//	test to be sure the shot sound id is valid
					gSoundMgr->PlaySound (shotSound, this->owner);																			//	play the gun shot sound
				return TRUE;																														//	return true to indicate that we shot
			}																																					//	end
		}																																						//	end
		else																																				//	otherwise, the weapon is out of ammo
		{																																						//	begin
			status = WEAPON_EMPTY;																										//	set the status to empty
			reload = ZERO;																														//	set the weapon to reload
		}																																						//	end
	}																																							//	end
	else																																					//	otherwise, the weapon must enter a cooling phase of operation
	{																																							//	begin
		status = WEAPON_HEAT;																												//	set the weapon status to overheated
		reload -= coolRate;																													//	add the cooling period to the reload period
		heat = ZERO;																																//	clear the heat
	}																																							//	end
	return FALSE;																																	//	return false to indicate that we didn't shoot
}																																								//	end

//------------------------------------------------------------------------------
Real	Gun::Velocity (void)																											//	return the muzzle velocity of the weapon
{																																								//	begin
	return velocity;																															//	return the velocity value
}																																								//	end

//------------------------------------------------------------------------------
int		Gun::Status (void)																												//	return the status of the current weapon
{																																								//	begin
	return status;																																//	return the weapon status
}																																								//	end

//------------------------------------------------------------------------------
int		Gun::AmmoCount (void)																											//	return the ammo count for the weapon
{																																								//	begin
	return ammo;																																	//	return the ammo count
}																																								//	end

//------------------------------------------------------------------------------
void	Gun::AddAmmoCount (int am)																								//	add ammo to the weapon
{																																								//	begin
	ammo += am;																																		//	add the new ammo to the ammo count
	status = WEAPON_READY;																												//	set the weapon status to reload
}																																								//	end

//------------------------------------------------------------------------------
CPoint	Gun::MuzzlePt (void)																										//	return the weapon muzzle point
{																																								//	begin
	return muzzle;																																//	return the muzzle point
}																																								//	end

//------------------------------------------------------------------------------
