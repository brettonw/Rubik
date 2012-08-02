//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/19/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "cluster weapon.h"

//------------------------------------------------------------------------------
Cluster::Cluster (const PiecePtr &ow, int count, ...) : Weapon (ZERO, ZERO, ow)	//	constructor
{																																								//	begin
	numWeapons = count;																														//	copy the weapon count
	curWeapon = 0;																																//	start with the first one
	weapon = new Weapon*[numWeapons];																							//	allocate the weapon array
	va_list	var;																																	//	variable argument list
	va_start (var, count);																												//	get the beginning of the variable arguments
	for (int i = 0; i < numWeapons; i++)																					//	loop over all of the weapon slots
		weapon[i] = va_arg (var, Weapon*);																					//	copying the weapon pointer from the argument list
	va_end (var);																																	//	finish up with arguments
}																																								//	end

//------------------------------------------------------------------------------
Cluster::~Cluster (void)																												//	destructor
{																																								//	begin
	for (int i = 0; i < numWeapons; i++)																					//	loop over all of the weapon slots
		delete weapon[i];																														//	release the contained weapons
	delete weapon;																																//	release the weapon array
}																																								//	end

//------------------------------------------------------------------------------
void	Cluster::Reload (Real time)																								//	reload the weapon
{																																								//	begin
	for (int i = 0; i < numWeapons; i++)																					//	loop over all of the weapon slots
		weapon[i]->Reload (time);																										//	reloading all of them
}																																								//	end

//------------------------------------------------------------------------------
void	Cluster::SetLock (const LockPtr &lock)																		//	set the weapon lock, if applicable
{																																								//	begin
	for (int i = 0; i < numWeapons; i++)																					//	loop over all of the weapon slots
		weapon[i]->SetLock (lock);																									//	setting the lock for each one
}																																								//	end

//------------------------------------------------------------------------------
Bool	Cluster::Fire (void)																											//	fire the weapon
{																																								//	begin
	if (weapon[curWeapon]->Fire ())																								//	if the current weapon fired successfully
	{																																							//	begin
		curWeapon++;																																//	move on to the next weapon
		if (curWeapon == numWeapons)																								//	if the next weapon is outside the size of the weapon array
			curWeapon = 0;																														//	start over at the beginning
		return TRUE;																																//	return true to indicate that the weapon fired
	}																																							//	end
	return FALSE;																																	//	return false to indicate that the weapon did not fire
}																																								//	end

//------------------------------------------------------------------------------
Real	Cluster::Range (void)																											//	return the weapon range
{																																								//	begin
	return weapon[curWeapon]->Range ();																						//	return the range of the current weapon
}																																								//	end

//------------------------------------------------------------------------------
Real	Cluster::Velocity (void)																									//	return the muzzle velocity of the weapon
{																																								//	begin
	return weapon[curWeapon]->Velocity ();																				//	return the velocity of the current weapon
}																																								//	end

//------------------------------------------------------------------------------
int		Cluster::Status (void)																										//	return the status of the current weapon
{																																								//	begin
	return weapon[curWeapon]->Status ();																					//	return the status of the current weapon
}																																								//	end

//------------------------------------------------------------------------------
int		Cluster::AmmoCount (void)																									//	return the ammo count for the weapon
{																																								//	begin
	return weapon[curWeapon]->AmmoCount ();																				//	return the ammo count of the current weapon
}																																								//	end

//------------------------------------------------------------------------------
void	Cluster::AddAmmoCount (int am)																						//	add ammo to the weapon
{																																								//	begin
	for (int i = 0; i < numWeapons; i++)																					//	loop over all of the weapon slots
		weapon[i]->AddAmmoCount (am);																								//	adding the ammo to each one
}																																								//	end

//------------------------------------------------------------------------------
CPoint	Cluster::MuzzlePt (void)																								//	return the weapon muzzle point
{																																								//	begin
	return weapon[curWeapon]->MuzzlePt ();																				//	return the muzzle point of the current weapon
}																																								//	end

//------------------------------------------------------------------------------
