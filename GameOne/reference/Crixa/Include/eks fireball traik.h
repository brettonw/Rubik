//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	02/21/97	JB		creation.
//
//------------------------------------------------------------------------------

#ifndef	EKS_FIREBALL_TRAIK
#define EKS_FIREBALL_TRAIK

//------------------------------------------------------------------------------
#include "gun.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class	EksFTraik : public Gun																										//	an eksekaren-hurled flaming ball of death
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	bulletSprite;																									//	sprite array for the projectile
				SArrayPtr	splutSprite;																									//	splut made by the bullet
				Real			overMass;																											//	the reciprocal of the mass of the bullet to fire
				Real			recoil;																												//	the recoil factor
	public:																																				//	members available externally
				EksFTraik (Real rate, Real range, const PiecePtr &owner, CPoint muzzle,	//	constructor
									 Real force, Real radius, Real damage, SArrayPtr bullet, 			//	constructor (continued)
									 SArrayPtr splut, int shotSound, Real recoil);								//	constructor (continued)
virtual	~EksFTraik (void);																											//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
};

//------------------------------------------------------------------------------

#endif // EKS_FIREBALL_TRAIK
