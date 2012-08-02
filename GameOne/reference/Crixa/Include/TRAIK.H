//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/12/96
//
//------------------------------------------------------------------------------

#ifndef	TRAIK
#define	TRAIK

//------------------------------------------------------------------------------
#include "gun.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class Traik : public Gun																												//	a basic projectile weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	bulletSprite;																									//	sprite array for the projectile
				SArrayPtr	splutSprite;																									//	splut made by the bullet
				Real			overMass;																											//	the reciprocal of the mass of the bullet to fire
				Real			recoil;																												//	the recoil factor
	public:																																				//	members available externally
				Traik (Real rate, Real range, const PiecePtr &owner, CPoint muzzle,			//	constructor
								Real force, Real radius, Real damage, SArrayPtr bullet, 				//	constructor (continued)
								SArrayPtr splut, int shotSound, Real recoil);										//	constructor (continued)
virtual	~Traik (void);																													//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	TRAIK
