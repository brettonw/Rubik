//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	02/03/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "weapon ship.h"
#include "rotating.h"

//------------------------------------------------------------------------------
WeaponShip::WeaponShip (Player *own, Real radius, CPoint loc, Real rloc,				//	constructor
												SArrayPtr ship, int num) :															//	constructor (continued)
Ship (own, radius, loc, rloc, ship)																							//	constructor (continued)
{																																								//	begin
	numWeapons = num;																															//	copy the weapon count
	curWeapon = 0;																																//	set the current weapon to 0
	weapon = new Weapon*[numWeapons];																							//	allocate the weapon array
	for (int i = 0; i < numWeapons; i++)																					//	loop over all of the weapon slots
		weapon[i] = 0;																															//	setting them as empty
}																																								//	end

//------------------------------------------------------------------------------
WeaponShip::~WeaponShip (void)																									//	destructor
{																																								//	begin
	for (int i = 0; i < numWeapons; i++)																					//	loop over all of the weapon slots
		if (weapon[i])																															//	if a slot is occupied
			delete weapon[i];																													//	release the weapon
	delete weapon;																																//	release the weapon array
}																																								//	end

//------------------------------------------------------------------------------
Bool	WeaponShip::Step (Real time)																							//	step the piece ahead in time
{																																								//	begin
	if (weapon[curWeapon])																												//	if the selected weapon is a valid weapon
		weapon[curWeapon]->Reload (time);																						//	reload it
	return Ship::Step (time);																											//	do the parent class step and return the result
}																																								//	end

//------------------------------------------------------------------------------
void	WeaponShip::SetWeapon (int i, Weapon *weap)																//	set the weapon
{																																								//	begin
	weapon[i] = weap;																															//	copy the pointer
}																																								//	end

//------------------------------------------------------------------------------
Bool	WeaponShip::SelectWeapon (int i)																					//	select the ith weapon
{																																								//	begin
	if (weapon[i])																																//	if the selected weapon slot is occupied
	{																																							//	begin
		curWeapon = i;																															//	make that the current weapon
		return TRUE;																																//	return true indicating success
	}																																							//	end
	return FALSE;																																	//	return false indicating that the desired weapon is not available
}																																								//	end

//------------------------------------------------------------------------------
int		WeaponShip::CurrentWeapon (void)																					//	return the current weapon selection
{																																								//	begin
	return curWeapon;																															//	return the current weapon index
}																																								//	end

//------------------------------------------------------------------------------
Weapon	*WeaponShip::GetCurrentWeapon (void)																		//	return a pointer to the current weapon
{																																								//	begin
	return weapon[curWeapon];																											//	return the current weapon
}																																								//	end

//------------------------------------------------------------------------------
Bool	WeaponShip::WeaponAvailable (int i)																				//	return whether or not the indexed weapon is available
{																																								//	begin
	if (i < numWeapons)																														//	if the index does not go outside the range of weapons
		return weapon[i] ? TRUE : FALSE;																						//	return whether or not the indexed slot hasa weapon in it
	return FALSE;																																	//	otherwise, just return false
}																																								//	end

//------------------------------------------------------------------------------
Real	WeaponShip::Aim (CVector axis)																						//	aim the ship to fire along a certain axis
{																																								//	begin
	return	Point (axis - body->TVelocity ());																		//	point the ship so that a bullet travels along the target vector
}																																								//	end

//------------------------------------------------------------------------------
Real	WeaponShip::AimAt (CPoint pt)																							//	aim the ship to fire at a certain point
{																																								//	begin
	Point_2d	muzzle = weapon[curWeapon]->MuzzlePt ();														//	get the muzzle point
	Rotating	*rot = (Rotating *) visual;																					//	get the rotating visual associated with the ship
	Vector_2d	muzzleOffset = rot->GetDirection ()[1] * muzzle[X];									//	compute the x offset of the muzzle point for aiming
	muzzle = body->TLocation () + muzzleOffset;																		//	add the muzzle offset to the ship location to get the real muzzle point
	//return	Point ((pt - muzzle) - body->TVelocity ());														//	point the ship so that a bullet travels along the vector to the target
	return	Point (pt - muzzle);																									//	point the ship at the target point
}																																								//	end

//------------------------------------------------------------------------------
