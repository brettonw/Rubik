//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/12/96
//
//------------------------------------------------------------------------------

#ifndef	PLASMA_GUN
#define	PLASMA_GUN

//------------------------------------------------------------------------------
#include "gun.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class PlasmaGun : public Gun																										//	a plasma gun
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				SArrayPtr	plasmaSprite;																									//	sprite array for the plasma
				Real			speed;																												//	the blossom speed of the plasma jet
	public:																																				//	members available externally
				PlasmaGun (Real range, const PiecePtr &owner, CPoint muzzle, Real speed,//	constructor
								Real damage, SArrayPtr plasma, int shotSound);									//	constructor (continued)
virtual	~PlasmaGun (void);																											//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	PLASMA_GUN
