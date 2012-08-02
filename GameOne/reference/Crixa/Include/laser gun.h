//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	08/14/96
//
//------------------------------------------------------------------------------

#ifndef	LASER
#define	LASER

//------------------------------------------------------------------------------
#include "gun.h"
#include "pixel.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class LaserGun : public Gun																											//	a laser gun
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Pixel			color;																												//	the color of the laser
				SArrayPtr	splut;
	public:																																				//	members available externally
				LaserGun (Real rate, Real range, const PiecePtr &owner, CPoint muzzle,	//	constructor
								Real damage, Pixel color, SArrayPtr splut, int shotSound);			//	constructor (continued)
virtual	~LaserGun (void);																												//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	LASER
