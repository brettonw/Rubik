//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	07/27/96
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "weapon.h"

//------------------------------------------------------------------------------
Weapon::Weapon (Real rat, Real ran, const PiecePtr &own)												//	constructor
{																																								//	begin
	rate = rat;																																		//	copy the shots per second
	if (rate != ZERO)																															//	if the rate is not zero (meaning continuous shot)
		rate = R(1.0) / rate;																												//	compute the reload rate
	range = ran;																																	//	copy the weapon range
	owner = own;																																	//	copy the weapon owner
	reload = ZERO;																																//	set the reload time to 0
}																																								//	end

//------------------------------------------------------------------------------
Weapon::~Weapon (void)																													//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
void	Weapon::Reload (Real time)																								//	reload the weapon
{																																								//	begin
	reload += time;																																//	add the elapsed time to the reload time
}																																								//	end

//------------------------------------------------------------------------------
void	Weapon::SetLock (const LockPtr &loc)																			//	set the weapon lock, if applicable
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
Bool	Weapon::Fire (void)																												//	fire the weapon
{																																								//	begin
	if (reload >= rate)																														//	if the elapsed reload time has exceeded the reload rate
	{																																							//	begin
		reload = ZERO;																															//	zero the elapsed reload time
		return TRUE;																																//	return true to indicate firing is successful
	}																																							//	end
	return FALSE;																																	//	return false to indicate that the weapon has not yet reloaded
}																																								//	end

//------------------------------------------------------------------------------
Real	Weapon::Range (void)																											//	return the weapon range
{																																								//	begin
	return range;																																	//	return the range
}																																								//	end

//------------------------------------------------------------------------------
