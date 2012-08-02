//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/01/97
//
//------------------------------------------------------------------------------

#ifndef	FLECHETTE
#define	FLECHETTE

//------------------------------------------------------------------------------
#include "gun.h"
#include "pixel.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class Flechette : public Gun																										//	a shotgun weapon
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				int				numBlasts;																										//	how many blasts to go with each shot of the weapon
				Real			spread;																												//	the angle spread of the weapon
				Pixel			color;																												//	particle color
				SArrayPtr	bullet;																												//	bullet
				SArrayPtr	splut;																												//	bullet splut
	public:																																				//	members available externally
				Flechette (Real rate, Real range, const PiecePtr &owner, CPoint muzzle,	//	constructor
										Real force, Real damage, int num, Real spread, Pixel color,	//	constructor (continued)
										SArrayPtr bullet, SArrayPtr splut, int shotSound);					//	constructor (continued)
virtual	~Flechette (void);																											//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
virtual	Real			Velocity (void);																							//	return the muzzle velocity of the weapon
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	FLECHETTE
