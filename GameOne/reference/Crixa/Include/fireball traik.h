//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/01/97
//
//------------------------------------------------------------------------------

#ifndef	FIREBALL_TRAIK
#define	FIREBALL_TRAIK

//------------------------------------------------------------------------------
#include "gun.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class FTraik : public Gun																												//	a basic projectile weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	bulletSprite;																									//	sprite array for the projectile
				SArrayPtr	splutSprite;																									//	splut made by the bullet
				Real			overMass;																											//	the reciprocal of the mass of the bullet to fire
				Real			recoil;																												//	the recoil factor
	public:																																				//	members available externally
				FTraik (Real rate, Real range, const PiecePtr &owner, CPoint muzzle,		//	constructor
								Real force, Real radius, Real damage, SArrayPtr bullet, 				//	constructor (continued)
								SArrayPtr splut, int shotSound, Real recoil);										//	constructor (continued)
virtual	~FTraik (void);																													//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	FIREBALL_TRAIK
