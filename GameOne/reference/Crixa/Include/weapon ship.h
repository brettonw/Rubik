//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Last Revision:	01/29/97
//
//------------------------------------------------------------------------------

#ifndef	WEAPON_SHIP
#define	WEAPON_SHIP

//------------------------------------------------------------------------------
#include "ship.h"
#include "weapon.h"

//------------------------------------------------------------------------------
class WeaponShip : public Ship																									//	a ship with a weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				numWeapons;																										//	how many weapons this ship is carrying
				int				curWeapon;																										//	which one is currently selected
				Weapon		**weapon;																											//	pointer to the weapon
	public:																																				//	members available externally
				WeaponShip (Player *owner, Real radius, CPoint loc, Real rloc,					//	constructor
										SArrayPtr ship, int numWeapons);														//	constructor (continued)
virtual	~WeaponShip (void);																											//	destructor
virtual	Bool			Step (Real time);																							//	step the piece ahead in time
virtual	void			SetWeapon (int i, Weapon *weapon);														//	set the weapon
virtual	Bool			SelectWeapon (int i);																					//	select the ith weapon
virtual	int				CurrentWeapon (void);																					//	return the current weapon selection
virtual	Weapon		*GetCurrentWeapon (void);																			//	return a pointer to the current weapon
virtual	Bool			WeaponAvailable (int i);																			//	return whether or not the indexed weapon is available
				Weapon		*GetWeapon(int i);																						//	ds: return a pointer to the specified weapon
				Bool			Fire (void);																									//	fire the weapon
				Real			Aim (CVector axis);																						//	aim the ship to fire along a certain axis
				Real			AimAt (CPoint pt);																						//	aim the ship to fire at a certain point
};																																							//	end

//------------------------------------------------------------------------------
inline	Bool	WeaponShip::Fire (void)																						//	fire the weapon
{																																								//	begin
	if ((weapon[curWeapon]) && (life > ZERO) && (stun <= ZERO))										//	if the selected weapon is a valid weapon and we are alive
		return weapon[curWeapon]->Fire ();																					//	fire the current weapon and return the result
	return FALSE;																																	//	return false to indicate failure
}																																								//	end

//------------------------------------------------------------------------------
inline	Weapon *WeaponShip::GetWeapon(int i)																		//	ds: return a pointer to the specified weapon
{																																								//	begin
	return weapon[i];																															//	return the selected weapon
}																																								//	end

//------------------------------------------------------------------------------

#endif	//	WEAPON_SHIP
