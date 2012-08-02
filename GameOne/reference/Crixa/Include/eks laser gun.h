//------------------------------------------------------------------------------
//	Copyright ©1996 qualia, inc. All rights reserved.
//
//	02/21/97	JB		creation.
//
//------------------------------------------------------------------------------

#ifndef	EKS_LASER
#define	EKS_LASER

//------------------------------------------------------------------------------
#include "gun.h"
#include "pixel.h"
#include "sprite array ptr.h"

//------------------------------------------------------------------------------
class EksLaserGun : public Gun																											//	a laser gun
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				Pixel			color;																												//	the color of the laser
				SArrayPtr	splut;
	public:																																				//	members available externally
				EksLaserGun (Real rate, Real range, const PiecePtr &owner, CPoint muzzle,	//	constructor
								Real damage, Pixel color, SArrayPtr splut, int shotSound);			//	constructor (continued)
virtual	~EksLaserGun (void);																												//	destructor
virtual	void			Shoot (void);																									//	shoot the projectile
};																																							//	end

//------------------------------------------------------------------------------

#endif	//	EKS_LASER
