//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Jeremy Biddle
//	Revisions:			01/31/97: creation.  (JB)
//
//------------------------------------------------------------------------------

#ifndef FURNACE
#define FURNACE

//------------------------------------------------------------------------------
#include "piece.h"
#include "sprite array ptr.h"
#include "weapon.h"

//------------------------------------------------------------------------------
class	Furnace : public Piece
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:
				Weapon		*weapon;																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Furnace (Player *own, Real radius, CPoint tLoc, Real rLoc, SArrayPtr spr);
virtual	~Furnace (void);
virtual	Bool			Step (Real time);
virtual	Real			TraceRay (CPoint origin, CVector direction);
				void			SetWeapon (Weapon *weapon);
				Weapon		*GetWeapon (void);
				Bool			Fire (void);
};																																							//	end

//------------------------------------------------------------------------------
inline	void	Furnace::SetWeapon (Weapon *weap)																	//	set the weapon
{																																								//	begin
	weapon = weap;																																//	copy the pointer
}																																								//	end

//------------------------------------------------------------------------------
inline	Weapon *Furnace::GetWeapon (void)																				//	get the weapon
{																																								//	begin
	return weapon;																																//	return the weapon
}																																								//	end

//------------------------------------------------------------------------------
inline	Bool	Furnace::Fire (void)																								//	fire the weapon
{																																								//	begin
	if (weapon)																																		//	if the selected weapon is a valid weapon and we are alive
		return weapon->Fire ();																											//	fire the current weapon and return the result
	return FALSE;																																	//	return false to indicate failure
}																																								//	end

//------------------------------------------------------------------------------

#endif // BLAST_FURNACE